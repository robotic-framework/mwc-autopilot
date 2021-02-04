//
// Created by 李翌文 on 2021/2/4.
//

#ifndef AUTOPILOT_TYPE_DEF_H
#define AUTOPILOT_TYPE_DEF_H

#include "stdint.h"

enum RC
{
    ROLL,
    PITCH,
    YAW,
    THROTTLE,
    AUX1,
    AUX2,
    AUX3,
    AUX4,
    AUX5,
    AUX6,
    AUX7,
    AUX8
};

struct PID
{
    uint8_t P;
    uint8_t I;
    uint8_t D;
};

enum PIDType
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

enum BoxType
{
    ARM,
#if SENSOR_ACC
    ANGLE,
    HORIZEN,
#endif
#if SENSOR_MAG
    MAG,
#endif
    BOXITEMS
};

#endif //AUTOPILOT_TYPE_DEF_H
