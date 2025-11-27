/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation;
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


* Created by:
*  Marco Malinverno, Politecnico di Torino (marco.malinverno1@gmail.com)
*  Francesco Raviglione, Politecnico di Torino (francescorav.es483@gmail.com)
*  Carlos Mateo Risma Carletti, Politecnico di Torino (carlosrisma@gmail.com)
*/
#include "ns3/carla-module.h"
#include "ns3/OpenCDAClient.h"
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/fd-net-device-module.h"
#include "ns3/sumo_xml_parser.h"
#include "ns3/mobility-module.h"
#include "ns3/traci-module.h"
#include "ns3/v2xEmulator-helper.h"
#include "ns3/emu-fd-net-device-helper.h"
#include "ns3/v2xEmulator.h"
#include "ns3/packet-socket-helper.h"
#include "amqp_recv.h"
#include "AMQPsend.h"
#include <thread>
#include "traci-client-global.h"
#include "ns3/collisionAvoidance.h"
#include "ns3/traci-client.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("v2x-emulator");

namespace ns3 {
   Ptr<TraciClient> g_traci_client;
}


// Flag to signal the AMQP thread to stop
std::atomic<bool> g_stopAmqpThread(false);


// --- Forward Declarations ---
void HandleAmqpMessage(std::string message);
void AmqpClientThreadFunc(AMQP_recv& recv);


// --- AMQP Message Handler ---
void HandleAmqpMessage(std::string message)
{
   NS_LOG_INFO(Simulator::Now().GetSeconds() << "s - NS-3 Handling AMQP Message: " << message);
   if (g_collisionAvoiderApp) {
       NS_LOG_INFO("Forwarding message to CollisionAvoidance instance.");
       g_collisionAvoiderApp->ReceiveString_send_to_SUMO(message);
   } else {
       NS_LOG_WARN("AMQP message received, but CollisionAvoidance app is not (yet) available.");
   }
}


void AmqpClientThreadFunc(AMQP_recv& recv)
{
   proton::container container(recv);
   try
   {
       container.run();
   }
   catch (const std::exception &e)
   {
       std::cerr << "Error in AMQP recv thread: " << e.what() << std::endl;
   }
}


