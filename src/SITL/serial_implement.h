//
// Created by 李翌文 on 2021/7/13.
//

#ifndef AUTOPILOT_SERIAL_IMPLEMENT_H
#define AUTOPILOT_SERIAL_IMPLEMENT_H

#include <stdint.h>

class SerialImplement {
public:
    void begin(unsigned long baud);
    void end();

    int available(void);
    int read(void);
    uint8_t write(const uint8_t *buffer, uint8_t size);
    operator bool() { return true; }
};

extern SerialImplement SerialSITL;
extern SerialImplement SerialSITL1;
extern SerialImplement SerialSITL2;
extern SerialImplement SerialSITL3;

#endif //AUTOPILOT_SERIAL_IMPLEMENT_H
