//
// Created by 李翌文 on 2021/7/13.
//

#include "wire_implement.h"

WireImplement Wire = WireImplement();

void WireImplement::begin() {

}

void WireImplement::beginTransmission(uint8_t addr) {
    beginTransmission((int) addr);
}

void WireImplement::beginTransmission(int addr) {
    if (addr == ADXL345_DEVICE || addr == BMA180_DEVICE) {
        currentDevice = DEVICE_ACC;
    } else if (addr == BMP085_DEVICE) {
        currentDevice = DEVICE_BAROMETER;
    } else if (addr == HMC5883L_DEVICE) {
        currentDevice = DEVICE_MAG;
    } else if (addr == ITG3205_DEVICE) {
        currentDevice = DEVICE_GYRO;
    }
}

uint8_t WireImplement::endTransmission(void) {
    return 0;
}

uint8_t WireImplement::requestFrom(uint8_t, uint8_t) {
    return 0;
}

uint8_t WireImplement::write(uint8_t) {
    return 0;
}

uint8_t WireImplement::write(const uint8_t *, uint8_t) {
    return 0;
}

int WireImplement::available(void) {
    return 0;
}

int WireImplement::read(void) {
    return 0;
}
