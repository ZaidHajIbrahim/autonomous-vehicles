#include "collisionAvoidance.h"
#include "ns3/simulator.h"
#include "ns3/log.h"


#include "ns3/packet-socket-address.h"
#include "ns3/denData.h"
#include "ns3/CAM.h"
#include "ns3/DENM.h"
#include "ns3/vdpTraci.h"
#include "ns3/socket.h"
#include "ns3/asn_utils.h"
#include "ns3/type-id.h"
#include <sstream>
#include <iomanip> 
#include "traci-client-global.h"
#include "AMQPsend.h"


namespace ns3 {


TypeId
CollisionAvoidance::GetTypeId()
{
 static TypeId tid = TypeId("ns3::CollisionAvoidance")
   .SetParent<Application>()
   .SetGroupName("Applications")
   .AddConstructor<CollisionAvoidance>()
 ;
 return tid;
}


CollisionAvoidance::CollisionAvoidance()
: m_producer   (nullptr),
  m_sendInterval (Seconds (1.0)),
  m_lastMessageTime (Seconds (0.0))
{}


CollisionAvoidance::~CollisionAvoidance()
{
}


void
CollisionAvoidance::StartApplication () {
 if (m_client == nullptr){
     for (uint32_t i = 0; i < GetNode()->GetNApplications(); ++i) {
         Ptr<TraciClient> c = DynamicCast<TraciClient> (GetNode()->GetApplication(i));
         if (c){
             m_client = c;
             NS_LOG_INFO("Injected TraciClient in StartApplication: " << m_client);
             GlobalTraciClient() = m_client;
             break;
         }
     }
 }


 CollisionAvoidance::PollVehicles();
}


void
CollisionAvoidance::SetClient(Ptr<TraciClient> client)
{
   m_client = client;
   std::cout<<"[DBG] SetClient on "<<std::endl;
}


void CollisionAvoidance::SetAmqpProducer(std::shared_ptr<AMQPsend> p) {
   m_producer = p;
}


void
CollisionAvoidance::Send_Slowdown_Message_To_SUMO(const std::string& carId, double speedMps)
{
   auto client = GlobalTraciClient();
   if (!client)
   {
       std::cout << "No global TraCI client!" << std::endl;
       return;
   }
   client->vehicle.setMaxSpeed(carId, speedMps);
   SetVehicleColor(carId, 255, 255, 150, 20); // Orange, slow down vehicle
}


void
CollisionAvoidance::Send_Stop_Message_to_SUMO(const std::string& carId)
{
   auto client = GlobalTraciClient();
   if (!client) {
       std::cout << "No global TraCI client!" << std::endl;
       return;
   }
   client->vehicle.setMaxSpeed(carId, 0.01);
   SetVehicleColor(carId, 255, 255, 51, 204); // Pink, stop vehicle
}


void
CollisionAvoidance::Recover_Vehicle_Speed_to_SUMO(const std::string& carId, double speedMps)
{
   auto client = GlobalTraciClient();
   if (!client) {
       std::cout << "No global TraCI client!" << std::endl;
       return;
   }
   client->vehicle.setMaxSpeed(carId, speedMps);
   SetVehicleColor(carId, 255, 255, 255, 255); // White, recover vehicle
}


void
CollisionAvoidance::ReceiveString_send_to_SUMO(const std::string& message)
{
   NS_LOG_FUNCTION(this << message);

   std::stringstream ss(message);
   std::string collisionType;
   ss >> collisionType;

   std::string vehID_v, vehID_w_or_speed_str;
   double speed_ms = 0.0;

   if (collisionType == "recovered")
   {
       ss >> vehID_v >> speed_ms;
       if (vehID_v.rfind("veh", 0) != 0) vehID_v = "veh" + vehID_v;

       Recover_Vehicle_Speed_to_SUMO(vehID_v, speed_ms);
   }
   else if (collisionType == "rear-end" || collisionType == "cross-road")
   {
       ss >> vehID_v >> vehID_w_or_speed_str;
       if (vehID_v.rfind("veh", 0) != 0) vehID_v = "veh" + vehID_v;
       if (vehID_w_or_speed_str.rfind("veh", 0) != 0 && !vehID_w_or_speed_str.empty()) {
            vehID_w_or_speed_str = "veh" + vehID_w_or_speed_str;
       }

       if (collisionType == "rear-end")
       {
           Send_Slowdown_Message_To_SUMO(vehID_v, 3.0);
       }
       else // cross-road
       {
           Send_Stop_Message_to_SUMO(vehID_v);
       }
   }
   else
   {
       std::cout << "Unknown message type received: " << collisionType << std::endl;
   }

   NS_LOG_INFO("Processed collision message. Vehicles updated based on message.");
}


// Set the color of the vehicle in SUMOvoid
void CollisionAvoidance::SetVehicleColor(const std::string& carId, uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
   auto client = GlobalTraciClient();

   NS_LOG_FUNCTION(this << carId << (int)r << (int)g << (int)b << (int)a);

   // Initialize TraCIColor by assigning values to its members
   libsumo::TraCIColor color;
   color.a = static_cast<unsigned char>(a);
   color.r = static_cast<unsigned char>(r);
   color.g = static_cast<unsigned char>(g);
   color.b = static_cast<unsigned char>(b);

   try
   {
       client->vehicle.setColor(carId, color);
       std::cout << "[TEST] COLOR CHANGE" << std::endl;
   }
   catch (const std::exception &e)
   {
       std::cout << "[TEST] FAILED TO CHANGE COLOR" << std::endl;
   }
}

void
CollisionAvoidance::StopApplication ()
{
 Simulator::Cancel (m_pollEvent);
}


void
CollisionAvoidance::PollVehicles ()
{
 // Increment counter at the beginning of the function
 m_pollCounter++;

 auto client = GlobalTraciClient();


 if (!client) {
     NS_LOG_ERROR ("PollVehicles(): no TraCI client set");
     std::cout << "No TraCI client set" << std::endl;
     // Reschedule before returning to avoid stopping the polling
     m_pollEvent = Simulator::Schedule (Seconds (1.0),
                                        &CollisionAvoidance::PollVehicles, this);
     return;
 }


 // read synch-interval again
 TimeValue tv;
 client->GetAttribute ("SynchInterval", tv);
 Time syncInterval = tv.Get ();
 client->simulationStep (syncInterval.GetSeconds ());

 Time now = Simulator::Now ();

 std::vector<std::string> ids = client->vehicle.getIDList ();
 for (auto &vehicle_id : ids) {
     std::cout << "[PollVehicles] Processing Vehicle: " << vehicle_id << std::endl;

     auto pos3d = client->vehicle.getPosition3D (vehicle_id);
     double x = pos3d.x;  
     double y = pos3d.y;  
     int elevation = 0;

     auto lonlat = client->simulation.convertXYtoLonLat (x, y);
     double lon = lonlat.x;
     double lat = lonlat.y;

     double heading = client->vehicle.getAngle (vehicle_id);

     double speed_ms = client->vehicle.getSpeed (vehicle_id);
     double acceleration = client->vehicle.getAcceleration(vehicle_id); 

     double   gnTimestamp = now.GetSeconds ();
     uint64_t timestamp_us = now.GetMicroSeconds ();
  
     // Construct message in the format: stationID lat lon heading elevation speed_ms acceleration gnTimestamp timestamp_us
     std::string message = vehicle_id + " " +
         std::to_string(lat) + " " + 
         std::to_string(lon) + " " +
         std::to_string(heading) + " " +
         std::to_string(elevation) + " " +
         std::to_string(speed_ms) + " " +
         std::to_string(acceleration) + " " + 
         std::to_string(gnTimestamp) + " " +
         std::to_string(timestamp_us);


     if (m_producer) { // Check if producer is valid before using
         m_producer->sendMessage (message);
     }
 }

 // single, unified reschedule at the very end using the same interval
 m_pollEvent = Simulator::Schedule (Seconds (1.0),
                                    &CollisionAvoidance::PollVehicles, this);
}
} // namespace ns3
