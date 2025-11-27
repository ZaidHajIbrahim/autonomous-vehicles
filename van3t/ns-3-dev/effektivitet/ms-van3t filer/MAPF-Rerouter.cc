//#include "ns3carla-module.h"
#include "ns3/traci-module.h"
#include "ns3/internet-module.h"
//#include "ns3/wave-module"
#include "ns3/mobility-module.h"
#include "ns3/sumo_xml_parser.h"
//#include "ns3/packet-socket-helper.h"
//#include "ns3/vehicle-visualizer-module.h"
//#include "ns3/MetricSupervisor.h"
#include <unistd.h>

#include "ns3/MAPF-RerouterServer.h"
#include "ns3/MAPFRerouterServerHelper.h"
#include "ns3/MAPF-RerouterClient.h"
#include "ns3/MAPFRerouterClientHelper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MAPF-Rerouter");

int main(int argc, char *argv[]){

    double sumo_updates = 0.001;
    //std::string csv_name;
    //std::string csv_name_cumulative;

    int numberOfNodes;
    uint32_t nodeCounter = 0;
    int numberOfRSUs;
    uint32_t rsuCounter = 0;

    // Command-line arguments
    std::string sumo_folder = "src/automotive/examples/sumo_files_MAPF_map/";
    std::string mob_trace = "cars.rou.xml";
    std::string rsu_file = "stations.xml";
    std::string sumo_config ="src/automotive/examples/sumo_files_MAPF_map/map.sumo.cfg";
    bool sumo_gui = true;
    double simTime = 100;



    CommandLine cmd;
    //cmd.AddValue ("csv-log", "Name of the CSV log file", "MAPF-Rerouter.csv");
    cmd.AddValue("sumo-config", "Path to SUMO configuration file", sumo_config);
    cmd.AddValue("sumo-gui", "Enable SUMO GUI", sumo_gui);
    cmd.Parse(argc, argv);

    xmlDocPtr rou_xml_file;

/*==============================================================*/
/*   Reading the number of RSUs and Cars from their XML-files   */
/*==============================================================*/
    std::cout << "Reading the .rou file..." << std::endl;
    NS_LOG_INFO("Reading the .rou file...");
    
    std::string rou_path = sumo_folder + mob_trace;
    /* Load the .rou.xml document */
    xmlInitParser();
    rou_xml_file = xmlParseFile(rou_path.c_str ());
    if (rou_xml_file == NULL)
        {
          std::cout << "Error: unable to parse the specified XML file: "<<rou_path << std::endl;
          NS_FATAL_ERROR("Error: unable to parse the specified XML file: "<<rou_path);
        }
    numberOfNodes = XML_rou_count_vehicles(rou_xml_file);

   /* Load the stations.xml file */  
    std::string rsu_path = sumo_folder + rsu_file; 
    std::ifstream rsu_file_stream (rsu_path.c_str());
    std::vector<std::tuple<std::string, float, float>> rsuData = XML_poli_count_stations(rsu_file_stream);
    numberOfRSUs = rsuData.size();

    xmlFreeDoc(rou_xml_file);
    xmlCleanupParser();

    if(numberOfNodes==-1)
        {
          std::cout << "Fatal error: cannot gather the number of vehicles from the specified XML file: "<<rou_path<<". Please check if it is a correct SUMO file." << std::endl;
          NS_FATAL_ERROR("Fatal error: cannot gather the number of vehicles from the specified XML file: "<<rou_path<<". Please check if it is a correct SUMO file.");
        }
    std::cout << "The .rou file has been read: " << numberOfNodes << " vehicles will be present in the simulation." << std::endl;
    NS_LOG_INFO("The .rou file has been read: " << numberOfNodes << " vehicles will be present in the simulation.");


    std::cout << "Simulation will last " << simTime << " seconds" << std::endl;
    NS_LOG_INFO("Simulation will last " << simTime << " seconds");
    ns3::Time simulationTime (ns3::Seconds(simTime));


    NodeContainer obuNodes; //Creating the on board unit nodes which will be installed on the cars
    obuNodes.Create(numberOfNodes);

    MobilityHelper mobility; //Installing the mobility model on the nodes
    mobility.Install (obuNodes);
    std::cout << "Mobility model installed on the nodes." << std::endl;

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

/*===========================================*/
/*   Adding the MAPF server to the program   */
/*===========================================*/
Ptr<MAPFRerouterServer> server;
Ptr<Node> serverNode = CreateObject<Node>();





NodeContainer rsuNodes;
rsuNodes.Create(numberOfRSUs);

MobilityHelper mobilityRSU;
mobilityRSU.Install (rsuNodes);

MAPFRerouterServerHelper serverHelper;
serverHelper.SetAttribute("Client", (PointerValue) sumoClient);

int i = 0;
for (auto rsu : rsuData)
{
    std::string id = std::get<0>(rsu);
    float x = std::get<1>(rsu);
    float y = std::get<2>(rsu);
    Ptr<Node> rsuNode = rsuNodes.Get (i);
    ApplicationContainer AppServer = serverHelper.Install (rsuNode);
    AppServer.Start (Seconds (0.0));
    AppServer.Stop (simulationTime - Seconds (0.1));

        // Retrieve the server instance
        if (i == 0) { // Only install the server on the first RSU
            server = DynamicCast<MAPFRerouterServer>(AppServer.Get(0));
        }

    ++rsuCounter;
    ++i;
}
// Ensure the server is initialized before using it
if (server) {
    std::string netFile = sumo_folder + "map.net.xml";
    std::string outputFile = "src/automotive/model/Applications/XML-files/CCBS_map.xml";

    std::cout << "Net file: " << netFile << std::endl;
    std::cout << "Output file: " << outputFile << std::endl;

    // Set the file paths in the server
    server->SetFilePaths(netFile, outputFile);
    std::cout << "File paths set in the server." << std::endl;

    // Call createCCBSgraph
    server->createCCBSgraph();
    std::cout << "createCCBSgraph called successfully." << std::endl;
} else {
    std::cerr << "Error: Server instance not initialized!" << std::endl;
    return -1;
}


MAPFRerouterClientHelper clientHelper;
clientHelper.SetAttribute("Client", (PointerValue) sumoClient);

/*==============================================================================*/
/*   Installing the nodes on each car. This is the purpouse of the StartupFcn   */
/*==============================================================================*/
    auto StartupFcn = [&] (std::string vehicleID, TraciClient::StationTypeTraCI_t stationType) -> Ptr<Node> {
        if (nodeCounter >= obuNodes.GetN()) {
            NS_FATAL_ERROR("Node Pool empty!: " << nodeCounter << " nodes created.");
        }

        Ptr<Node> includedNode = obuNodes.Get(nodeCounter);
        ++nodeCounter;

        ApplicationContainer ClientApp = clientHelper.Install(includedNode);
        ClientApp.Start(Seconds(0.0));
        ClientApp.Stop(simulationTime - Simulator::Now() - Seconds(0.1));

        Ptr<MAPFRerouterClient> client = DynamicCast<MAPFRerouterClient>(ClientApp.Get(0));
        if (client) {
            client->SetServer(server); // Notify the server about this client
            client->SetVehicleID(vehicleID); // Set the vehicle ID
        }

        return includedNode;
    };
    
    auto ShutdownFcn = [&] (Ptr<Node> exNode, std::string vehicleID) {

    };

    sumoClient->SumoSetup (StartupFcn, ShutdownFcn);

    
    Simulator::Stop (simulationTime);
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}




