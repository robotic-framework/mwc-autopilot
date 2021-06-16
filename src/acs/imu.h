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

class IMU
{
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
    int16_t accSmooth[3]{0, 0, 0};
    uint32_t accLPF[3]{0, 0, 0};

    int16_t gyroWeighted[3];
    int16_t gyroPrevWeight[3] = {0, 0, 0};
    int16_t gyroWeight[3];

public:
    IMU();

    void Init();
    void UpdateAcc(uint32_t currentTime);
    void UpdateGyro(uint32_t currentTime);
    void UpdateMag(uint32_t currentTime);
    void UpdateBaro(uint32_t currentTime);
    void UpdateSonar(uint32_t currentTime);

    void GetRawData(int16_t *buf, uint8_t length);
    void GetAccData(int16_t *buf, uint8_t length);
    void GetGyroData(int16_t *buf, uint8_t length);
    void GetMagData(int16_t *buf, uint8_t length);
    void GetBaroData(int16_t *ct, int32_t *cp, int32_t *ccp);
    void GetBaroLogData(float *gps, float *gts);
    void AccCalibration();
    void MagCalibration();
    void BaroCalibration();

    bool IsAccCalibrating() {
#if SENSOR_ACC
        return this->acc->IsCalibrating();
#else
        return false;
#endif
    }
    bool IsGyroCalibrating() {
#if SENSOR_GYRO
        return this->gyro->IsCalibrating();
#else
        return false;
#endif
    }
    bool IsMagCalibrating() {
#if SSENSOR_MAG
        return this->mag->IsCalibrating();
#else
        return false;
#endif
    }

    bool IsBaroCalibrating() {
#if SENSOR_BARO
        return this->baro->IsCalibrating();
#else
        return false;
#endif
    }
};

#endif // IMU_H_
