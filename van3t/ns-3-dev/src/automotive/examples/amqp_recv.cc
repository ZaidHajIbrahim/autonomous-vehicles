//
// Created by carlos on 05/02/24.
//
#include <proton/message.hpp>
#include <proton/connection_options.hpp>
#include <proton/ssl.hpp>
#include <proton/source_options.hpp>
#include <proton/receiver_options.hpp>
#include <iostream>
#include "amqp_recv.h"
#include <string>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "collisionAvoidance.h"
#include "traci-client-global.h"
#include "ns3/ptr.h"                    // ← ensure Ptr<> is known


#include "ns3/simulator.h"      // for Simulator::Schedule
#include "time.h"           // for Seconds()
#include "traci-client-global.h" // For ns3::g_collisionAvoiderApp
#include "collisionAvoidance.h"  // For ns3::CollisionAvoidance

void AMQP_recv::set_no_local_filter(proton::source_options &opts) {
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

void AMQP_recv::on_container_start(proton::container &c) {
    proton::connection_options co;
    co.idle_timeout(proton::duration(10000));
    c.connect(m_url,co);
}

void AMQP_recv::on_connection_open(proton::connection &c) {
    proton::source_options so;
    set_no_local_filter(so);
    c.open_receiver(m_topic, proton::receiver_options().source(so));
    c.open_sender(m_topic);
}

void AMQP_recv::on_sender_open(proton::sender &protonsender) {
    m_sender = protonsender;
    std::cout << "[AMQP recv] Sender link opened." << std::endl; // Log sender open

    // Get the work queue pointer out of the sender's context
    m_work_queue_ptr = &m_sender.work_queue();
    if (!m_work_queue_ptr) {
         std::cerr << "[AMQP recv] CRITICAL: Failed to get work queue pointer!" << std::endl;
    }

    // Set "m_sender_ready" to true -> now the sender is ready
    m_sender_ready.store(true);
    std::cout << "[AMQP recv] Sender marked as ready." << std::endl;
}

void AMQP_recv::Send(const proton::message& msg) {
    if (!m_sender_ready.load()) {
        return;
    }
    if (!m_work_queue_ptr) {
        std::cerr << "[AMQP Send] Work queue pointer is null. Cannot schedule send." << std::endl;
        return;
    }

    // Use the work queue to schedule the send operation on the handler's thread
    m_work_queue_ptr->add([this, msg]() { // Capture msg by copy
        if (m_sender_ready.load()) { // Re-check inside the work queue task
            try {
                m_sender.send(msg);
            } catch (const std::exception& e) {
                std::cerr << "[AMQP Send] Error during send in work_queue: " << e.what() << std::endl;
                // Consider setting m_sender_ready to false or handling error
            }
        }
    });
}

bool AMQP_recv::IsSenderReady() const {
    return m_sender_ready.load();
}

void AMQP_recv::on_sender_close(proton::sender &sender) {
    std::cout << "[AMQP recv] SENDER CLOSED" << std::endl;
    m_sender_ready.store(false);
    // Potentially signal stop if needed: if (m_stop_flag_ptr) m_stop_flag_ptr->store(true);
    messaging_handler::on_sender_close(sender); // Call base class if overriding
}

void AMQP_recv::on_sendable(proton::sender &sndr) {
    messaging_handler::on_sendable(sndr);
}

void AMQP_recv::on_message(proton::delivery &dlvr, proton::message &msg) {
    std::string body;
    try {
        body = proton::get<std::string>(msg.body());
    } catch (const proton::conversion_error& e) {
        return; 
    }

    // Schedule the processing on the main ns-3 simulation thread
    // using the global CollisionAvoidance application instance.
    if (ns3::g_collisionAvoiderApp) {
        // Use a copy of body for the scheduled event
        std::string body_copy = body;
        ns3::Simulator::ScheduleNow(&ns3::CollisionAvoidance::ReceiveString_send_to_SUMO, 
                                    ns3::g_collisionAvoiderApp, 
                                    body_copy);
    } 
}

void AMQP_recv::SetTraciClient(ns3::Ptr<ns3::TraciClient> client) {
    m_collisionHandler.SetClient(client);
}

AMQP_recv::AMQP_recv(const std::string &url,
                         const std::string &topic)
        {
    m_url = url;
    m_topic = topic;
}

void AMQP_recv::on_transport_error(proton::transport &transport) {
    std::cerr << "[AMQP recv] TRANSPORT ERROR: " << transport.error().what() << std::endl;
    m_sender_ready.store(false);
    messaging_handler::on_transport_error(transport);
}

void AMQP_recv::on_transport_close(proton::transport &transport) {
    std::cout << "[AMQP recv] TRANSPORT CLOSE" << std::endl;
    m_sender_ready.store(false);
    messaging_handler::on_transport_close(transport);
}

void AMQP_recv::on_connection_close(proton::connection &connection) {
    std::cout << "[AMQP recv] CONNECTION CLOSE" << std::endl;
    m_sender_ready.store(false);
    messaging_handler::on_connection_close(connection);
}

void AMQP_recv::on_connection_error(proton::connection &connection) {
    std::cerr << "[AMQP recv] CONNECTION ERROR: " << connection.error().what() << std::endl;
    m_sender_ready.store(false);
    messaging_handler::on_connection_error(connection);
}

AMQP_recv::AMQP_recv() {
    m_url = "amqp://admin:admin@10.10.7.254:5672";
    m_topic = "topic://sldm.to.van3t";
}