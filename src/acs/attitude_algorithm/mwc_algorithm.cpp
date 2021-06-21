//
// Created by 李翌文 on 2021/6/11.
//

#include "mwc_algorithm.h"
#include "../utils.h"

MWCAlgorithm::MWCAlgorithm(Configuration *conf) : estPrevTime(0) {
    this->conf = conf;
    this->estimatedGyroData = {0, 0, (int32_t) ACC_1G_LSB << 16};
#if !SENSOR_MAG
    this->estimatedMagData = {0, (int32_t) 1 << 24, 0};
#endif
}

void MWCAlgorithm::updateAttitude(uint32_t currentTime) {
    uint8_t axis;
    float invGyro; // 1/|G|
    int32_t accMag = 0;
    int32_t accZTmp = 0;
    static int16_t accZoffset = 0;
    int16_t deltaAngle[3];
    int16_t accData[3], gyroData[3];

    if (estPrevTime == 0) {
        estPrevTime = currentTime;
        return;
    }

    // unit: radian per bit, scaled by 2^16 for further multiplication
    // with a delta time of 3000 us, and GYRO scale of most gyros, scale = a little bit less than 1
    float scale = (currentTime - estPrevTime) * (GYRO_SCALE * 65536);
    estPrevTime = currentTime;

// calculate delta angle
#if SENSOR_ACC && SENSOR_GYRO
    imu->getAccData(accData, 3);
    imu->getGyroData(gyroData, 3);

    for (axis = 0; axis < 3; axis++) {
        // used to calculate later the magnitude of acc vector
        accMag += aa::mul(accData[axis], accData[axis]);
        // unit: radian scaled by 2^16
        // imu.gyroADC[axis] is 14 bit long, the scale factor ensure deltaGyroAngle16[axis] is still 14 bit long
        deltaAngle[axis] = gyroData[axis] * scale;
    }
#endif

    // we rotate the intermediate 32 bit vector with the radian vector (deltaAngle), scaled by 2^16
    // however, only the first 16 MSB of the 32 bit vector is used to compute the result
    // it is ok to use this approximation as the 16 LSB are used only for the complementary filter part
    aa::rotateV32(&estimatedGyroData, deltaAngle);
    aa::rotateV32(&estimatedMagData, deltaAngle);

    // Apply complimentary filter (Gyro drift correction)
#if SENSOR_MAG
    int16_t magData[3];
    mag->GetData(magData, 3);
#endif
    for (axis = 0; axis < 3; axis++) {
        // If accel magnitude >1.15G or <0.85G and ACC vector outside of the limit range => we neutralize the effect of accelerometers in the angle estimation.
        // To do that, we just skip filter, as EstV already rotated by Gyro
        if ((int16_t) (accMag >> 8) > (int16_t) (0.85 * ACC_1G_LSB * ACC_1G_LSB / 256) &&
            (int16_t) (accMag >> 8) < (int16_t) (1.15 * ACC_1G_LSB * ACC_1G_LSB / 256)) {
            estimatedGyroData.A32[axis] +=
                    (int32_t) (accData[axis] - estimatedGyroData.A16[2 * axis + 1]) << (16 - GYR_CMPF_FACTOR);
        }
#if SENSOR_MAG
        estimatedMagData.A32[axis] += (int32_t)(magData[axis] - estimatedMagData.A16[2 * axis + 1]) << (16 - GYR_CMPFM_FACTOR);
#endif
        accZTmp += aa::mul(accData[axis], estimatedGyroData.A16[2 * axis + 1]);
    }

//    if (estimatedGyroData.V16.Z > ACCZ_25DEG) {
//        smallAngle25 = 1;
//    } else {
//        smallAngle25 = 0;
//    }

    // Attitude of the estimated vector
    int32_t sqrtGyroXZ = aa::mul(estimatedGyroData.V16.X, estimatedGyroData.V16.X) +
                         aa::mul(estimatedGyroData.V16.Z, estimatedGyroData.V16.Z);
    invGyro = aa::invSqrt(sqrtGyroXZ + aa::mul(estimatedGyroData.V16.Y, estimatedGyroData.V16.Y));
    att.Angle[ROLL] = aa::atan2(estimatedGyroData.V16.X, estimatedGyroData.V16.Z);
    att.Angle[PITCH] = aa::atan2(estimatedGyroData.V16.Y, aa::invSqrt(sqrtGyroXZ) * sqrtGyroXZ);
    att.Heading = aa::atan2(
            aa::mul(estimatedMagData.V16.Z, estimatedGyroData.V16.X) -
            aa::mul(estimatedMagData.V16.X, estimatedGyroData.V16.Z),
            (estimatedMagData.V16.Y * sqrtGyroXZ - (aa::mul(estimatedMagData.V16.X, estimatedGyroData.V16.X) +
                                                    aa::mul(estimatedMagData.V16.Z, estimatedGyroData.V16.Z)) *
                                                   estimatedGyroData.V16.Y) *
            invGyro);

#if SENSOR_MAG
    // att.Heading += 地磁偏角 // set from GUI
#endif
    att.Heading /= 10;

    accZ = accZTmp * invGyro;
    if (!conf->arm) {
        accZoffset -= accZoffset >> 3;
        accZoffset += accZ;
    }
    accZ -= accZoffset >> 3;
}

void MWCAlgorithm::updateAltitude(uint32_t currentTime) {
    int16_t ct;
    int32_t cp, ccp;
    float gps, gts;
    imu->getBaroData(&ct, &cp, &ccp);
    imu->getBaroLogData(&gps, &gts);
    int32_t baroAlt =
            (log(ccp) - gps) * gts;
    alt.Alt = (alt.Alt * 7 + baroAlt) >> 3; // additional LPF to reduce baro noise (faster by 30 µs)

#if defined(TEST_ALTHOLD)
    alt.Alt = (alt.Alt * 4 + testAltBase * 4) >> 3;
#endif
}
