#ifndef MAPF_REROUTER_SERVER_H
#define MAPF_REROUTER_SERVER_H

#include "ns3/application.h"
#include "ns3/traci-client.h"
#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include <stdexcept>

namespace ns3 {

class MAPFRerouterServer : public Application {
public:
    static TypeId GetTypeId(void);

    MAPFRerouterServer();
    virtual ~MAPFRerouterServer();

    // Method to gather vehicle positions (ID and current road)
    std::vector<std::pair<std::string, std::string>> GetVehiclePositions();

    // Method to gather vehicle destinations (ID and destination edge)
    std::unordered_map<std::string, std::string> GetVehicleDestinations();

    // Method to run CCBS and process the solution
    bool RunCCBSAndProcessSolution(const std::string &taskFile, const std::string &graphmlFile);

protected:
    virtual void StartApplication() override;
    virtual void StopApplication() override;

private:
    Ptr<TraciClient> m_sumoClient; // Pointer to the SUMO TraCI client
};

} // namespace ns3

#endif // MAPF_REROUTER_SERVER_H