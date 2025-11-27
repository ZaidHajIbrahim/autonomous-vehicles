#include "MAPF-RerouterClient.h"
#include "MAPF-RerouterServer.h"
#include "ns3/log.h"
#include "ns3/names.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("MAPFRerouterClient");
NS_OBJECT_ENSURE_REGISTERED(MAPFRerouterClient);

TypeId MAPFRerouterClient::GetTypeId(void) {
    static TypeId tid = TypeId("ns3::MAPFRerouterClient")
        .SetParent<Application>()
        .SetGroupName("Applications")
        .AddConstructor<MAPFRerouterClient>()
        .AddAttribute("VehicleID",
            "The ID of the vehicle associated with this client.",
            StringValue(""),
            MakeStringAccessor(&MAPFRerouterClient::m_vehicleID),
            MakeStringChecker())
        .AddAttribute ("Client",
            "TraCI client for SUMO",
            PointerValue (0),
            MakePointerAccessor (&MAPFRerouterClient::m_sumoClient),
            MakePointerChecker<TraciClient> ());
        
    return tid;
}

MAPFRerouterClient::MAPFRerouterClient() {

    NS_LOG_FUNCTION(this);

    m_sumoClient = nullptr;

}

MAPFRerouterClient::~MAPFRerouterClient() {}

Ptr<MAPFRerouterServer> m_server;

void MAPFRerouterClient::SetVehicleID(const std::string &vehicleID) {
    m_vehicleID = vehicleID;
}

void MAPFRerouterClient::SetServer(Ptr<MAPFRerouterServer> server) {
    m_server = server;
    if (m_server) {
        m_server->AddClient(this);
    }
}

std::string MAPFRerouterClient::GetVehicleID() const {
    //std::cout << "Vehicle ID: " << m_vehicleID << std::endl;
    return m_vehicleID;
}

std::string MAPFRerouterClient::GetCurrentRoadID() const {
    if (m_sumoClient) {
        return m_sumoClient->TraCIAPI::vehicle.getRoadID(m_vehicleID);
    }
    return "UNKNOWN";
}

std::string MAPFRerouterClient::GetDestination() const {
    if (m_sumoClient) {
        auto route = m_sumoClient->TraCIAPI::vehicle.getRoute(m_vehicleID);
        if (!route.empty()) {
            return route.back(); // Return the last edge in the route
        }
    }
    
    return "UNKNOWN";
}
// Only for testing
std::vector<std::string> MAPFRerouterClient::GetRoute() const {
    if (m_sumoClient) {
        auto route = m_sumoClient->TraCIAPI::vehicle.getRoute(m_vehicleID);
        if (!route.empty()) {
            return route; // Return the last edge in the route
        }
    }
    
    return {"UNKNOWN"} ;
}



void MAPFRerouterClient::ReceiveRoute(const std::vector<std::string> &route) {
    if (m_sumoClient) {
        m_sumoClient->TraCIAPI::vehicle.setRoute(m_vehicleID, route);
    }
    std::cout << "Received new route for vehicle " << m_vehicleID << ": " << route.size() << " edges." << std::endl;
    NS_LOG_INFO("Received new route for vehicle " << m_vehicleID << ": " << route.size() << " edges.");
}

void MAPFRerouterClient::StartApplication() {
    
    NS_LOG_INFO("Starting MAPFRerouterClient for node: " << GetNode()->GetId());
    m_vehicleID = m_sumoClient->GetVehicleId (this->GetNode ());
    //m_sumoClient = GetNode()->GetObject<TraciClient>();
    if (!m_sumoClient) {
        std::cout << "TraciClient not found on node " << GetNode()->GetId() << "!" << std::endl;
        NS_LOG_ERROR("TraciClient not found on node!");
    }
    else {
        std::cout << "TraciClient found on node " << GetNode()->GetId() << "!" << std::endl;
    }
    //std::cout << "Vehicle ID: " << m_vehicleID << std::endl;
    
}

void MAPFRerouterClient::StopApplication() {
    NS_LOG_INFO("Stopping MAPFRerouterClient for vehicle: " << m_vehicleID);
    m_sumoClient = nullptr;
}

ApplicationContainer MAPFRerouterClient::Install(Ptr<Node> node) {
    Ptr<MAPFRerouterClient> app = CreateObject<MAPFRerouterClient>();
    node->AddApplication(app);
    return ApplicationContainer(app);
}

ApplicationContainer MAPFRerouterClient::Install(NodeContainer nodes) {
    ApplicationContainer apps;
    for (NodeContainer::Iterator i = nodes.Begin(); i != nodes.End(); ++i) {
        apps.Add(Install(*i));
    }
    return apps;
}

} // namespace ns3
