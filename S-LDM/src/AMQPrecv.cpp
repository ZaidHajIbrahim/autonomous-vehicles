#include <proton/message.hpp>
#include <proton/connection_options.hpp>
#include <proton/ssl.hpp>
#include <proton/source_options.hpp>
#include <proton/receiver_options.hpp> // <<< Make sure this is included
#include <proton/codec/encoder.hpp> // Include for encoder
#include <proton/codec/decoder.hpp> // Include for decoder (if needed)
#include <proton/scalar.hpp>        // Include for scalar types like symbol
#include <proton/transport.hpp>     // <<< Add this include
#include <iostream>
#include <string>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream> // Make sure sstream is included
#include <vector>  // Make sure vector is included

#include <chrono>  // <<< Add for time
#include <ctime>   // <<< Add for time formatting
#include <iomanip> // <<< Add for time formatting
#include <stdexcept> // For exception handling
#include <atomic>
#include <proton/sender.hpp> // Include sender header if needed elsewhere
#include <proton/delivery.hpp>   // <<< ADD THIS LINE

#include "../include/utils.h" // For get_timestamp_us() if you use it
#include "AMQPrecv.h"
#include "CollisionAvoidance.h"
#include "LDMmap.h" // --- Include LDMmap header ---
#include "AMQPclient.h" // Include AMQPclient header


// Helper function for timestamp
std::string current_time_str() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = std::chrono::system_clock::to_time_t(now);
    std::tm bt = *std::localtime(&timer);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", &bt);
    return std::string(buf) + "." + std::to_string(ms.count());
}


// --- Definition for constructor taking LDMMap only ---
AMQPrecv::AMQPrecv(ldmmap::LDMMap& ldmmap)
   : m_ldmmap(ldmmap), // Initialize reference member
     m_url("amqp://admin:admin@10.10.7.254:5672"), // Provide default URL
     m_topic("topic://van3t.to.sldm") // Provide default topic (adjust if needed)
{
   std::cout << "[AMQPrecv] Constructor (LDMMap only) called." << std::endl;
   // Initialize other members if necessary
}


// --- Definition for constructor taking LDMMap, URL, Topic ---
AMQPrecv::AMQPrecv(ldmmap::LDMMap& ldmmap, const std::string &url, const std::string &topic)
   : m_ldmmap(ldmmap),
     m_url(url),
     m_topic(topic),
     m_ca()
{
   std::cout << "[AMQPrecv] Constructor (LDMMap, URL, Topic) called." << std::endl;
   // Initialize other members if necessary
}


// --- Keep Destructor Definition ---
AMQPrecv::~AMQPrecv() {
   // Cleanup if needed
   std::cout << "[AMQPrecv] Destructor called." << std::endl;
}


void AMQPrecv::set_no_local_filter(proton::source_options &opts) {
  proton::source::filter_map map;
  proton::symbol filter_key("no-local");
  proton::value filter_value;


  // The value is a specific AMQP "described type": list with symbolic descriptor
  proton::codec::encoder enc(filter_value);
  enc << proton::codec::start::described()
      << proton::symbol("apache.org:no-local-filter:list")
      << proton::codec::start::list() << proton::codec::finish()
      << proton::codec::finish();


  // In our case, the map has this one element
  map.put(filter_key, filter_value);
  opts.filters(map);
}


void AMQPrecv::on_container_start(proton::container &c) {
    std::cout << "[" << current_time_str() << " AMQPrecv] on_container_start called." << std::endl;
    proton::connection_options co;
    c.connect(m_url, co);
    std::cout << "[" << current_time_str() << " AMQPrecv] connect() called for " << m_url << std::endl;
}


void AMQPrecv::on_connection_open(proton::connection &c) {
    std::cout << "[" << current_time_str()
              << " AMQPrecv] on_container_start: opening connection to " << m_url << std::endl;

    // configure a fixed‐topic (non‐dynamic) source, then hand it to the receiver
    proton::source_options so;
    so.dynamic(false);
    so.address(m_topic);

    proton::receiver_options ro;
    ro.credit_window(100);   // 100‐message window, no auto‐topup below that
    ro.source(so);
    c.open_receiver(m_topic, ro);

    std::cout << "[" << current_time_str()
              << " AMQPrecv] open_receiver() called." << std::endl;
}

// Add handler for receiver drain event
void AMQPrecv::on_receiver_drain_finish(proton::receiver &r) {
     std::cout << "[AMQPrecv] Receiver drain finished. Current credit: " << r.credit() << std::endl;
     // Proton usually auto-replenishes credit after drain if window > 0
}

// Add handler for receiver open event
void AMQPrecv::on_receiver_open(proton::receiver &r) {
    m_receiver = r; // Store the receiver object
    std::cout << "[" << current_time_str() << " AMQPrecv] on_receiver_open for topic: " << r.source().address() << ". Initial credit: " << r.credit() << std::endl;
}

