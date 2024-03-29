#include "acs/imu_impl.h"

ImuImpl::ImuImpl() {
#if defined(ACC_ADXL345)
    acc = new ADXL345(ADXL345_DEVICE);
#endif
#if defined(ACC_BMA180)
    acc = new BMA180(BMA180_DEVICE);
#endif

#if defined(GYRO_ITG3205)
    gyro = new ITG3205(ITG3205_DEVICE);
#endif

#if defined(MAG_HMC5883L)
    mag = new HMC5883L(HMC5883L_DEVICE);
#endif

#if defined(BARO_BMP085)
    baro = new BMP085(BMP085_DEVICE);
#endif

}

void ImuImpl::init() {
    Wire.begin();

    while (1) {
#if SENSOR_ACC
        acc->init();
#endif

#if SENSOR_GYRO
        gyro->init();
#endif

#if SENSOR_MAG
        mag->init();
#endif

#if SENSOR_BARO
        baro->init();
#endif

        if (I2C::getErrCount() == 0) {
            break;
        }
        blinkLED(2, 100, 3);
        delay(1000);
    }

    blinkLED(2, 50, 10);
    LEDPIN_OFF;
}

void ImuImpl::accCalibration() {
#if SENSOR_ACC
    acc->calibration();
#endif

#if SENSOR_GYRO
    gyro->calibration();
#endif
}

void ImuImpl::magCalibration() {
#if SENSOR_MAG
    mag->calibration();
#endif
}

void ImuImpl::baroCalibration() {
#if SENSOR_BARO
    baro->calibration(200);
#endif
}

// Attention: the 'length' must be consistent with the length of the array pointed to by the 'buf'
void ImuImpl::getRawData(int16_t *buf, uint8_t length) {
    uint8_t stepLength = min2(3, length);
    for (uint8_t i = 0; i < stepLength; i++) {
#if SENSOR_ACC
        *(buf + i) = accSmooth[i];
#else
        *(buf + i) = 0;
#endif
    }
    if (stepLength < 3) {
        return;
    }

    length -= stepLength;
    stepLength = min2(3, length);
    for (uint8_t i = 0; i < stepLength; i++) {
#if SENSOR_GYRO
        *(buf + 3 + i) = gyroWeighted[i];
#else
        *(buf + 3 + i) = 0;
#endif
    }

    if (stepLength < 3) {
        return;
    }

    length -= stepLength;
    stepLength = min2(3, length);

#if SENSOR_MAG
    mag->getData(buf + 6, stepLength);
#else
    for (uint8_t i = 0; i < stepLength; i++) {
        *(buf + 6 + i) = 0;
    }
#endif
}

void ImuImpl::getAccData(int16_t *buf, uint8_t length) {
    length = min2(length, 3);
    for (uint8_t i = 0; i < length; i++) {
#if SENSOR_ACC
        *(buf + i) = accSmooth[i];
#else
        *(buf + i) = 0;
#endif
    }
}

void ImuImpl::getGyroData(int16_t *buf, uint8_t length) {
    length = min2(length, 3);
    for (uint8_t i = 0; i < length; i++) {
#if SENSOR_GYRO
        *(buf + i) = gyroWeighted[i];
#else
        *(buf + i) = 0;
#endif
    }
}

void ImuImpl::getMagData(int16_t *buf, uint8_t length) {
#if SENSOR_MAG
    mag->getData(buf, length);
#else
    length = min2(length, 3);
    for (uint8_t i = 0; i < length; i++) {
        *(buf + i) = 0;
    }
#endif
}

void ImuImpl::getBaroData(int16_t *ct, int32_t *cp, int32_t *ccp) {
#if SENSOR_BARO
    *ct = baro->getCTData();
    *cp = baro->getCPData();
    *ccp = baro->getCCPData();
#else
    *ct = 0;
    *cp = 0;
    *ccp = 0;
#endif
}

void ImuImpl::updateAcc(uint32_t currentTime) {
#if SENSOR_ACC
    // update sensors data
    acc->update(currentTime);

    int16_t accData[3] = {0, 0, 0};
    acc->getData(accData, 3);

    for (uint8_t axis = 0; axis < 3; axis++) {
        // valid as long as LPF_FACTOR is less than 15
        accSmooth[axis] = accLPF[axis] >> ACC_LPF_FACTOR;
        accLPF[axis] += accData[axis] - accSmooth[axis];
    }
#endif
}

void ImuImpl::updateGyro(uint32_t currentTime) {
#if SENSOR_GYRO
    // calc gyro weights
    gyro->update(currentTime);
    gyro->getData(gyroWeight, 3);

    // update gyroscope
    int16_t currentGyro[3];
    gyro->update(currentTime);
    gyro->getData(currentGyro, 3);
    for (uint8_t i = 0; i < 3; i++) {
        gyroWeight[i] = currentGyro[i] + gyroWeight[i];
        gyroWeighted[i] = (gyroWeight[i] + gyroPrevWeight[i]) / 3;
        gyroPrevWeight[i] = gyroWeight[i] >> 1;
    }
#endif
}

void ImuImpl::updateMag(uint32_t currentTime) {
#if SENSOR_MAG
    mag->update(currentTime);
#endif
}

void ImuImpl::updateBaro(uint32_t currentTime) {
#if SENSOR_BARO
    baro->update(currentTime);
#endif
}

void ImuImpl::updateSonar(uint32_t currentTime) {
#if SENSOR_SONAR

#endif
}
