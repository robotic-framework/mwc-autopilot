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
    Motors *motors;

public:
    ACSController(Configuration *conf) {
        this->conf = conf;
        this->motors = new Motors(conf);
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

public:
    void GetRawData(int16_t *buf, uint8_t length);
    void GetAccData(int16_t *buf, uint8_t length);
    void GetGyroData(int16_t *buf, uint8_t length);
    void GetMagData(int16_t *buf, uint8_t length);
    void GetBaroData(int16_t *ct, int32_t *cp, int32_t *ccp);
    void AccCalibration();
    void MagCalibration();
    void BaroCalibration();

public:
    void GetAttitude(int16_t *buf, uint8_t length);
    void GetAltitude(int32_t *targetAlt);
    void GetAltitude(int32_t *targetAlt, int16_t *targetVario);

#if defined(TEST_ALTHOLD)
    void SetTestAltBase(uint16_t a);
#endif

public:
    void GetMotors(uint16_t *buf, uint8_t length);
    uint8_t GetMotorCount();
};


#endif //AUTOPILOT_CONTROLLER_H
