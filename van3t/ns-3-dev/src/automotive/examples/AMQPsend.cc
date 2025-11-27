#include <proton/delivery.hpp>
#include <proton/connection.hpp>
#include <proton/duration.hpp>
#include <proton/container.hpp>
#include <proton/message.hpp> 
#include <proton/sender.hpp>  
#include <proton/target.hpp>  
#include <proton/tracker.hpp> 
#include <proton/transport.hpp> 
#include <proton/connection_options.hpp>
#include <proton/error_condition.hpp> 
#include <proton/types.hpp> 
#include <proton/link.hpp>
#include <proton/work_queue.hpp> 

#include <memory>
#include <chrono>
#include <ctime>
#include <functional>
#include <thread>
#include <queue>  
#include <mutex>  
#include <atomic> 
#include <iostream>
#include <string>   
#include "AMQPsend.h"


AMQPsend::AMQPsend(const std::string& brokerURI, const std::string& topicName)
  : brokerURI(brokerURI),
    topicName(topicName),
    container(*this),
    running(true),
    m_work_queue(nullptr) // Initialize work queue pointer
{

  // container thread
  m_containerThread = std::thread([this]() {
    auto id = std::this_thread::get_id();
    try {
        container.run();
    } catch (const std::exception& e) {
        running = false; // Signal other threads to stop
        queueCv.notify_all();
    }
  });

  // poll thread
  pollThread = std::thread([this]() {
    auto id = std::this_thread::get_id();
    std::unique_lock<std::mutex> lk(queueMutex);
    while (running) {
      queueCv.wait(lk, [this] {
        return !running || (!messageQueue.empty() && m_work_queue && sender.credit() > 0);
      });
      if (!running) break;

      // Double check conditions after waking up
      if (!messageQueue.empty() && m_work_queue && sender.credit() > 0) {
        auto msg = std::move(messageQueue.front());
        messageQueue.pop();

        // Unlock mutex BEFORE adding work to avoid holding lock during potential block
        lk.unlock();

        bool scheduled = m_work_queue->add([this, moved_msg = std::move(msg)]() mutable {
            // This lambda runs on the container thread
            auto container_thread_id = std::this_thread::get_id();
            if (sender.credit() > 0) {
                 try {
                    std::string msg_body_str = "[body unavailable]";
                    try { msg_body_str = proton::get<std::string>(moved_msg.body()); } catch(...) {}

                    proton::tracker trk = sender.send(moved_msg); // Send the moved message
                 } catch (const proton::error& e) {
                 }
            } 
        });

        lk.lock();

      } else {
          // Log why we didn't schedule a send
          if (messageQueue.empty()) {
          } 
      }
    }
  });
}

AMQPsend::~AMQPsend() {
  running = false;
  queueCv.notify_all();

  if (pollThread.joinable()) pollThread.join();

  container.stop();

  if (m_containerThread.joinable()) m_containerThread.join();
}

void AMQPsend::sendMessage(const std::string& message) 
{
    { 
      std::lock_guard<std::mutex> lk(queueMutex);
        messageQueue.emplace(proton::message{message});
    }
    queueCv.notify_one();
}

void AMQPsend::on_container_start(proton::container& c) {
   proton::connection_options opts; // Still create opts for potential future use (e.g., idle_timeout)

   std::string url_with_creds = "amqp://admin:admin@" + brokerURI + "/" + topicName;

   try {
       // Open sender directly, connection happens implicitly
       sender = c.open_sender(url_with_creds, opts); 
   } catch (const proton::error& e) {
   }
}

void AMQPsend::on_sender_open(proton::sender& s) {
    m_work_queue = &s.work_queue(); // Get the work queue associated with this sender

    // Notify pollThread that the sender (and work queue) is ready and might have credit
    {
        std::lock_guard<std::mutex> lk(queueMutex); 
        if (!messageQueue.empty() && s.credit() > 0) {
             queueCv.notify_one();
        }
    }
}

void AMQPsend::on_sendable(proton::sender& s) {
    // Only notify if there are actually messages waiting
    {
        std::lock_guard<std::mutex> lk(queueMutex);
        if (!messageQueue.empty()) {
            queueCv.notify_one();
        } else {
        }
    }
}

void AMQPsend::on_tracker_accept(proton::tracker& t) {
}

void AMQPsend::on_transport_close(proton::transport& t) {
  std::cerr << "[AMQPsend::on_transport_close] Transport closed." << std::endl;
  proton::error_condition err = t.error();
   if (err) {
       std::cerr << "[AMQPsend::on_transport_close] Error: " << err.what() << " (" << err.description() << ")" << std::endl;
   }
}

// --- Add other handlers as needed ---
void AMQPsend::on_error(const proton::error_condition& e) {
    std::cerr << "[AMQPsend::on_error] General AMQP Error: " << e.what() << " (" << e.description() << ")" << std::endl;
}

int
AMQPsend::GetCredit () const
{
  return m_credit.load ();
}

