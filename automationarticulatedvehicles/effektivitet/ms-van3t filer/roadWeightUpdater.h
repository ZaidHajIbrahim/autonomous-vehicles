#ifndef ROAD_WEIGHT_UPDATER_H
#define ROAD_WEIGHT_UPDATER_H

#include "ns3/traci-module.h"

namespace ns3 {
void UpdateRoadWeights(Ptr<TraciClient> sumoClient);
}

#endif // ROAD_WEIGHT_UPDATER_H