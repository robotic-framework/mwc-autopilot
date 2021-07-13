//
// Created by 李翌文 on 2021/7/13.
//

#include "serial_implement.h"

SerialImplement SerialSITL = SerialImplement();
SerialImplement SerialSITL1 = SerialImplement();
SerialImplement SerialSITL2 = SerialImplement();
SerialImplement SerialSITL3 = SerialImplement();

void SerialImplement::begin(unsigned long baud) {

}

void SerialImplement::end() {

}

int SerialImplement::available(void) {
    return 0;
}

int SerialImplement::read(void) {
    return 0;
}

uint8_t SerialImplement::write(const uint8_t *buffer, uint8_t size) {
    return 0;
}
