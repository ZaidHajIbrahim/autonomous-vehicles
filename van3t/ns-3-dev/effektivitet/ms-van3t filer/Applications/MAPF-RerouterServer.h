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

#include "MAPF-RerouterClient.h"

namespace ns3 {

class MAPFRerouterClient;

class MAPFRerouterServer : public Application {
public:
    static TypeId GetTypeId(void);

    MAPFRerouterServer();
    virtual ~MAPFRerouterServer();

    // Public methods
    std::vector<std::pair<std::string, std::string>> GetVehiclePositions();
    std::vector<std::tuple<std::string, std::string>> GatherVehicleData();
    bool RunCCBSAndProcessSolution(const std::string &taskFile, const std::string &graphmlFile);
    //void ReceiveRoute(const std::string &route);
    void SendRoutesToClients(const std::vector<std::vector<std::string>> &ccbsRoutes);
    void SetClient(Ptr<TraciClient> client);
    void ScheduleGatherVehicleData();
    void GatherVehicleDataWrapper();
    void AddClient(Ptr<MAPFRerouterClient> client);
    void SetFilePaths(const std::string &netFile, const std::string &outputFile);
    void createCCBSgraph();

protected:
    virtual void StartApplication() override;
    virtual void StopApplication() override;

private:
    // Private methods
    bool createTaskFile(const std::string &taskFile, const std::vector<std::tuple<std::string, std::string>> &vehicleData);
    bool convertSolutionToSUMO(const std::string &taskFile, const std::string &graphmlFile);

    bool runCCBS();
    void CallPythonFunction(const std::string &moduleName, const std::string &functionName, const std::vector<std::string> &args);
    std::string runCommand(const std::string &command);

    // Private members
    Ptr<TraciClient> m_sumoClient; // Pointer to the SUMO TraCI client
    std::vector<Ptr<MAPFRerouterClient>> m_clients; // List of connected clients
    std::string m_id;
    std::string m_netFile;
    std::string m_outputFile;
};

} // namespace ns3

#endif // MAPF_REROUTER_SERVER_H