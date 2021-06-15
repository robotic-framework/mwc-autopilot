#include "global.h"

Configuration conf;
ACSController acs(&conf);
uint8_t smallAngle25 = 1;
uint16_t cycleTime = 0;

int16_t rcCommand[4] = {0, 0, 0, MINTHROTTLE};

#if GPS_ENABLED
Navigation nav;
#endif
