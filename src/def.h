#if !defined(DEF_H_)
#define DEF_H_

#include "config.h"
#include "motors.h"
#include "imu.h"

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

enum box
{
    ARM,
#if SENSOR_ACC
    ANGLE,
    HORIZEN,
#endif
    MAG,
    BOXITEMS
};

void
loadDefaults();

#endif // DEF_H_
