#ifndef ZMQ_PROXY_AMQP_RECV_H
#define ZMQ_PROXY_AMQP_RECV_H

#include <proton/messaging_handler.hpp>
#include <proton/container.hpp>
#include <proton/work_queue.hpp>
#include <proton/source_options.hpp>
#include <proton/receiver_options.hpp> 
#include <proton/sender.hpp>
#include <proton/receiver.hpp>
#include <proton/transport.hpp> 
#include <atomic>
#include <mutex>
#include <string> 
#include <iostream>
#include <queue>
#include <condition_variable>
#include "CollisionAvoidance.h"

// Forward declaration for LDMMap 
namespace ldmmap {
    class LDMMap;
}

// Forward declaration for source_options
namespace proton {
    class source_options;
}


class AMQPrecv : public proton::messaging_handler {
private: // Changed order for initialization consistency
   // LDMMap reference first
   ldmmap::LDMMap& m_ldmmap;

   // CollisionAvoidance instance
   CollisionAvoidance m_ca;

   // Connection details 
   std::string m_url;
   std::string m_topic;
   std::string m_user;
   std::string m_password;

   // Proton objects
   proton::work_queue *m_work_queue_ptr = nullptr;
   proton::sender m_sender;
   proton::receiver m_receiver;
   std::atomic<bool> m_sender_ready{false};

    // SSL configuration
    std::string cert_file;  // Path to SSL certificate file
    std::string key_file;   // Path to SSL private key file
    std::string ca_file;    // Path to CA certificate file
    bool verify_peer;       // Whether to verify the peer's certificate

   // <<< Add this declaration
   void set_no_local_filter(proton::source_options &opts);


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
    void on_receiver_open(proton::receiver &r) override;
    void on_receiver_close(proton::receiver &r) override;
    void on_receiver_error(proton::receiver &r) override;
    void on_receiver_drain_finish(proton::receiver &r) override;

public:

   //  Constructors 
   AMQPrecv(ldmmap::LDMMap& ldmmap);
   AMQPrecv(ldmmap::LDMMap& ldmmap, const std::string &url, const std::string &topic);
   // AMQPrecv(const std::string &url, const std::string &topic, ldmmap::LDMMap& ldmmap); // Consider removing if redundant

   // Destructor 
   ~AMQPrecv();

   // Public methods 
   void run();
   void stop();
   void set_work_queue(proton::work_queue* wq);
};

#endif //ZMQ_PROXY_AMQP_RECV_H
