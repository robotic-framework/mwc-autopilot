#if !defined(_PID_H_)
#define _PID_H_

#include "stdint.h"
#include "../configuration/configuration.h"
#include "imu.h"
#include "attitude_algorithm/attitude_algorithm.h"

class PIDController
{
private:
    Configuration *conf;
    IMU *imu;
    AttitudeAlgorithm *aa;

    int16_t pidOffset[3];
    int32_t prop;
    static int16_t errorAltI;
    uint8_t lastBaroMode;
    uint8_t isBaroModeChanged;
    int16_t initialThrottleHold;

public:
    PIDController(Configuration *conf);

    void Update(uint32_t currentTime);
    uint16_t MixPID(int8_t x, int8_t y, int8_t z);

    void InjectIMU(IMU *imu) {
        this->imu = imu;
    }

    void InjectAttitudeAlgorithm(AttitudeAlgorithm *aa) {
        this->aa = aa;
    }
};

#endif
