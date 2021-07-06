#ifndef IMU_H_
#define IMU_H_

#include <Wire.h>
#include "config.h"
#include "../mixed/led.h"

#if SENSOR_ACC

#include "sensors/accelerator.h"

#endif
#if SENSOR_GYRO

#include "sensors/gyroscope.h"

#endif
#if SENSOR_MAG
#include "sensors/meganetometer.h"
#endif
#if SENSOR_BARO

#include "sensors/barometer.h"

#endif

#if defined(ACC_ADXL345)
#include "sensors/ADXL345.h"
#endif
#if defined(ACC_BMA180)

#include "sensors/BMA180.h"

#endif

#if defined(GYRO_ITG3205)

#include "sensors/ITG3205.h"

#endif

#if defined(MAG_HMC5883L)
#include "sensors/HMC5883L.h"
#endif

#if defined(BARO_BMP085)

#include "sensors/BMP085.h"

#endif

class IMU {
private:
    // sensors
#if SENSOR_ACC
    Accelerator *acc;
#endif

#if SENSOR_GYRO
    Gyroscope *gyro;
#endif

#if SENSOR_MAG
    Magnetometer *mag;
#endif

#if SENSOR_BARO
    Barometer *baro;
#endif

    // indicators
    int16_t accSmooth[3] = {0, 0, 0};
    uint32_t accLPF[3] = {0, 0, 0};

    int16_t gyroWeighted[3] = {0, 0, 0};
    int16_t gyroPrevWeight[3] = {0, 0, 0};
    int16_t gyroWeight[3] = {0, 0, 0};

public:
    IMU();

    void init();

    void updateAcc(uint32_t currentTime);

    void updateGyro(uint32_t currentTime);

    void updateMag(uint32_t currentTime);

    void updateBaro(uint32_t currentTime);

    void updateSonar(uint32_t currentTime);

    void getRawData(int16_t *buf, uint8_t length);

    void getAccData(int16_t *buf, uint8_t length);

    void getGyroData(int16_t *buf, uint8_t length);

    void getMagData(int16_t *buf, uint8_t length);

    void getBaroData(int16_t *ct, int32_t *cp, int32_t *ccp);

    void accCalibration();

    void magCalibration();

    void baroCalibration();

    bool isAccCalibrating() {
#if SENSOR_ACC
        return this->acc->isCalibrating();
#else
        return false;
#endif
    }

    bool isGyroCalibrating() {
#if SENSOR_GYRO
        return this->gyro->isCalibrating();
#else
        return false;
#endif
    }

    bool isMagCalibrating() {
#if SENSOR_MAG
        return this->mag->IsCalibrating();
#else
        return false;
#endif
    }

    bool isBaroCalibrating() {
#if SENSOR_BARO
        return this->baro->isCalibrating();
#else
        return false;
#endif
    }
};

#endif // IMU_H_
