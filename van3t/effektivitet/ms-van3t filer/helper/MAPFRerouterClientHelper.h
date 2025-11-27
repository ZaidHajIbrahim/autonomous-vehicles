#ifndef MAPF_REROUTER_CLIENT_HELPER_H
#define MAPF_REROUTER_CLIENT_HELPER_H

#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/MAPF-RerouterClient.h"

namespace ns3 {

class MAPFRerouterClientHelper {
public:
    MAPFRerouterClientHelper();

    void SetAttribute(const std::string &name, const AttributeValue &value);

    ApplicationContainer Install(Ptr<Node> node) const;
    ApplicationContainer Install(NodeContainer nodes) const;

private:
    ObjectFactory m_factory;
};

} // namespace ns3

#endif // MAPF_REROUTER_CLIENT_HELPER_H