int
main (int argc, char *argv[])
{
 std::cout << "[v2x-emulator] main start on thread "
           << std::this_thread::get_id() << "\n";


 AMQP_recv amqp_recv;


 std::thread amqpRecvThread([&](){
   auto id = std::this_thread::get_id();
   std::cout << "[AMQPRecvThread] ["<< id <<"] start\n";
   proton::container recvContainer(amqp_recv);
   recvContainer.run();
   std::cout << "[AMQPRecvThread] ["<< id <<"] exit\n";
 });
 std::cout << "[v2x-emulator] amqpRecvThread id="
           << amqpRecvThread.get_id() << "\n";


 //Initializing a new AMQPsend object
 std::string brokerURI = "10.10.7.254:5672";
 std::string topicName = "topic://van3t.to.sldm";
 auto amqpProducer = std::make_shared<AMQPsend>(brokerURI, topicName);

  // Physical interface parameters
 std::string deviceName ("eth1");
 std::string encapMode ("Dix");


 // UDP mode parameters
 std::string udpIp = "";
 std::string gwstr = "192.168.1.1";
 std::string subnet = "192.168.1.0";
 std::string netmask = "255.255.255.0";
 int destPort=0;
 Ipv4Address destAddr;
 Ipv4AddressHelper ipv4helper;


//  Simulation parameters
 std::string sumo_folder = "src/automotive/examples/sumo_files_test_map/";
 std::string mob_trace = "cars.rou.xml";
 std::string sumo_config = "src/automotive/examples/sumo_files_test_map/new_map.sumo.cfg";

// For small roundabout
//  std::string sumo_folder = "src/automotive/examples/Liten_korsning/"; 
// //  std::string mob_trace =   "osm.passenger.trips.xml";
//  std::string mob_trace =   "test_vehicles.rou.xml";
//  std::string sumo_config = "src/automotive/examples/Liten_korsning/liten_korsning_map.sumocfg";


 std::string sumo_netns = "";

 bool sendCam = true;
 bool sendDenm = true;
 bool sendCpm = true;


 bool verbose = true;
 bool sumo_gui = true;
 double sumo_updates = 0.01;
 bool print_summary = false;


 double emuTime = 100;


 int numberOfNodes = 0;
 uint32_t nodeCounter = 0;


 double testCollisionTime = 0.0;


 // --- Command Line Parsing ---
 CommandLine cmd (__FILE__);
 xmlDocPtr rou_xml_file;


 /* Cmd Line option for vehicular application */
 cmd.AddValue ("sumo-gui", "Use SUMO gui or not", sumo_gui);
 cmd.AddValue ("sumo-updates", "SUMO granularity", sumo_updates);
 cmd.AddValue ("sumo-folder","Position of sumo config files",sumo_folder);
 cmd.AddValue ("mob-trace", "Name of the mobility trace file", mob_trace);
 cmd.AddValue ("sumo-config", "Location and name of SUMO configuration file", sumo_config);
 cmd.AddValue ("sumo-netns","[Advanced users] Name of the network namespace SUMO shall be launched in. If not specified, SUMO is launched normally.",sumo_netns);
 cmd.AddValue ("summary", "Print a summary for each vehicle at the end of the simulation", print_summary);
 cmd.AddValue ("verbose", "Enable verbose printing on stdout", verbose);
 cmd.AddValue ("interface", "Name of the physical interface to send V2X messages to", deviceName);
 cmd.AddValue ("sim-time", "Total duration of the emulation [s]", emuTime);
 cmd.AddValue ("send-cam", "To trigger the CAM dissemination", sendCam);
 cmd.AddValue ("send-cpm", "To trigger the CPM dissemination", sendCpm);
 cmd.AddValue ("send-denm", "To trigger the DENM dissemination", sendDenm);
 cmd.AddValue ("udp", "[UDP mode] To enable UDP mode and specify UDP port and IP address where the V2X messages are redirected (format: <IP>:<port>)", udpIp);
 cmd.AddValue ("gateway", "[UDP mode] To specify the gateway at which the UDP/IP packets will be sent", gwstr);
 cmd.AddValue ("subnet", "[UDP mode] To specify the subnet which will be used to assign the IP addresses of emulated nodes (the .1 address is automatically excluded)", subnet);
 cmd.AddValue ("netmask", "[UDP mode] To specify the netmask of the network", netmask);
 cmd.AddValue ("testCollisionTime", "Time (seconds) to simulate sending a collision warning AMQP message", testCollisionTime);


 cmd.Parse (argc, argv);
 NS_LOG_INFO("Parsed testCollisionTime = " << testCollisionTime); // <-- Log parsed value


 /* If verbose is true, enable some additional logging */
 if (verbose)
   {
     LogComponentEnable ("v2x-emulator", LOG_LEVEL_INFO);
     LogComponentEnable ("CABasicService", LOG_LEVEL_INFO);
     LogComponentEnable ("DENBasicService", LOG_LEVEL_INFO);
   }


 /* Using the real-time scheduler is mandatory when emulating vehicles */
 /* WARNING: you must be sure that the computer is capable enough to simulate
  * the desired number of vehicles, otherwise the simulation will slow down
  * and the real-time constraint will not be respected anymore (causing, for
  * instance, the vehicles to look as if they were moving more slowly and
  * sending CAMs with a lower frequency) */
 GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));


 GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));


 /* Read the SUMO mobility information and map from the .rou XML file */
 NS_LOG_INFO("Reading the .rou file...");
 std::string path = sumo_folder + mob_trace;


 /* Load the .rou.xml document */
 xmlInitParser();
 rou_xml_file = xmlParseFile(path.c_str ());
 if (rou_xml_file == NULL)
   {
     NS_FATAL_ERROR("Error: unable to parse the specified XML file: "<<path);
   }
 numberOfNodes = XML_rou_count_vehicles(rou_xml_file);


 xmlFreeDoc(rou_xml_file);
 xmlCleanupParser();


 if(numberOfNodes==0)
   {
     NS_FATAL_ERROR("Fatal error: cannot gather the number of vehicles from the specified XML file: "<<path<<". Please check if it is a correct SUMO file.");
   }
 NS_LOG_INFO("The .rou file has been read: " << numberOfNodes << " vehicles will be present in the simulation.");


 // When --udp is specified, parse the IPv4 and port parameters
 if (udpIp != "")
 {
   std::istringstream udpIpStream(udpIp);
   std::string curr_str;
   // IP address
   std::getline(udpIpStream, curr_str, ':');


   destAddr = Ipv4Address(curr_str.c_str());


   // Port
   std::getline(udpIpStream, curr_str, ':');
   destPort=std::stoi(curr_str);


   if(destPort<=0 || destPort > 65535)
     {
       NS_FATAL_ERROR("Error: "<<destPort<<" is not a valid port for UDP operations.");
     }


   std::cout<< "V2X Messages will be sent to:"<<destAddr<<":"<<destPort << std::endl;
 }


 /* Set the emulation total time (in seconds) */
 NS_LOG_INFO("Simulation will last " << emuTime << " seconds");
 ns3::Time simulationTime (ns3::Seconds(emuTime));


 /* Create containers for OBUs */
 NodeContainer obuNodes;
 obuNodes.Create(numberOfNodes);


 if (udpIp != "")
   {
     InternetStackHelper internet;
     internet.SetIpv4StackInstall (true);
     internet.Install (obuNodes);
     ipv4helper.SetBase (subnet.c_str(), netmask.c_str());
     ipv4helper.NewAddress (); // "Burn" the first IP as it may be assigned to a router in the local network
   }


 /* Setup Mobility and position node pool */
 MobilityHelper mobility;
 mobility.Install (obuNodes);


 /* Setup Traci and start SUMO */
 ns3::g_traci_client = CreateObject<TraciClient>();
 g_traci_client->SetAttribute ("SumoConfigPath", StringValue (sumo_config));
 g_traci_client->SetAttribute ("SumoBinaryPath", StringValue (""));    // use system installation of sumo
 g_traci_client->SetAttribute ("SynchInterval", TimeValue (Seconds (sumo_updates)));
 g_traci_client->SetAttribute ("StartTime", TimeValue (Seconds (0.0)));
 g_traci_client->SetAttribute ("SumoGUI", (BooleanValue) sumo_gui);
 g_traci_client->SetAttribute ("SumoPort", UintegerValue (3400));
 g_traci_client->SetAttribute ("PenetrationRate", DoubleValue (1.0));
 g_traci_client->SetAttribute ("SumoLogFile", BooleanValue (false));
 g_traci_client->SetAttribute ("SumoStepLog", BooleanValue (false));
 g_traci_client->SetAttribute ("SumoSeed", IntegerValue (10));
 g_traci_client->SetAttribute ("SumoAdditionalCmdOptions", StringValue ("--verbose true"));
 g_traci_client->SetAttribute ("SumoWaitForSocket", TimeValue (Seconds (1.0))); //chek if this is needed maybe take away
 g_traci_client->SetAttribute ("SumoAdditionalCmdOptions", StringValue ("--collision.action warn --collision.check-junctions --error-log=sumo-errors-or-collisions.xml"));
 g_traci_client->SetAttribute ("UseNetworkNamespace", StringValue(sumo_netns));


 // publish it to everyone
 ns3::GlobalTraciClient() = g_traci_client;


 // --- Create and Configure CollisionAvoidance ---
 ns3::g_collisionAvoiderApp = CreateObject<CollisionAvoidance>();
 g_collisionAvoiderApp->SetAmqpProducer(amqpProducer);


 NS_LOG_INFO("CollisionAvoidance instance created and linked to TraCI client.");
 {
   //install the one CollisionAvoidance on node 0
   obuNodes.Get (0)->AddApplication (g_collisionAvoiderApp);
   g_collisionAvoiderApp->SetStartTime (Seconds (0.0));
   g_collisionAvoiderApp->SetStopTime  (simulationTime - Seconds (0.1));
  


 // Ptr<CollisionAvoidance> ca = CreateObject<CollisionAvoidance>();
 // ca->SetClient(sumoClient);
 // NodeContainer all;
 // all.Add(obuNodes);
 // all.Get(0)->AddApplication(ca);
 // ca->SetStartTime(Seconds(0.0));
 // ca->SetStopTime(simulationTime - Seconds(0.1));


 // --- Create and Configure AMQP_recv ---
 auto amqp = std::make_shared<AMQP_recv>(brokerURI, topicName);
 amqp->SetTraciClient(g_traci_client);


 // // Set global pointer to CollisionAvoidance app
 // g_collisionAvoiderApp = ca;


 /* Create the OBU application (v2xEmulator, see also model/v2xEmulator.cc/.h) */
 v2xEmulatorHelper emuHelper;
 emuHelper.SetAttribute ("Client", (PointerValue) g_traci_client); // pass TraciClient object for accessing sumo in application
 emuHelper.SetAttribute ("SendCAM", (BooleanValue) sendCam);
 emuHelper.SetAttribute ("SendDENM", (BooleanValue) sendDenm);
 emuHelper.SetAttribute ("SendCPM", (BooleanValue) sendCpm);
 if(udpIp!="")
 {
   emuHelper.SetAttribute ("DestinationIPv4", Ipv4AddressValue(destAddr));
   emuHelper.SetAttribute ("DestinationPort", IntegerValue(destPort));
   emuHelper.SetAttribute ("UDPmode",(BooleanValue) true);
 }


 /* Create the FdNetDevice to send packets over a physical interface */
 EmuFdNetDeviceHelper emuDev;
 emuDev.SetDeviceName (deviceName);
 emuDev.SetAttribute ("EncapsulationMode", StringValue (encapMode));


 /* Give packet socket powers to nodes (otherwise, if the app tries to create a PacketSocket, CreateSocket will end up with a segmentation fault */
 if (udpIp=="")
 {
   PacketSocketHelper packetSocket;
   packetSocket.Install (obuNodes);
 }


 NetDeviceContainer fdnetContainer;
 Ipv4InterfaceContainer ipv4ic;
 Ipv4Address gateway (gwstr.c_str());
 Ipv4StaticRoutingHelper ipv4RoutingHelper;


 /* Callback function for node creation */
 STARTUP_FCN setupNewEmuNode = [&] (std::string vehicleID,TraciClient::StationTypeTraCI_t stationType) -> Ptr<Node>
   {
     auto id = std::this_thread::get_id();
     std::cout << "[setupNewEmuNode] thread="<< id
               << " vehicleID="<< vehicleID << "\n";


     if (nodeCounter >= obuNodes.GetN())
       NS_FATAL_ERROR("Node Pool empty!: " << nodeCounter << " nodes created.");


     std::cout<<"Creating node: "<<nodeCounter<<std::endl;


     /* don't create and install the protocol stack of the node at simulation time -> take from "node pool" */
     Ptr<Node> includedNode = obuNodes.Get(nodeCounter);
     ++nodeCounter; //increment counter for next node


     /* Install FdNetDevice and set MAC address (using 00:00:00:00:00:01, 00:00:00:00:00:02, and so on, for each vehicle) */
     std::ostringstream veh_mac;
     veh_mac << "00:00:00:00:00:" << std::setfill('0') << std::setw(2) << nodeCounter;
     fdnetContainer = emuDev.Install(includedNode);
     Ptr<FdNetDevice> dev = fdnetContainer.Get (0)->GetObject<FdNetDevice> ();
     dev->SetAddress (Mac48Address (veh_mac.str().c_str ()));


     std::cout<<"MAC of node "<<nodeCounter-1<<": "<<veh_mac.str()<<std::endl;


     /* When in UDP mode, configure the IP stack of the nodes/vehicles */
     if(udpIp!="")
     {
        Ptr<Ipv4> ipv4 = includedNode->GetObject<Ipv4> ();
        uint32_t interface = ipv4->AddInterface (dev);
        ipv4ic = ipv4helper.Assign (fdnetContainer);
        Ipv4InterfaceAddress address = Ipv4InterfaceAddress (ipv4ic.GetAddress (0,0), netmask.c_str());
        ipv4->AddAddress (interface, address);
        ipv4->SetMetric (interface, 1);
        ipv4->SetUp (interface);


        Ptr<Ipv4StaticRouting> staticRouting = ipv4RoutingHelper.GetStaticRouting (ipv4);
        staticRouting->SetDefaultRoute (gateway, interface);
        std::cout<<"IPv4 of node "<<nodeCounter-1<<": "<<ipv4ic.GetAddress (0,0)<<std::endl;
     }


     /* Install Application */
     ApplicationContainer v2xEmulatorApp = emuHelper.Install(includedNode);
     Ptr<v2xEmulator> v2xApp = v2xEmulatorApp.Get(0)->GetObject<v2xEmulator>();



     v2xApp->SetCollisionAvoider (g_collisionAvoiderApp);


     v2xEmulatorApp.Start (Seconds (0.0));
     v2xEmulatorApp.Stop (simulationTime - Simulator::Now () - Seconds (0.1));


     std::cout<<"New node: "<<nodeCounter-1<<std::endl;


     std::cout << "[setupNewEmuNode] created Node #"<< nodeCounter-1
               << " on thread="<< id << "\n";


     return includedNode;
   };


 /* Callback function for node shutdown */
 SHUTDOWN_FCN shutdownEmuNode = [] (Ptr<Node> exNode,std::string vehicleID)
   {
     auto id = std::this_thread::get_id();
     std::cout << "[shutdownEmuNode] thread="<< id
               << " vehicleID="<< vehicleID << "\n";


     /* stop all applications */
     Ptr<v2xEmulator> v2xEmulatorApp_ = exNode->GetApplication(0)->GetObject<v2xEmulator>();
     if(v2xEmulatorApp_)
       v2xEmulatorApp_->StopApplicationNow ();


      /* set position outside communication range */
     Ptr<ConstantPositionMobilityModel> mob = exNode->GetObject<ConstantPositionMobilityModel>();
     mob->SetPosition(Vector(-1000.0+(rand()%25),320.0+(rand()%25),250.0));// rand() for visualization purposes


     /* NOTE: further actions could be required for a safe shut down! */


     std::cout << "[shutdownEmuNode] done on thread="<< id << "\n";
   };


 std::cout << "[v2x-emulator] calling sumoClient->SumoSetup() on thread "
           << std::this_thread::get_id() << "\n";
 g_traci_client->SumoSetup (setupNewEmuNode, shutdownEmuNode);
 std::cout << "[v2x-emulator] Sumocli setup done on thread "
           << std::this_thread::get_id() << "\n";

  
 // --- Schedule Test Collision Message ---
 if (testCollisionTime > 0.0) {
     std::string testMessage = "cross-road veh0 veh1"; // The message CollisionAvoidance expects
     NS_LOG_INFO("*** Scheduling TEST message '" << testMessage << "' at " << testCollisionTime << "s ***"); // <-- Log before scheduling
     Simulator::Schedule(Seconds(testCollisionTime), &HandleAmqpMessage, testMessage);
 } else {
     NS_LOG_INFO("No test collision message scheduled (testCollisionTime <= 0.0). Relying on external AMQP.");
 }


 std::cout << "[v2x-emulator] calling Simulator::Run() on thread "
           << std::this_thread::get_id() << "\n";
 Simulator::Run ();
 std::cout << "[v2x-emulator] Simulator::Run() returned at t="
           << Simulator::Now().GetSeconds() << "\n";


 Simulator::Destroy ();
 std::cout << "[v2x-emulator] Simulator destroyed on thread "
           << std::this_thread::get_id() << "\n";


 std::cout << "[v2x-emulator] joining amqpRecvThread\n";
 amqpRecvThread.join();
 std::cout << "[v2x-emulator] amqpRecvThread joined, exiting main\n";


 return 0;
}
}
