#include "MAPF-RerouterClient.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("MAPFRerouterClient");
NS_OBJECT_ENSURE_REGISTERED(MAPFRerouterClient);

TypeId MAPFRerouterClient::GetTypeId(void) {
    static TypeId tid = TypeId("ns3::MAPFRerouterClient")
        .SetParent<Application>()
        .SetGroupName("Applications")
        .AddConstructor<MAPFRerouterClient>();
    return tid;
}

MAPFRerouterClient::MAPFRerouterClient() : m_vehicleID("") {}

MAPFRerouterClient::~MAPFRerouterClient() {}

void MAPFRerouterClient::SetVehicleID(const std::string &vehicleID) {
    m_vehicleID = vehicleID;
}

std::string MAPFRerouterClient::GetVehicleID() const {
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

void MAPFRerouterClient::StartApplication() {
    NS_LOG_INFO("Starting MAPFRerouterClient for vehicle: " << m_vehicleID);
    m_sumoClient = GetNode()->GetObject<TraciClient>();
    if (!m_sumoClient) {
        NS_LOG_ERROR("TraciClient not found on node!");
    }
}

void MAPFRerouterClient::StopApplication() {
    NS_LOG_INFO("Stopping MAPFRerouterClient for vehicle: " << m_vehicleID);
    m_sumoClient = nullptr;
}
void MAPFRerouterClient::reciveRoute(const std::string &route) {
    // Handle the received route (e.g., update the vehicle's route in SUMO)
    if (m_sumoClient) {
        m_sumoClient->TraCIAPI::vehicle.setRoute(m_vehicleID, route);
    }
    NS_LOG_INFO("Received new route for vehicle " << m_vehicleID << ": " << route);

} // namespace ns3

