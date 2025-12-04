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
  std::cout << "[AMQPsend] ctor on thread " << std::this_thread::get_id() << "\n";

  // container thread
  m_containerThread = std::thread([this]() {
    auto id = std::this_thread::get_id();
    // std::cout << "[ContainerThread] [" << id << "] start\n";
    try {
        container.run();
    } catch (const std::exception& e) {
        // std::cerr << "[ContainerThread] [" << id << "] Exception: " << e.what() << std::endl;
        running = false; // Signal other threads to stop
        queueCv.notify_all();
    }
    // std::cout << "[ContainerThread] [" << id << "] exit\n";
  });
  // std::cout << "[AMQPsend] containerThread launched id="
  //           << m_containerThread.get_id() << "\n";


  // poll thread
  pollThread = std::thread([this]() {
    auto id = std::this_thread::get_id();
    std::cout << "[PollThread] [" << id << "] start\n";
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
        try { std::cout << "[PollThread] scheduling send of '"
                        << proton::get<std::string>(msg.body())
                        << "'\n";
        } catch(...) {}

        // Capture necessary data by value or move
        // std::cout << "[PollThread] [" << id << "] Scheduling send via work_queue. Queue size after pop: "
        //           << messageQueue.size() << "\n";

        // Unlock mutex BEFORE adding work to avoid holding lock during potential block
        lk.unlock();

        bool scheduled = m_work_queue->add([this, moved_msg = std::move(msg)]() mutable {
            // This lambda runs on the container thread
            auto container_thread_id = std::this_thread::get_id();
            if (sender.credit() > 0) {
                 try {
                    std::string body = "<unreadable>";
                    try { body = proton::get<std::string>(moved_msg.body()); }
                    catch(...) {}
                    std::cout << "[WorkQueueLambda] actually calling sender.send(): '"
                              << body << "'\n";

                    std::string msg_body_str = "[body unavailable]";
                    try { msg_body_str = proton::get<std::string>(moved_msg.body()); } catch(...) {}

                    // std::cout << "[WorkQueueLambda] [" << container_thread_id << "] Executing send. Msg: '" << msg_body_str
                              // << "', Credit: " << sender.credit() << "\n";
                    proton::tracker trk = sender.send(moved_msg); // Send the moved message
                    // std::cout << "[WorkQueueLambda] [" << container_thread_id << "] send() called. Tracker involved.\n";
                 } catch (const proton::error& e) {
                    //  std::cerr << "[WorkQueueLambda] [" << container_thread_id << "] Exception during send: " << e.what() << std::endl;
                 }
            } else {
                // std::cout << "[WorkQueueLambda] [" << container_thread_id << "] Skipped send due to 0 credit.\n";
            }
        });

        if (!scheduled) {
            //  std::cerr << "[PollThread] [" << id << "] Failed to schedule send via work_queue (queue likely closed).\n";
        }
        lk.lock();

      } else {
          // Log why we didn't schedule a send
          if (messageQueue.empty()) {
          } else if (!m_work_queue) {
              std::cout << "[PollThread] [" << id << "] woke but work_queue not ready.\n";
          } else { // No credit
              std::cout << "[PollThread] [" << id
                        << "] woke but NO CREDIT. Queue size: " << messageQueue.size()
                        << ", Credit: " << sender.credit() << "\n";
          }
      }
    }
    std::cout << "[PollThread] [" << id << "] exit\n";
  });
  std::cout << "[AMQPsend] pollThread launched id="
            << pollThread.get_id() << "\n";
}

AMQPsend::~AMQPsend() {
  std::cout << "[AMQPsend] dtor on thread " << std::this_thread::get_id() << "\n";
  running = false;
  queueCv.notify_all();

  std::cout << "[AMQPsend] joining pollThread id=" << pollThread.get_id() << "\n";
  if (pollThread.joinable()) pollThread.join();
  std::cout << "[AMQPsend] pollThread joined\n";

  container.stop();

  std::cout << "[AMQPsend] joining containerThread id=" << m_containerThread.get_id() << "\n";
  if (m_containerThread.joinable()) m_containerThread.join();
  std::cout << "[AMQPsend] containerThread joined\n";
}

