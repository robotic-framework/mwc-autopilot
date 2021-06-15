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

    int16_t gyroData[3];
    int16_t accData[3];

    imu.GetGyroData(gyroData, 3);
    imu.GetAccData(accData, 3);

    aa->SetAccData(accData, 3);
    aa->SetGyroData(gyroData, 3);
    aa->UpdateAttitude(currentTime);
#endif
}

void ACSController::UpdateAltitude(uint32_t currentTime) {
#if SENSOR_BARO
    aa->UpdateAltitude(currentTime);
#endif
}

void ACSController::UpdatePID(uint32_t currentTime) {
    motors.UpdatePID(currentTime);
}

void ACSController::UpdateMotors(uint32_t currentTime) {
    motors.UpdateMotors(currentTime);
}

