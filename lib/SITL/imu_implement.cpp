//
// Created by 李翌文 on 2021/7/29.
//

#include "imu_implement.h"
#include "common_handler.h"

ImuSITLImpl::ImuSITLImpl() = default;

void ImuSITLImpl::init() {

}

void ImuSITLImpl::updateAcc(uint32_t currentTime) {
    read((uint8_t *) acc, (uint8_t *) handler.acc, 6);
}

void ImuSITLImpl::updateGyro(uint32_t currentTime) {
    read((uint8_t *) gyro, (uint8_t *) handler.gyro, 6);
}

void ImuSITLImpl::updateMag(uint32_t currentTime) {
    read((uint8_t *) mag, (uint8_t *) handler.mag, 6);
}

void ImuSITLImpl::updateBaro(uint32_t currentTime) {
    ct = handler.ct;
    cp = handler.cp;
    ccp = handler.ccp;
}

void ImuSITLImpl::updateSonar(uint32_t currentTime) {

}

void ImuSITLImpl::getRawData(int16_t *buf, uint8_t length) {
    uint8_t stepLength = min2(3, length);
    read((uint8_t *) buf, 0, (uint8_t *) acc, 0, stepLength * 2);
    if (stepLength < 3) {
        return;
    }

    length -= stepLength;
    stepLength = min2(3, length);
    read((uint8_t *) buf, 6, (uint8_t *) acc, 0, stepLength * 2);
    if (stepLength < 3) {
        return;
    }

    length -= stepLength;
    stepLength = min2(3, length);
    read((uint8_t *) buf, 12, (uint8_t *) acc, 0, stepLength * 2);
}

void ImuSITLImpl::getAccData(int16_t *buf, uint8_t length) {
    read((uint8_t *) buf, (uint8_t *) acc, min2(length, 3) * 2);
}

void ImuSITLImpl::getGyroData(int16_t *buf, uint8_t length) {
    read((uint8_t *) buf, (uint8_t *) gyro, min2(length, 3) * 2);
}

void ImuSITLImpl::getMagData(int16_t *buf, uint8_t length) {
    read((uint8_t *) buf, (uint8_t *) mag, min2(length, 3) * 2);
}

void ImuSITLImpl::getBaroData(int16_t *ct, int32_t *cp, int32_t *ccp) {
    *ct = this->ct;
    *cp = this->cp;
    *ccp = this->ccp;
}

void ImuSITLImpl::accCalibration() {}

void ImuSITLImpl::magCalibration() {}

void ImuSITLImpl::baroCalibration() {}
