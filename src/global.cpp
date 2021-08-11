#include "global.h"

Configuration conf;
ACSController acs(&conf);
uint8_t smallAngle25 = 1;
uint16_t cycleTime = 0;
RCCommander rc;

#if GPS_ENABLED
Navigation nav(&conf);
#endif
