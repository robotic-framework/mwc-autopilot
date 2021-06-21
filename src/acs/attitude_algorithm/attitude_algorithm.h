//
// Created by 李翌文 on 2021/6/11.
//

#ifndef AUTOPILOT_ATTITUDE_ALGORITHM_H
#define AUTOPILOT_ATTITUDE_ALGORITHM_H

#include "stdint.h"
#include "../../definition.h"
#include "../../type_def.h"
#include "../imu.h"

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
    virtual void updateAttitude(uint32_t currentTime) = 0;

    virtual void updateAltitude(uint32_t currentTime) = 0;

    void getAttitude(int16_t *buf, uint8_t length) {
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

    void getAltitude(int32_t *targetAlt, int16_t *targetVario) const {
        *targetAlt = alt.Alt;
        *targetVario = alt.Vario;
    }

    void getAltitude(int32_t *targetAlt) const {
        *targetAlt = alt.Alt;
    }

    void setAltitudeVario(int16_t vario) {
        alt.Vario = vario;
    }

    void injectIMU(IMU *imu) {
        this->imu = imu;
    }

    int16_t getAccZ() const {
        return this->accZ;
    }

#if defined(TEST_ALTHOLD)

    void setTestAltBase(uint16_t a) {
        testAltBase = a;
    }

#endif

};

#endif //AUTOPILOT_ATTITUDE_ALGORITHM_H
