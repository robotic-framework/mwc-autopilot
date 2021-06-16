#include "barometer.h"

void Barometer::Update(uint32_t currentTime) {
    this->getRawData(currentTime);

    if (calibrateSteps > 0) {
        LEDPIN_ON;
        calibration(currentTime);
        calibrateSteps--;
        if (calibrateSteps == 0) {
            LEDPIN_OFF;
        }
    }
}

void Barometer::Calibration(uint16_t steps) {
    calibrateSteps = steps;
}

void Barometer::calibration(uint32_t currentTime) {
    logBaroGroundPressureSum = log(ccp);
    baroGroundTemperatureScale = ((int32_t) ct + 27315) * 29.271267f;
}

bool Barometer::IsCalibrating() {
    return (calibrateSteps > 0);
}

uint16_t Barometer::GetUTData() {
    return ut.val;
}

uint32_t Barometer::GetUPData() {
    return up.val;
}

int16_t Barometer::GetCTData() {
    return ct;
}

int32_t Barometer::GetCPData() {
    return cp;
}

int32_t Barometer::GetCCPData() {
    return ccp;
}
