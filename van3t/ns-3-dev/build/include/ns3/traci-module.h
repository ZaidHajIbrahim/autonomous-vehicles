#ifdef NS3_MODULE_COMPILATION 
    error "Do not include ns3 module aggregator headers from other modules these are meant only for end user scripts." 
#endif 
#ifndef NS3_MODULE_TRACI
    // Module headers: 
    #include <ns3/traci-client.h>
    #include <ns3/sumo-TraCIAPI.h>
    #include <ns3/sumo-config.h>
    #include <ns3/sumo-socket.h>
    #include <ns3/sumo-storage.h>
    #include <ns3/sumo-TraCIConstants.h>
    #include <ns3/sumo-TraCIDefs.h>
#endif 