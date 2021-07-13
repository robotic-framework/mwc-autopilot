//
// Created by 李翌文 on 2021/7/13.
//

#ifndef AUTOPILOT_WIRE_IMPLEMENT_H
#define AUTOPILOT_WIRE_IMPLEMENT_H

#include <stdint.h>

class WireImplement {
public:
    void begin();
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    uint8_t write(uint8_t);
    uint8_t write(const uint8_t *, uint8_t);
    int read(void);
    int available(void);
};

extern WireImplement Wire;

#endif //AUTOPILOT_WIRE_IMPLEMENT_H
