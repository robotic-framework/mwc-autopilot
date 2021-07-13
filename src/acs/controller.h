//
// Created by 李翌文 on 2021/6/15.
//

#ifndef AUTOPILOT_CONTROLLER_H
#define AUTOPILOT_CONTROLLER_H

#include "attitude_algorithm/attitude_algorithm.h"
#include "../configuration/configuration.h"
#include "imu.h"
#include "pid/pid.h"
#include "motors.h"

class ACSController {
private:
    Configuration *conf;
    AttitudeAlgorithm *aa;
    IMU imu;
    PIDController *pid;
    Motors *motors;

private:
    bool holdAlt;
    bool holdHeading;
    bool holdSpeed;
    bool holdPos;

    int32_t targetAlt;
    int16_t targetHeading;
    int16_t targetSpeed;
    int32_t targetPos[2];

public:
    bool isHoldAlt() const;

    void setHoldAlt(bool holdAlt);

    bool isHoldHeading() const;

    void setHoldHeading(bool holdHeading);

    bool isHoldSpeed() const;

    void setHoldSpeed(bool holdSpeed);

    bool isHoldPos() const;

    void setHoldPos(bool holdPos);

    void getTargetAlt(int32_t *alt) const;

    void setTargetAlt(int32_t targetAlt);

    void getTargetHeading(int16_t *target) const;

    void setTargetHeading(int16_t targetHeading);

    void getTargetSpeed(int16_t *targetSpeed) const;

    void setTargetSpeed(int16_t targetSpeed);

    void getTargetPos(int32_t *buf) const;

    void setTargetPos(int32_t *lat, int32_t *lng);

public:
    ACSController(Configuration *conf) {
        this->conf = conf;
        this->pid = new PIDController(conf);
        this->motors = new Motors(conf);
    }

    void init();

    void updateAcc(uint32_t currentTime);

    void updateGyro(uint32_t currentTime);

    void updateMag(uint32_t currentTime);

    void updateBaro(uint32_t currentTime);

    void updateSonar(uint32_t currentTime);

    void updateAttitude(uint32_t currentTime);

    void updateAltitude(uint32_t currentTime);

    void updatePID(uint32_t currentTime);

    void updateMotors(uint32_t currentTime);

public:
    void getRawData(int16_t *buf, uint8_t length);

    void getAccData(int16_t *buf, uint8_t length);

    void getGyroData(int16_t *buf, uint8_t length);

    void getMagData(int16_t *buf, uint8_t length);

    void getBaroData(int16_t *ct, int32_t *cp, int32_t *ccp);

    void accCalibration();

    void magCalibration();

    void baroCalibration();

public:
    void getAttitude(int16_t *buf, uint8_t length);

    void getAltitude(int32_t *targetAlt);

    void getAltitude(int32_t *targetAlt, int16_t *targetVario);

#if defined(TEST_ALTHOLD)

    void setTestAltBase(uint16_t a);

#endif

public:
    void getMotors(uint16_t *buf, uint8_t length);
};


#endif //AUTOPILOT_CONTROLLER_H
