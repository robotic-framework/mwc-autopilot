//
// Created by 李翌文 on 2021/6/15.
//

#include "acs/controller.h"
#include "acs/attitude_algorithm/mwc_algorithm.h"
#include "attitude_algorithm_implement.h"
#if defined(SITL)
#include "imu_implement.h"
#include "motors_implement.h"
#else
#include "acs/imu_impl.h"
#include "acs/motors_impl.h"
#endif

void ACSController::init() {
#if defined(SITL)
    this->imu = new ImuSITLImpl;
    this->motors = new MotorSITLImpl(conf);
#else
    this->imu = new ImuImpl;
    this->motors = new MotorImpl(conf);
#endif

    this->pid = new PIDController(conf);
    this->motors->init();
    this->imu->init();

#if defined(SITL)
    this->aa = new AAImpl;
#else
    switch (this->conf->raw.aaType) {
        case MWC:
            this->aa = new MWCAlgorithm(this->conf);
    }
#endif

    this->aa->injectIMU(imu);
    this->pid->injectIMU(imu);
    this->pid->injectAttitudeAlgorithm(aa);
}

void ACSController::updateAcc(uint32_t currentTime) {
    imu->updateAcc(currentTime);
}

void ACSController::updateGyro(uint32_t currentTime) {
    imu->updateGyro(currentTime);
}

void ACSController::updateMag(uint32_t currentTime) {
    imu->updateMag(currentTime);
}

void ACSController::updateBaro(uint32_t currentTime) {
    imu->updateBaro(currentTime);
}

void ACSController::updateSonar(uint32_t currentTime) {
    imu->updateSonar(currentTime);
}

void ACSController::updateAttitude(uint32_t currentTime) {
#if SENSOR_ACC && SENSOR_GYRO
    // check if is in _calibration
#if SENSOR_ACC
    if (imu->isAccCalibrating()) {
        return;
    }
#endif
#if SENSOR_GYRO
    if (imu->isGyroCalibrating()) {
        return;
    }
#endif

    aa->updateAttitude(currentTime);
#endif
}

void ACSController::updateAltitude(uint32_t currentTime) {
#if SENSOR_BARO
    if (imu->isBaroCalibrating()) {
        return;
    }

    aa->updateAltitude(currentTime);
#endif
}

void ACSController::updatePID(uint32_t currentTime) {
    if (!conf->arm) {
        return;
    }
    pid->update(currentTime);
    motors->mixPID(pid);
}

void ACSController::updateMotors(uint32_t currentTime) {
    motors->updateMotors(currentTime);
}

void ACSController::getRawData(int16_t *buf, uint8_t length) {
    imu->getRawData(buf, length);
}

void ACSController::getAccData(int16_t *buf, uint8_t length) {
    imu->getAccData(buf, length);
}

void ACSController::getGyroData(int16_t *buf, uint8_t length) {
    imu->getGyroData(buf, length);
}

void ACSController::getMagData(int16_t *buf, uint8_t length) {
    imu->getMagData(buf, length);
}

void ACSController::getBaroData(int16_t *ct, int32_t *cp, int32_t *ccp) {
    imu->getBaroData(ct, cp, ccp);
}

void ACSController::accCalibration() {
    imu->accCalibration();
}

void ACSController::magCalibration() {
    imu->magCalibration();
}

void ACSController::baroCalibration() {
    imu->baroCalibration();
}

void ACSController::getAttitude(int16_t *buf, uint8_t length) {
    aa->getAttitude(buf, length);
}

void ACSController::getAltitude(int32_t *targetAlt) {
    aa->getAltitude(targetAlt);
}

void ACSController::getAltitude(int32_t *targetAlt, int16_t *targetVario) {
    aa->getAltitude(targetAlt, targetVario);
}

#if defined(TEST_ALTHOLD)

void ACSController::setTestAltBase(uint16_t a) {
    aa->setTestAltBase(a);
}

#endif

void ACSController::getMotors(uint16_t *buf, uint8_t length) {
    motors->getMotors(buf, length);
}

bool ACSController::isHoldAlt() const {
    return holdAlt;
}

void ACSController::setHoldAlt(bool hold) {
    this->holdAlt = hold;
}

bool ACSController::isHoldHeading() const {
    return holdHeading;
}

void ACSController::setHoldHeading(bool hold) {
    this->holdHeading = hold;
}

bool ACSController::isHoldSpeed() const {
    return holdSpeed;
}

void ACSController::setHoldSpeed(bool hold) {
    this->holdSpeed = hold;
}

bool ACSController::isHoldPos() const {
    return holdPos;
}

void ACSController::setHoldPos(bool hold) {
    this->holdPos = hold;
}

void ACSController::getTargetAlt(int32_t *target) const {
    *target = this->targetAlt;
}

void ACSController::setTargetAlt(int32_t target) {
    this->targetAlt = target;
}

void ACSController::getTargetHeading(int16_t *target) const {
    *target = this->targetHeading;
}

void ACSController::setTargetHeading(int16_t target) {
    this->targetHeading = target;
}

void ACSController::getTargetSpeed(int16_t *target) const {
    *target = this->targetSpeed;
}

void ACSController::setTargetSpeed(int16_t target) {
    this->targetSpeed = target;
}

void ACSController::getTargetPos(int32_t *buf) const {
    *buf = targetPos[0];
    *(buf + 1) = targetPos[1];
}

void ACSController::setTargetPos(int32_t *lat, int32_t *lng) {
    targetPos[0] = *lat;
    targetPos[1] = *lng;
}
