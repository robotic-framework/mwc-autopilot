//
// Created by 李翌文 on 2021/6/15.
//

#include "controller.h"
#include "../global.h"
#include "attitude_algorithm/mwc_algorithm.h"
#include "../type_def.h"

extern Configuration conf;

void ACSController::Init() {
    switch (this->conf->raw.aaType) {
        case MWC:
            this->aa = new MWCAlgorithm(this->conf);
    }

    this->aa->InjectIMU(&imu);
}

void ACSController::UpdateAcc(uint32_t currentTime) {
    imu.UpdateAcc(currentTime);
}

void ACSController::UpdateGyro(uint32_t currentTime) {
    imu.UpdateGyro(currentTime);
}

void ACSController::UpdateMag(uint32_t currentTime) {
    imu.UpdateMag(currentTime);
}

void ACSController::UpdateBaro(uint32_t currentTime) {
    imu.UpdateBaro(currentTime);
}

void ACSController::UpdateSonar(uint32_t currentTime) {
    imu.UpdateSonar(currentTime);
}

void ACSController::UpdateAttitude(uint32_t currentTime) {
#if SENSOR_ACC && SENSOR_GYRO
    // check if is in calibration
#if SENSOR_ACC
    if (imu.IsAccCalibrating()) {
        return;
    }
#endif
#if SENSOR_GYRO
    if (imu.IsGyroCalibrating()) {
        return;
    }
#endif
#if SENSOR_MAG
    if (imu.IsMagCalibrating())
    {
        return;
    }
#endif

    aa->UpdateAttitude(currentTime);
#endif
}

void ACSController::UpdateAltitude(uint32_t currentTime) {
#if SENSOR_BARO
    if (imu.IsBaroCalibrating()) {
        return;
    }

    aa->UpdateAltitude(currentTime);
#endif
}

void ACSController::UpdatePID(uint32_t currentTime) {
    motors.UpdatePID(currentTime);
}

void ACSController::UpdateMotors(uint32_t currentTime) {
    motors.UpdateMotors(currentTime);
}

void ACSController::GetRawData(int16_t *buf, uint8_t length) {
    imu.GetRawData(buf, length);
}

void ACSController::GetAccData(int16_t *buf, uint8_t length) {
    imu.GetAccData(buf, length);
}

void ACSController::GetGyroData(int16_t *buf, uint8_t length) {
    imu.GetGyroData(buf, length);
}

void ACSController::GetMagData(int16_t *buf, uint8_t length) {
    imu.GetMagData(buf, length);
}

void ACSController::GetBaroData(int16_t *ct, int32_t *cp, int32_t *ccp) {
    imu.GetBaroData(ct, cp, ccp);
}

void ACSController::AccCalibration() {
    imu.AccCalibration();
}

void ACSController::MagCalibration() {
    imu.MagCalibration();
}

void ACSController::BaroCalibration() {
    imu.BaroCalibration();
}

void ACSController::GetAttitude(int16_t *buf, uint8_t length) {
    aa->GetAttitude(buf, length);
}

void ACSController::GetAltitude(int32_t *targetAlt) {
    aa->GetAltitude(targetAlt);
}

void ACSController::GetAltitude(int32_t *targetAlt, int16_t *targetVario) {
    aa->GetAltitude(targetAlt, targetVario);
}

#if defined(TEST_ALTHOLD)
void ACSController::SetTestAltBase(uint16_t a) {
    aa->SetTestAltBase(a);
}

#endif

void ACSController::GetMotors(uint16_t *buf, uint8_t length) {
    motors.GetMotors(buf, length);
}

uint8_t ACSController::GetMotorCount() {
    return motors.GetMotorCount();
}