// Add handler for receiver close event
void AMQPrecv::on_receiver_close(proton::receiver &r) {
     std::cerr << "[AMQPrecv] Receiver link closed." << std::endl;
}

// Add handler for receiver error event
void AMQPrecv::on_receiver_error(proton::receiver &r) {
    std::cerr << "[" << current_time_str() << " AMQPrecv] on_receiver_error: " << r.error().what() << std::endl;
    r.close();
    r.connection().close();
}


void AMQPrecv::on_sender_open(proton::sender &protonsender) {
  m_sender=protonsender;
  m_work_queue_ptr=&m_sender.work_queue();
  m_sender_ready=true;
  std::cout << "[AMQPrecv] Sender opened (unexpected for receiver?)." << std::endl;
}


void AMQPrecv::on_sendable(proton::sender &sndr) {
  std::cout << "[AMQPrecv] Sender sendable (unexpected for receiver?)." << std::endl;
}


void AMQPrecv::on_message(proton::delivery &d, proton::message &m) {
    std::string body = proton::to_string(m.body());
    std::cout << "[" << current_time_str() << " AMQPrecv] Received: " << body << std::endl;

    try {
        m_ca.receive_message(m_ldmmap, body);
    } catch (const std::exception &e) {
        std::cerr << "[" << current_time_str()
                  << " AMQPrecv] Error in receive_message(): " << e.what() << std::endl;
    }

    d.accept();
    std::cout << "[" << current_time_str() << " AMQPrecv] Delivery accepted. Credit after: "
              << m_receiver.credit() << std::endl;
}


void AMQPrecv::on_transport_error(proton::transport &t) {
    std::cerr << "[" << current_time_str() << " AMQPrecv] on_transport_error: " << t.error().what() << std::endl;
}


void AMQPrecv::on_transport_close(proton::transport &transport) {
  std::cout << "[AMQP recv] TRANSPORT CLOSED" << std::endl;
}


void AMQPrecv::on_connection_close(proton::connection &connection) {
    std::cout << "[AMQPrecv] Connection closed." << std::endl;
    m_sender_ready = false; // Reset sender ready flag if connection closes
}


void AMQPrecv::on_connection_error(proton::connection &c) {
    std::cerr << "[" << current_time_str() << " AMQPrecv] on_connection_error: " << c.error().what() << std::endl;
    c.close();
    m_sender_ready = false; // Reset sender ready flag on error
}


void AMQPrecv::run() {
   try {
       proton::container(*this).run();
   } catch (const std::exception& e) {
       std::cerr << "[AMQPrecv::run] Exception: " << e.what() << std::endl;
   }
}


void AMQPrecv::set_work_queue(proton::work_queue* wq) {
    m_work_queue_ptr = wq;
}

void AMQPrecv::stop() {
   std::cout << "[AMQPrecv::stop] Stop function called." << std::endl;
   if (m_work_queue_ptr) {
        m_work_queue_ptr->add([this]() { // Schedule the close on the container's thread
            try {
                if (m_receiver && m_receiver.connection() && m_receiver.connection().active()) {
                    std::cout << "[AMQPrecv::stop] Closing receiver connection via work_queue." << std::endl;
                    m_receiver.connection().close(proton::error_condition("user.shutdown", "Application shutting down"));
                } else {
                     std::cout << "[AMQPrecv::stop] Receiver connection not valid/active (checked via work_queue)." << std::endl;
                }
                // Close sender if it exists and is different connection
                if (m_sender && m_sender.connection() && m_sender.connection().active() &&
                    (!m_receiver || !m_receiver.connection() || m_sender.connection() != m_receiver.connection()))
                {
                     std::cout << "[AMQPrecv::stop] Closing sender connection via work_queue." << std::endl;
                     m_sender.connection().close(proton::error_condition("user.shutdown", "Application shutting down"));
                }
            } catch (const std::exception& e) {
                std::cerr << "[AMQPrecv::stop] Exception in work_queue close: " << e.what() << std::endl;
            }
        });
         std::cout << "[AMQPrecv::stop] Close task added to work_queue." << std::endl;
    } else {
         std::cerr << "[AMQPrecv::stop] Work queue pointer is null, cannot schedule close safely." << std::endl;
         // Fallback: Attempt direct close (less safe if called from wrong thread)
         try {
              if (m_receiver && m_receiver.connection() && m_receiver.connection().active()) {
                   m_receiver.connection().close(proton::error_condition("user.shutdown", "Application shutting down (direct)"));
              }
              // Direct close for sender if needed
         } catch (const std::exception& e) {
              std::cerr << "[AMQPrecv::stop] Exception during direct close attempt: " << e.what() << std::endl;
         }
    }
   std::cout << "[AMQPrecv::stop] Finished stop function." << std::endl;
}

// definition of set_topic()
void AMQPClient::set_topic(const std::string& new_topic) {
    // use addr_ as the subscription/topic string
    addr_ = new_topic;
    std::cout << "[AMQPClient " << m_client_id << "] topic set to: " << addr_ << std::endl;
}





