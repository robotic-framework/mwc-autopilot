//
// Created by 李翌文 on 2021/7/29.
//

#ifndef AUTOPILOT_IMU_H
#define AUTOPILOT_IMU_H

#include <stdint.h>

class IMU {
public:
    virtual void init() = 0;

    virtual void updateAcc(uint32_t currentTime) = 0;

    virtual void updateGyro(uint32_t currentTime) = 0;

    virtual void updateMag(uint32_t currentTime) = 0;

    virtual void updateBaro(uint32_t currentTime) = 0;

    virtual void updateSonar(uint32_t currentTime) = 0;

    virtual void getRawData(int16_t *buf, uint8_t length) = 0;

    virtual void getAccData(int16_t *buf, uint8_t length) = 0;

    virtual void getGyroData(int16_t *buf, uint8_t length) = 0;

    virtual void getMagData(int16_t *buf, uint8_t length) = 0;

    virtual void getBaroData(int16_t *ct, int32_t *cp, int32_t *ccp) = 0;

    virtual void accCalibration() = 0;

    virtual void magCalibration() = 0;

    virtual void baroCalibration() = 0;

    virtual bool isAccCalibrating() = 0;

    virtual bool isGyroCalibrating() = 0;

    virtual bool isMagCalibrating() = 0;

    virtual bool isBaroCalibrating() = 0;
};

#endif //AUTOPILOT_IMU_H
