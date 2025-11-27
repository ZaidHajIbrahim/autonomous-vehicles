#ifdef NS3_MODULE_COMPILATION 
    error "Do not include ns3 module aggregator headers from other modules these are meant only for end user scripts." 
#endif 
#ifndef NS3_MODULE_GPS_TC
    // Module headers: 
    #include <ns3/utmups.h>
    #include <ns3/utmups_math.h>
    #include <ns3/gps-tc.h>
    #include <ns3/gps-tc-helper.h>
#endif 