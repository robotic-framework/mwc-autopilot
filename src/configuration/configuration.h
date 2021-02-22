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
    bool baroMode;
    int32_t altHold;

    struct {
        PID pid[PIDITEMS];
        int16_t accOffset[3];
        int16_t gyroOffset[3];
        int16_t magOffset[3];
        uint8_t checksum;
    } raw{};

public:
    Configuration();

    void Load(uint8_t pIndex = 0);

    void Write(uint8_t pIndex = 0);

private:
    void loadDefault();
    static uint8_t calcChecksum(uint8_t *payload, uint8_t size);
};


#endif //AUTOPILOT_CONFIGURATION_H
