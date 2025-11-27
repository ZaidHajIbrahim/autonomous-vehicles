#ifdef NS3_MODULE_COMPILATION 
    error "Do not include ns3 module aggregator headers from other modules these are meant only for end user scripts." 
#endif 
#ifndef NS3_MODULE_VEHICLE_VISUALIZER
    // Module headers: 
    #include <ns3/vehicle-visualizer.h>
#endif 