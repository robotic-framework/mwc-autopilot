//
// Created by 李翌文 on 2021/6/15.
//

#ifndef AUTOPILOT_CONTROLLER_H
#define AUTOPILOT_CONTROLLER_H

#include "attitude_algorithm/attitude_algorithm.h"
#include "../configuration/configuration.h"
#include "imu.h"
#include "motors.h"

class ACSController {
private:
    Configuration *conf;
    AttitudeAlgorithm *aa;
    IMU imu;
    Motors motors;

public:
    ACSController(Configuration *conf) {
        this->conf = conf;
    }
    void Init();

    void UpdateAcc(uint32_t currentTime);
    void UpdateGyro(uint32_t currentTime);
    void UpdateMag(uint32_t currentTime);
    void UpdateBaro(uint32_t currentTime);
    void UpdateSonar(uint32_t currentTime);
    void UpdateAttitude(uint32_t currentTime);
    void UpdateAltitude(uint32_t currentTime);
    void UpdatePID(uint32_t currentTime);
    void UpdateMotors(uint32_t currentTime);
};


#endif //AUTOPILOT_CONTROLLER_H
