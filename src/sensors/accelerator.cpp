#include "accelerator.h"
#include "global.h"

extern Configuration conf;

void Accelerator::update(uint32_t currentTime) {
    this->getRawData();

    if (calibrateSteps > 0) {
        _calibration(currentTime);
    }
    for (uint8_t i = 0; i < 3; i++) {
        data[i] -= conf.raw.accOffset[i];
    }
}

void Accelerator::calibration() {
    calibrateSteps = 512;
    for (uint8_t i = 0; i < 3; i++) {
        offsetTotal[i] = 0;
    }
}

bool Accelerator::isCalibrating() {
    return (calibrateSteps > 0);
}

void Accelerator::_calibration(uint32_t stepTime) {
    LEDPIN_ON
    for (uint8_t i = 0; i < 3; i++) {
        offsetTotal[i] += data[i];
        conf.raw.accOffset[i] = offsetTotal[i] >> 9;
    }

    if (calibrateSteps == 1) {
        // _calibration done
        LEDPIN_OFF
        conf.raw.accOffset[YAW] -= ACC_1G_LSB;
    }
    calibrateSteps--;
}

void Accelerator::getData(int16_t *buf, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        if (i > 2) {
            break;
        }

        *(buf + i) = this->data[i];
    }
}
