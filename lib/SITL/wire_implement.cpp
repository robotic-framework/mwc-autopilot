//
// Created by 李翌文 on 2021/7/13.
//

#include "wire_implement.h"

WireImplement Wire = WireImplement();

void WireImplement::begin() {

}

void WireImplement::beginTransmission(uint8_t) {

}

void WireImplement::beginTransmission(int) {

}

uint8_t WireImplement::endTransmission(void) {
    return 0;
}

uint8_t WireImplement::endTransmission(uint8_t) {
    return 0;
}

uint8_t WireImplement::requestFrom(uint8_t, uint8_t) {
    return 0;
}

uint8_t WireImplement::requestFrom(uint8_t, uint8_t, uint8_t) {
    return 0;
}

uint8_t WireImplement::requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t) {
    return 0;
}

uint8_t WireImplement::requestFrom(int, int) {
    return 0;
}

uint8_t WireImplement::requestFrom(int, int, int) {
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
