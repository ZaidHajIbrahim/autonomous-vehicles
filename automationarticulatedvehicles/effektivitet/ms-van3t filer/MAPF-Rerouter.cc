#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/traci-module.h"
#include "ns3/MAPF-RerouterClient.h"
#include "ns3/MAPF-RerouterServer.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleSumoLauncher");

std::string sumo_folder = "src/automotive/examples/sumo_files_v2i_map/";
std::string mob_trace = "cars.rou.xml";
std::string rsu_file = "stations.xml";
std::string sumo_config ="src/automotive/examples/sumo_files_v2i_map/map.sumo.cfg";


int main(int argc, char *argv[]){

    double sumo_updates = 0.01;

    // Command-line arguments
    std::string sumo_config = "src/automotive/examples/sumo_files_v2i_map/map.sumo.cfg";
    bool sumo_gui = true;

    CommandLine cmd;
    cmd.AddValue("sumo-config", "Path to SUMO configuration file", sumo_config);
    cmd.AddValue("sumo-gui", "Enable SUMO GUI", sumo_gui);
    cmd.Parse(argc, argv);

/*===========================================*/
/*Reading the number of RSUs and Cars from their XML-files*/
/*===========================================*/
    NS_LOG_INFO("Reading the .rou file...");
    std::string rou_path = sumo_folder + mob_trace;
    /* Load the .rou.xml document */
    xmlInitParser();
    rou_xml_file = xmlParseFile(rou_path.c_str ());
    if (rou_xml_file == NULL)
        {
          NS_FATAL_ERROR("Error: unable to parse the specified XML file: "<<rou_path);
        }
    numberOfNodes = XML_rou_count_vehicles(rou_xml_file);

    std::string rsu_path = sumo_folder + rsu_file;
    std::ifstream rsu_file_stream (rsu_path.c_str());
    std::vector<std::tuple<std::string, float, float>> rsuData = XML_poli_count_stations(rsu_file_stream);
    numberOfRSUs = rsuData.size();

    xmlFreeDoc(rou_xml_file);
    xmlCleanupParser();


    NodeContainer obuNodes; //Creating the on board unit nodes which will be installed on the cars
    obuNodes.Create(numberOfNodes);

    MobilityHelper mobility; //Installing the mobility model on the nodes
    mobility.Install (obuNodes);


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



/*========================================*/
/*Installing the nodes on each car. This is the purpouse of the StartupFcn*/
/*========================================*/
    auto StartupFcn = [&sumoClient] (std::string vehicleID, TraciClient::StationTypeTraCI_t stationType) -> Ptr<Node> {
        if (nodeCounter >= obuNodes.GetN())
        NS_FATAL_ERROR("Node Pool empty!: " << nodeCounter << " nodes created.");

      /* Don't create and install the protocol stack of the node at simulation time -> take from "node pool" */
      Ptr<Node> includedNode = obuNodes.Get(nodeCounter);
      ++nodeCounter; //increment counter for next node

      /* Install Application */
      
      ApplicationContainer ClientApp = MAPFRerouterClient.Install (includedNode);
      ClientApp.Start (Seconds (0.0));
      ClientApp.Stop (simulationTime - Simulator::Now () - Seconds (0.1));

      return includedNode;
        // Skapa applikation för att skicka och ta emot meddelanden
        // Skall skicka position och mål 
        // Skall ta emot ny rutt
    };
    
    auto ShutdownFcn = [] (Ptr<Node> exNode, std::string vehicleID) {

    };

    try
    {
        std::cout << "Starting SUMO..." << std::endl;
        sumoClient->Start();
        std::cout << "SUMO Started" << std::endl;

        // Lägg till program för att hantera starta python-script och köra C-CBS
        // Alternativt skapa RSU och lägg till programet på dem.
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    std:: << "Starting simulation..." << std::endl;
    Simulator::Stop(Seconds(30.0));
    Simulator::Run();
    std:cout << "Simulation finished." << std::endl;
    Simulator::Destroy();

    return 0;
} 






}
