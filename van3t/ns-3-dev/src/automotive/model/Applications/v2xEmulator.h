#ifndef NS3_V2X_EMULATOR_H
#define NS3_V2X_EMULATOR_H

#include "ns3/application.h"
#include "ns3/asn_utils.h"
#include "ns3/denBasicService.h"
#include "ns3/caBasicService.h"
#include "ns3/cpBasicService.h"
#include "ns3/btp.h"
#include "ns3/sumo-sensor.h"
#include "ns3/LDM.h"
#include "ns3/traci-client.h"
#include "ns3/collisionAvoidance.h"   
#include "ns3/event-id.h"
#include "ns3/simulator.h"
#include "ns3/socket.h"
#include "ns3/ipv4-address.h"
#include "ns3/address.h"

namespace ns3 {

class v2xEmulator : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  v2xEmulator ();
  virtual ~v2xEmulator ();

  void SetCollisionAvoider (Ptr<CollisionAvoidance> ca);
  Ptr<CollisionAvoidance> GetCollisionAvoider() const;

  virtual void StartApplication (void) override;
  virtual void StopApplication (void) override;
  void StopApplicationNow ();

protected:
  virtual void DoDispose (void) override;

private:
  // Callback methods for received messages
  void receiveCAM (asn1cpp::Seq<CAM> cam, Address from);
  void receiveCPM (asn1cpp::Seq<CollectivePerceptionMessage> cpm, Address from);
  void receiveDENM (denData denm, Address from);

  // Existing private members and methods
  DENBasicService m_denService; //!< DEN Basic Service object
  CABasicService m_caService; //!< CA Basic Service object
  CPBasicService m_cpService; //!< CP Basic Service object

  Ptr<btp> m_btp; //! BTP object
  Ptr<GeoNet> m_geoNet; //! GeoNetworking Object

  Ptr<SUMOSensor> m_sensor;
  Ptr<LDM> m_LDM; //! LDM object

  Ptr<Socket> m_socket; //!< Client socket

  void TriggerDenm ();
  void UpdateDenm (DEN_ActionID actionid);

  vehicleData_t translateCPMdata(asn1cpp::Seq<CollectivePerceptionMessage> cpm,asn1cpp::Seq<PerceivedObject> object, int objectIndex);
  long compute_timestampIts ();

  Ptr<TraciClient> m_client; //!< TraCI client
  std::string m_id; //!< vehicle id
  bool m_send_cam; //!< To decide if CAM dissemination is active or not
  bool m_send_denm; //!< To decide if DENM dissemination is active or not
  bool m_send_cpm;  //!< To decide if CPM dissemination is active or not

  // UDP mode parameters
  Ipv4Address m_udpmode_ipAddress;
  int m_udpmode_port;
  bool m_udpmode_enabled;

  EventId m_sendDenmEvent; //!< Event to send the DENM

  Ptr<CollisionAvoidance> m_collisionAvoider; 

  EventId        m_caPollEvent;
  void           PollCollisionAvoider();
};

} // namespace ns3

#endif // NS3_V2X_EMULATOR_H

