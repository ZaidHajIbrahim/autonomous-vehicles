#ifndef MAPF_REROUTER_CLIENT_H
#define MAPF_REROUTER_CLIENT_H

#include "ns3/application.h"
#include "ns3/traci-client.h"
#include <string>

namespace ns3 {

class MAPFRerouterClient : public Application {
public:
    static TypeId GetTypeId(void);

    MAPFRerouterClient();
    virtual ~MAPFRerouterClient();

    void SetVehicleID(const std::string &vehicleID);
    std::string GetVehicleID() const;

    std::string GetCurrentRoadID() const;
    std::string GetDestination() const;

protected:
    virtual void StartApplication() override;
    virtual void StopApplication() override;

private:
    std::string m_vehicleID; // Vehicle ID
    Ptr<TraciClient> m_sumoClient; // Pointer to the SUMO TraCI client
};

} // namespace ns3

#endif // MAPF_REROUTER_CLIENT_H