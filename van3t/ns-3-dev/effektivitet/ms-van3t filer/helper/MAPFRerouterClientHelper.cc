#include "MAPFRerouterClientHelper.h"
#include "ns3/log.h"
#include "ns3/names.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("MAPFRerouterClientHelper");

MAPFRerouterClientHelper::MAPFRerouterClientHelper() {
    m_factory.SetTypeId(MAPFRerouterClient::GetTypeId());
}

void MAPFRerouterClientHelper::SetAttribute(
    const std::string &name, 
    const AttributeValue &value) 
{
  m_factory.Set (name, value);
}

ApplicationContainer MAPFRerouterClientHelper::Install(Ptr<Node> node) const {
    Ptr<MAPFRerouterClient> app = m_factory.Create<MAPFRerouterClient>();
    node->AddApplication(app);
    return ApplicationContainer(app);
}

ApplicationContainer MAPFRerouterClientHelper::Install(NodeContainer nodes) const {
    ApplicationContainer apps;
    for (NodeContainer::Iterator i = nodes.Begin(); i != nodes.End(); ++i) {
        apps.Add(Install(*i));
    }
    return apps;
}

} // namespace ns3