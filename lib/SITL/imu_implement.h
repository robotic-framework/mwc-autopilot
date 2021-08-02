//
// Created by 李翌文 on 2021/7/29.
//

#ifndef AUTOPILOT_IMU_IMPLEMENT_H
#define AUTOPILOT_IMU_IMPLEMENT_H

#include "definition.h"
#include "acs/imu.h"

class ImuSITLImpl : public IMU {
private:
    int16_t acc[3];
    int16_t gyro[3];
    int16_t mag[3];
    int16_t ct;
    int32_t cp;
    int32_t ccp;

public:
    ImuSITLImpl();

    void init() override;

    void updateAcc(uint32_t currentTime) override;

    void updateGyro(uint32_t currentTime) override;

    void updateMag(uint32_t currentTime) override;

    void updateBaro(uint32_t currentTime) override;

    void updateSonar(uint32_t currentTime) override;

    void getRawData(int16_t *buf, uint8_t length) override;

    void getAccData(int16_t *buf, uint8_t length) override;

    void getGyroData(int16_t *buf, uint8_t length) override;

    void getMagData(int16_t *buf, uint8_t length) override;

    void getBaroData(int16_t *ct, int32_t *cp, int32_t *ccp) override;

    void accCalibration() override;

    void magCalibration() override;

    void baroCalibration() override;

    bool isAccCalibrating() override {
        return false;
    }

    bool isGyroCalibrating() override {
        return false;
    }

    bool isMagCalibrating() override {
        return false;
    }

    bool isBaroCalibrating() override {
        return false;
    }
};


#endif //AUTOPILOT_IMU_IMPLEMENT_H
