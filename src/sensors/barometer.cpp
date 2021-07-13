#include "barometer.h"
#include "../global.h"

extern Configuration conf;

void Barometer::update(uint32_t currentTime) {
    this->getRawData(currentTime);

    if (calibrateSteps > 0) {
        LEDPIN_ON;
        _calibration(currentTime);
        calibrateSteps--;
        if (calibrateSteps == 0) {
            LEDPIN_OFF;
        }
    }
}

void Barometer::calibration(uint16_t steps) {
    calibrateSteps = steps;
}

void Barometer::_calibration(uint32_t stepTime) {
    conf.raw.logBaroGroundPressureSum = log(ccp);
    conf.raw.baroGroundTemperatureScale = ((int32_t) ct + 27315) * 29.271267f;
}

bool Barometer::isCalibrating() {
    return (calibrateSteps > 0);
}

uint16_t Barometer::getUTData() {
    return ut.val;
}

uint32_t Barometer::getUPData() {
    return up.val;
}

int16_t Barometer::getCTData() {
    return ct;
}

int32_t Barometer::getCPData() {
    return cp;
}

int32_t Barometer::getCCPData() {
    return ccp;
}
