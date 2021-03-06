#include "def.h"

Motors motors;
IMU imu;
uint8_t smallAngle25 = 1;
uint16_t cycleTime = 0;
bool arm = 0;
bool angleMode = 1;
bool horizenMode = 1;
bool baroMode = 0;
int32_t altHold = 0;

int16_t rcCommand[4] = {0, 0, 0, MINTHROTTLE};
PID pid[PIDITEMS];

#if GPS_ENABLED
Navigation nav;
#endif

void loadDefaults()
{
    pid[ROLL].P = 33;
    pid[ROLL].I = 30;
    pid[ROLL].D = 23;
    pid[PITCH].P = 33;
    pid[PITCH].I = 30;
    pid[PITCH].D = 23;
    pid[YAW].P = 68;
    pid[YAW].I = 45;
    pid[YAW].D = 0;
    pid[PIDLEVEL].P = 90;
    pid[PIDLEVEL].I = 10;
    pid[PIDLEVEL].D = 100;
    pid[PIDALT].P = 64;
    pid[PIDALT].I = 25;
    pid[PIDALT].D = 24;
}
