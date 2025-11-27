#ifndef MAPF_REROUTER_CLIENT_H
#define MAPF_REROUTER_CLIENT_H
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/application.h"
#include "ns3/traci-client.h"
#include <string>
#include "MAPF-RerouterServer.h"

namespace ns3 {

class MAPFRerouterServer;

class MAPFRerouterClient : public Application {
public:
    static TypeId GetTypeId(void);

    MAPFRerouterClient();
    virtual ~MAPFRerouterClient();

    void SetVehicleID(const std::string &vehicleID);
    std::string GetVehicleID() const;

    std::string GetCurrentRoadID() const;
    std::string GetDestination() const;
    std::vector<std::string> GetRoute() const;

    void ReceiveRoute(const std::vector<std::string> &route);

    void SetServer(Ptr<MAPFRerouterServer> server);

    static ApplicationContainer Install(Ptr<Node> node);
    static ApplicationContainer Install(NodeContainer nodes);

    virtual void StopApplication(void);


private:
    virtual void StartApplication (void);
     


    std::string m_vehicleID; // Vehicle ID
    Ptr<TraciClient> m_sumoClient; // Pointer to the SUMO TraCI client
    Ptr<MAPFRerouterServer> m_server; // Pointer to the server
};

} // namespace ns3

#endif // MAPF_REROUTER_CLIENT_H