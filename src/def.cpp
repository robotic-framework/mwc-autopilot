#include "def.h"

Motors motors;
IMU imu;
uint8_t smallAngle25 = 1;
uint16_t cycleTime = 0;

Configuration conf;
int16_t rcCommand[4] = {0, 0, 0, MINTHROTTLE};

#if GPS_ENABLED
Navigation nav;
#endif
