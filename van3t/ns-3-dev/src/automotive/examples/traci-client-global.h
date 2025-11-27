#ifndef TRACI_CLIENT_SINGLETON_H
#define TRACI_CLIENT_SINGLETON_H


#include "ns3/ptr.h"
#include "ns3/traci-client.h"


namespace ns3 {

class TraciClient; // Forward declaration if not already included via ns3/traci-client.h
class CollisionAvoidance; // Forward declaration

// Returns a reference to one global Ptr<TraciClient>
inline Ptr<TraciClient>&
GlobalTraciClient ()
{
 static Ptr<TraciClient> client;
 return client;
}

extern Ptr<CollisionAvoidance> g_collisionAvoiderApp;

} // namespace ns3


#endif // TRACI_CLIENT_SINGLETON_H