void AMQPsend::sendMessage(const std::string& message) 
{
    std::cout << "[AMQPsend] sendMessage enqueuing → '" << message << "'\n";
    { 
      std::lock_guard<std::mutex> lk(queueMutex);
        // std::cout << "[DBG AMQPsend::sendMessage] Enqueue: " << message << "\n";
        messageQueue.emplace(proton::message{message});
        // std::cout << "[DBG AMQPsend::sendMessage] Notifying poll thread\n";
    }
    std::cout << "[AMQPsend] sendMessage notified pollThread\n";
    queueCv.notify_one();
    // std::cout << "[DBG AMQPsend::sendMessage] ← Exit sendMessage\n";
}

void AMQPsend::on_container_start(proton::container& c) {
    // std::cout << "[DBG AMQPsend::on_container_start] → Entry\n";
    // std::cout << "[AMQPsend::on_container_start] on thread "
              // << std::this_thread::get_id() << "\n";
  //  std::cout << "[AMQPsend::on_container_start] Opening sender with credentials in URL..." << std::endl;
   proton::connection_options opts; // Still create opts for potential future use (e.g., idle_timeout)

   std::string url_with_creds = "amqp://admin:admin@" + brokerURI + "/" + topicName;

   try {
       // Open sender directly, connection happens implicitly
       sender = c.open_sender(url_with_creds, opts); // Pass options if needed
       std::cout << "[AMQPsend] on_container_start → opened sender to '"
                 << url_with_creds << "'\n";
      //  std::cout << "[AMQPsend::on_container_start] Sender opening initiated to: " << url_with_creds << std::endl;
      //  std::cout << "[DBG AMQPsend::on_container_start] ← done\n";
   } catch (const proton::error& e) {
        // std::cerr << "[AMQPsend::on_container_start] Failed to initiate sender opening: " << e.what() << std::endl;
   }
    // std::cout << "[DBG AMQPsend::on_container_start] ← Exit\n";
}

void AMQPsend::on_sender_open(proton::sender& s) {
    std::cout << "[AMQPsend] on_sender_open → link opened, credit=" << s.credit() << "\n";
    std::cout << "[DBG AMQPsend::on_sender_open] → Entry\n";
    std::cout << "[AMQPsend::on_sender_open] Sender link opened to "
              << s.target().address()
              << " — initial credit=" << s.credit()
              << "\n";

    m_work_queue = &s.work_queue(); // Get the work queue associated with this sender
    // std::cout << "[AMQPsend::on_sender_open] Work queue pointer obtained.\n";

    // Notify pollThread that the sender (and work queue) is ready and might have credit
    {
        std::lock_guard<std::mutex> lk(queueMutex); // Lock needed for safe check
        if (!messageQueue.empty() && s.credit() > 0) {
            //  std::cout << "[DBG AMQPsend::on_sender_open] Notifying pollThread (sender ready, queue not empty, credit > 0)\n";
             queueCv.notify_one();
        } else {
            //  std::cout << "[DBG AMQPsend::on_sender_open] Sender open, but queue empty or no credit. No immediate notification needed.\n";
        }
    }
    // std::cout << "[DBG AMQPsend::on_sender_open] ← Exit (Notified pollThread if necessary)\n";
}

void AMQPsend::on_sendable(proton::sender& s) {
    // std::cout << "[DBG AMQPsend::on_sendable] → on_sendable, credit=" << s.credit() << "\n";
    {
        std::lock_guard<std::mutex> lk(queueMutex);
        std::cout << "[AMQPsend] on_sendable → credit=" << s.credit()
                  << ", queueSize=" << messageQueue.size() << "\n";
        if (!messageQueue.empty()) {
            // std::cout << "[DBG AMQPsend::on_sendable] Notifying pollThread (credit available, queue not empty)\n";
            queueCv.notify_one();
        } else {
            //  std::cout << "[DBG AMQPsend::on_sendable] Credit available, but queue empty. No notification needed.\n";
        }
    }
    std::cout << "[AMQPsend] on_sendable ← done\n";
    // std::cout << "[DBG AMQPsend::on_sendable] ← Exit\n";
}

void AMQPsend::on_tracker_accept(proton::tracker& t) {
  std::cout << "[AMQPsend::on_tracker_accept] Broker ACCEPTED message associated with tracker."
            << " on thread " << std::this_thread::get_id() << std::endl;
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

