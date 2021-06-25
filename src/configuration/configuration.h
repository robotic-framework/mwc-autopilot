//
// Created by 李翌文 on 2021/2/4.
//

#ifndef AUTOPILOT_CONFIGURATION_H
#define AUTOPILOT_CONFIGURATION_H

#include "type_def.h"

#define MAX_PROFILES 10

class Configuration {
public:
    bool arm;
    bool angleMode;
    bool horizonMode;
    bool altHoldMode;
    int32_t altHold;

    struct {
        AAType aaType;
        PID pid[PIDITEMS];
        int16_t accOffset[3] = {0, 0, 0};
        int16_t gyroOffset[3] = {0, 0, 0};
        int16_t magOffset[3] = {0, 0, 0};
        uint8_t checksum = 0;
    } raw{};

public:
    Configuration();

    void load(uint8_t pIndex = 0);

    void write(uint8_t pIndex = 0);

private:
    void loadDefault();

    static uint8_t calcChecksum(uint8_t *payload, uint8_t size);
};


#endif //AUTOPILOT_CONFIGURATION_H
