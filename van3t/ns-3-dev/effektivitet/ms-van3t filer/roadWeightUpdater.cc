#include "roadWeightUpdater.h"
#include "ns3/core-module.h"
#include <iostream>

namespace ns3 {

void UpdateRoadWeights(Ptr<TraciClient> sumoClient) {
    std::cout << "Updating road weights..." << std::endl;

    // Example: Set a custom weight for a specific road
    std::string edgeID = "n1_to_c1"; // Replace with your edge ID
         
    double previousWeight = sumoClient->TraCIAPI::edge.getEffort(edgeID, Simulator::Now().GetSeconds());
    double newWeight = previousWeight + 1.0; // Increment the weight by 1.0 for demonstration
    std::cout << "Previous weight for edge " << edgeID << ": " << previousWeight << std::endl;
    sumoClient->TraCIAPI::edge.setEffort(edgeID, newWeight);
    std::cout << "Updated weight for edge " << edgeID << " to " << newWeight << std::endl;

    // Schedule the next update
    Simulator::Schedule(Seconds(5.0), &UpdateRoadWeights, sumoClient);
}

} // namespace ns3