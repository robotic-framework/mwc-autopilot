//
// Created by 李翌文 on 2021/2/4.
//

#ifndef AUTOPILOT_TYPE_DEF_H
#define AUTOPILOT_TYPE_DEF_H

#include "stdint.h"

enum RC {
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

struct PID {
    uint8_t P;
    uint8_t I;
    uint8_t D;
};

enum AAType {
    MWC,
};

enum PIDType {
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

enum BoxType {
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

typedef struct {
    int32_t X, Y, Z;
} t_int32_t_vector_def;

typedef struct {
    uint16_t XL;
    int16_t X;
    uint16_t YL;
    int16_t Y;
    uint16_t ZL;
    int16_t Z;
} t_int16_t_vector_def;

// note: we use implicit first 16 MSB bits 32 -> 16 cast. ie V32.X>>16 = V16.X
typedef union {
    int32_t A32[3];
    t_int32_t_vector_def V32;
    int16_t A16[6];
    t_int16_t_vector_def V16;
} t_int32_t_vector;

typedef struct {
    int16_t Angle[2]; // absolute angle inclination in multiple of 0.1 degree    180 deg = 1800
    int16_t Heading;
} Attitude;

typedef struct {
    int32_t Alt;
    int16_t Vario;
} Altitude;

#endif //AUTOPILOT_TYPE_DEF_H
