#include "MAPFRerouterServerHelper.h"
#include "ns3/log.h"
#include "ns3/names.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("MAPFRerouterServerHelper");

MAPFRerouterServerHelper::MAPFRerouterServerHelper() {
    m_factory.SetTypeId(MAPFRerouterServer::GetTypeId());
}

void MAPFRerouterServerHelper::SetAttribute(const std::string &name, const AttributeValue &value) {
    m_factory.Set(name, value);
}

ApplicationContainer MAPFRerouterServerHelper::Install(Ptr<Node> node) const {
    Ptr<MAPFRerouterServer> app = m_factory.Create<MAPFRerouterServer>();
    node->AddApplication(app);
    return ApplicationContainer(app);
}

ApplicationContainer MAPFRerouterServerHelper::Install(NodeContainer nodes) const {
    ApplicationContainer apps;
    for (NodeContainer::Iterator i = nodes.Begin(); i != nodes.End(); ++i) {
        apps.Add(Install(*i));
    }
    return apps;
}

} // namespace ns3