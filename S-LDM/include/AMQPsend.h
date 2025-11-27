#pragma once

#include <proton/messaging_handler.hpp>
#include <proton/container.hpp>
#include <proton/message.hpp>
#include <proton/sender.hpp>
#include <proton/connection.hpp>
#include <proton/tracker.hpp>
#include <proton/transport.hpp>
#include <proton/error_condition.hpp>
#include <proton/link.hpp>
#include <proton/work_queue.hpp> 
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional> 
#include <condition_variable>
#include <deque>

class AMQPsend : public proton::messaging_handler {
public:
    AMQPsend(const std::string& brokerURI, const std::string& topicName);
    ~AMQPsend();
    void sendMessage(const std::string& message);

    void reconnect();


    int GetCredit() const;

private:
    std::string brokerURI;
    std::string topicName;
    proton::container container;
    proton::sender sender; 

    std::queue<proton::message> messageQueue;
    std::mutex queueMutex;
    std::condition_variable queueCv;   

    std::thread m_containerThread; 
    std::thread pollThread;        
    std::atomic<bool> running;
    std::atomic<int> m_credit; 

    proton::work_queue* m_work_queue; 

    std::deque<std::string> m_messageBuffer;

    // Proton event handlers
    void on_container_start(proton::container& c) override;
    void on_sender_open(proton::sender& s) override;

    void on_sendable(proton::sender& s) override;
    void on_tracker_accept(proton::tracker& t) override;
    void on_transport_close(proton::transport& t) override;
    void on_error(const proton::error_condition& e) override;
};
