#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H

#include <memory>
#include "AMQPsend.h"     
#include "ns3/application.h"
#include "ns3/packet-socket-address.h"
#include "ns3/denData.h"
#include "ns3/log.h"
#include "ns3/CAM.h"
#include "ns3/DENM.h"
#include "ns3/vdpTraci.h"
#include "ns3/socket.h"
#include "ns3/asn_utils.h"
#include "ns3/type-id.h"
#include "ns3/event-id.h"
#include "ns3/simulator.h"
#include <string>
#include <sstream>
#include <deque>



namespace ns3 {

    class TraciClient; 

class CollisionAvoidance : public Application {
    
public:
  CollisionAvoidance();
  virtual ~CollisionAvoidance();// override; // Destructor declared virtual with override
  static TypeId GetTypeId();
  void SetClient(Ptr<TraciClient> client);
  void ReceiveString_send_to_SUMO(const std::string& message);

  void Send_Slowdown_Message_To_SUMO(const std::string& carId, double speedMps);
  void Send_Stop_Message_to_SUMO(const std::string& carId);
  void Recover_Vehicle_Speed_to_SUMO(const std::string& carId, double speedMps);

  void SetVehicleColor(const std::string& carId, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

  virtual void StartApplication() override;
  virtual void StopApplication() override;

  void SetAmqpProducer(std::shared_ptr<AMQPsend> p);

private:

  EventId m_pollEvent;
  EventId m_sendEvent;
  std::shared_ptr<AMQPsend> m_producer;
  Time m_sendInterval;
  Time m_lastMessageTime;

  Ptr<TraciClient> m_client;

  std::deque<std::string> m_messageBuffer;

  uint64_t m_pollCounter = 0; 

  void PollVehicles ();
  void SendMessages ();
};

} // namespace ns3s

#endif // COLLISION_AVOIDANCE_H