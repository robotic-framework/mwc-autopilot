#if !defined(_PID_H_)
#define _PID_H_

#include "stdint.h"
#include "configuration/configuration.h"
#include "acs/imu.h"
#include "acs/attitude_algorithm/attitude_algorithm.h"

#if LOG_LEVEL > 0
#include "log/log.h"
#endif

class PIDController
{
private:
    Configuration *conf;
    IMU *imu;
    AttitudeAlgorithm *aa;

    int16_t pidOffset[3];
    int32_t prop;
    static int16_t errorAltI;
    uint8_t lastAltHoldMode;
    bool isAltHoldModeChanged;
    int16_t initialThrottleHold;

public:
    PIDController(Configuration *conf);

    void update(uint32_t currentTime);
    uint16_t mixPID(int8_t x, int8_t y, int8_t z);

    void injectIMU(IMU *imu) {
        this->imu = imu;
    }

    void injectAttitudeAlgorithm(AttitudeAlgorithm *aa) {
        this->aa = aa;
    }
};

#endif
