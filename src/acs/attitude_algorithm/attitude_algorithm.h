//
// Created by 李翌文 on 2021/6/11.
//

#ifndef AUTOPILOT_ATTITUDE_ALGORITHM_H
#define AUTOPILOT_ATTITUDE_ALGORITHM_H

#include "stdint.h"
#include "../../definition.h"
#include "../../type_def.h"
#include "../imu.h"

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

class AttitudeAlgorithm {

protected:
    IMU *imu;

protected:
    Attitude att{};
    Altitude alt{};
    int16_t accZ;

#if defined(TEST_ALTHOLD)
    int32_t testAltBase;
#endif

public:
    virtual void UpdateAttitude(uint32_t currentTime) = 0;

    virtual void UpdateAltitude(uint32_t currentTime) = 0;

    void GetAttitude(int16_t *buf, uint8_t length) {
        if (length < 3) {
            if (length > 1) {
                *buf = att.Angle[ROLL];
                *(buf + 1) = att.Angle[PITCH];
            } else {
                *buf = att.Angle[ROLL];
            }
        } else {
            *buf = att.Angle[ROLL];
            *(buf + 1) = att.Angle[PITCH];
            *(buf + 2) = att.Heading;
        }
    }

    void GetAltitude(int32_t *targetAlt, int16_t *targetVario) {
        *targetAlt = alt.Alt;
        *targetVario = alt.Vario;
    }

    void GetAltitude(int32_t *targetAlt) {
        *targetAlt = alt.Alt;
    }

    void SetAltitudeVario(int16_t vario) {
        alt.Vario = vario;
    }

    void InjectIMU(IMU *imu) {
        this->imu = imu;
    }

    int16_t GetACCZ() {
        return this->accZ;
    }

#if defined(TEST_ALTHOLD)

    void SetTestAltBase(uint16_t a) {
        testAltBase = a;
    }

#endif

};

#endif //AUTOPILOT_ATTITUDE_ALGORITHM_H
