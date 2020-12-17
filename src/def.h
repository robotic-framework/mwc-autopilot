#if !defined(DEF_H_)
#define DEF_H_

#include "config.h"
#include "motors.h"
#include "imu.h"
#include "sensors/ADXL345.h"

enum SelfTune
{
    DISABLED,
    ANGLE,
    HORIZEN
};

struct PID
{
    uint8_t P;
    uint8_t I;
    uint8_t D;
};

enum pid
{
    PIDROLL,
    PIDPITCH,
    PIDYAW,
    PIDALT,
    PIDPOS,
    PIDPOSR,
    PIDNAVR,
    PIDLEVEL,
    PIDMAG,
    PIDVEL, // not used currently
    PIDITEMS
};

void loadDefaults();

#endif // DEF_H_
