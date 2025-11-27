//
// Created by carlos on 05/02/24.
//

#ifndef ZMQ_PROXY_AMQP_RECV_H
#define ZMQ_PROXY_AMQP_RECV_H

#include <proton/messaging_handler.hpp>
#include <proton/container.hpp>
#include <proton/work_queue.hpp>
#include <atomic>
#include <mutex>
#include "collisionAvoidance.h"
#include "ns3/ptr.h"                // ← add this

class AMQP_recv : public proton::messaging_handler {
    std::string m_url;                           // URL of the AMQP broker
    std::string m_topic;                         // Topic
    std::string m_user;
    std::string m_password;
    proton::work_queue *m_work_queue_ptr = nullptr; // Initialize to nullptr
    proton::sender m_sender;
    proton::receiver m_receiver;
    std::atomic<bool> m_sender_ready{false}; // Initialize to false

    // SSL configuration
    std::string cert_file;  // Path to SSL certificate file
    std::string key_file;   // Path to SSL private key file
    std::string ca_file;    // Path to CA certificate file
    bool verify_peer;       // Whether to verify the peer's certificate



    // Qpid Proton event callbacks
    void on_container_start(proton::container& c) override;
    void on_connection_open(proton::connection& c) override;
    void on_sender_open(proton::sender& protonsender) override;
    void on_sendable (proton::sender& sndr) override;
    void on_message(proton::delivery &dlvr, proton::message &msg) override;
    void on_transport_error(proton::transport &transport) override;
    void on_transport_close(proton::transport &transport) override;
    void on_connection_close(proton::connection &connection) override;
    void on_connection_error(proton::connection &connection) override;
    void on_sender_close(proton::sender &sender) override;

public:
    // Empty constructor
    // You must call set_args just after the usage of an empty constructor, otherwise the behaviour may be undefined
    AMQP_recv();
    AMQP_recv(const std::string &url,
                const std::string &topic);

    /**
     * @brief Safely schedules a message to be sent via the AMQP sender.
     *
     * @param msg The proton::message to send.
     */
    void Send(const proton::message& msg);

    /**
     * @brief Check if the AMQP sender link is open and ready.
     *
     * @return true if the sender is ready, false otherwise.
     */
    bool IsSenderReady() const;

    /// Inject the SUMO/TraCI client into the collision‐handler.
    void SetTraciClient(ns3::Ptr<ns3::TraciClient> client); 

private:
    void set_no_local_filter(proton::source_options &opts);

    ns3::CollisionAvoidance m_collisionHandler;
};


#endif //ZMQ_PROXY_AMQP_RECV_H
