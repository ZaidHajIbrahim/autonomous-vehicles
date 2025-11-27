#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/traci-module.h"
#include "ns3/roadWeightUpdater.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleSumoLauncher");

int main(int argc, char *argv[])
{
  double sumo_updates = 0.01;

  // Command-line arguments
  std::string sumo_config = "src/automotive/examples/sumo_files_v2i_map/map.sumo.cfg";
  bool sumo_gui = true;

  CommandLine cmd;
  cmd.AddValue("sumo-config", "Path to SUMO configuration file", sumo_config);
  cmd.AddValue("sumo-gui", "Enable SUMO GUI", sumo_gui);
  cmd.Parse(argc, argv);

  // Set up TraCI client
  Ptr<TraciClient> sumoClient = CreateObject<TraciClient>();
  sumoClient->SetAttribute ("SumoConfigPath", StringValue (sumo_config));
  sumoClient->SetAttribute ("SumoBinaryPath", StringValue (""));    // use system installation of sumo
  sumoClient->SetAttribute ("SynchInterval", TimeValue (Seconds (sumo_updates)));
  sumoClient->SetAttribute ("StartTime", TimeValue (Seconds (0.0)));
  sumoClient->SetAttribute ("SumoGUI", (BooleanValue) sumo_gui);
  sumoClient->SetAttribute ("SumoPort", UintegerValue (3400));
  sumoClient->SetAttribute ("PenetrationRate", DoubleValue (1.0));
  sumoClient->SetAttribute ("SumoLogFile", BooleanValue (false));
  sumoClient->SetAttribute ("SumoStepLog", BooleanValue (false));
  sumoClient->SetAttribute ("SumoSeed", IntegerValue (10));

// Define a  node in the startup function
/*
auto StartupFcn = [&sumoClient] (std::string vehicleID, TraciClient::StationTypeTraCI_t stationType) -> Ptr<Node> {
  std::cout << "Startup function called for vehicleID: " << vehicleID << std::endl;

  // Create a node
  Ptr<Node> node = CreateObject<Node>();
  std::cout << "Node created with ID: " << node->GetId() << std::endl;

  // Add a mobility model to the node
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<ConstantPositionMobilityModel>();
  node->AggregateObject(mobility);
  std::cout << "Mobility model added to node." << std::endl;

  // Define a function to continuously print the road ID
  auto printRoadID = std::make_shared<std::function<void()>>();
  *printRoadID = [vehicleID, sumoClient, printRoadID]() {
      auto route = sumoClient->TraCIAPI::vehicle.getRoute(vehicleID);
      auto roadID = sumoClient->TraCIAPI::vehicle.getRoadID(vehicleID);
      
     // auto new_route = sumoClient->TraCIAPI::simulation.findRoute(roadID, route.back());
     // sumoClient->TraCIAPI::vehicle.setRoute(vehicleID, new_route.edges);
      std::cout << "Road ID for vehicle " << vehicleID << ": " << roadID << " at time " << Simulator::Now().GetSeconds() << "s" << std::endl;

      // Schedule the next execution of this function
      Simulator::Schedule(Seconds(1.0), *printRoadID);
  };

  // Schedule the first execution of the printRoadID function
  Simulator::Schedule(Seconds(1.0), *printRoadID);

  return node; // Return the node
};
*/

auto StartupFcn = [&sumoClient] (std::string vehicleID, TraciClient::StationTypeTraCI_t stationType) -> Ptr<Node> {
  std::cout << "Startup function called for vehicleID: " << vehicleID << std::endl;

  // Create a node
  Ptr<Node> node = CreateObject<Node>();
  std::cout << "Node created with ID: " << node->GetId() << std::endl;

  // Add a mobility model to the node
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<ConstantPositionMobilityModel>();
  node->AggregateObject(mobility);
  std::cout << "Mobility model added to node." << std::endl;

  // Enable rerouting for the vehicle
  sumoClient->TraCIAPI::vehicle.setParameter(vehicleID, "has.rerouting.device", "true");
  sumoClient->TraCIAPI::vehicle.setParameter(vehicleID, "device.rerouting.period", "5.0"); // Reroute every 5 seconds
  std::cout << "Rerouting enabled for vehicle " << vehicleID << std::endl;

  // Define a function to periodically print the road ID and trigger rerouting
  auto rerouteAndPrint = std::make_shared<std::function<void()>>();
  *rerouteAndPrint = [vehicleID, sumoClient, rerouteAndPrint]() {
      // Get the current road ID
      auto roadID = sumoClient->TraCIAPI::vehicle.getRoadID(vehicleID);
      std::cout << "Road ID for vehicle " << vehicleID << ": " << roadID << " at time " << Simulator::Now().GetSeconds() << "s" << std::endl;

      // Trigger rerouting (optional, as SUMO handles rerouting automatically based on the period)
      sumoClient->TraCIAPI::vehicle.rerouteTraveltime(vehicleID);
      std::cout << "Rerouting triggered for vehicle " << vehicleID << std::endl;

      // Schedule the next execution of this function
      Simulator::Schedule(Seconds(5.0), *rerouteAndPrint);
  };

  // Schedule the first execution of the rerouteAndPrint function
  Simulator::Schedule(Seconds(5.0), *rerouteAndPrint);

  return node; // Return the node
};


// Define an empty shutdown function
auto emptyShutdownFcn = [] (Ptr<Node> exNode, std::string vehicleID) {
  // std::cout << "Empty shutdown function called for vehicleID: " << vehicleID << std::endl;
  // No dynamic node shutdown
};

  // Start SUMO and connect to TraCI
  try
  {
    std::cout << "Starting SUMO..." << std::endl;
    sumoClient->SumoSetup(StartupFcn, emptyShutdownFcn); // Provide the required callbacks
    std::cout << "SUMO Started" << std::endl;

    Simulator::Schedule(Seconds(10.0), &ns3::UpdateRoadWeights, sumoClient);
  }
  catch (const std::exception &e)
  {
    std::cout << "Failed to start SUMO: " << e.what() << std::endl;
    return 1;
  }

  // Run the simulation for a short time to keep SUMO running
  std::cout << "Starting simulation..." << std::endl;
  Simulator::Stop(Seconds(30.0));
  Simulator::Run();
  std::cout << "Simulation completed." << std::endl;
  Simulator::Destroy();
  std::cout << "Simulator destroyed." << std::endl;

  return 0;
}