//
// Created by 李翌文 on 2021/2/4.
//

#ifndef AUTOPILOT_CONFIGURATION_H
#define AUTOPILOT_CONFIGURATION_H

#include "definition.h"
#include "type_def.h"

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
        float logBaroGroundPressureSum = 0, baroGroundTemperatureScale = 0;
        uint8_t checksum = 0;
    } raw{};

    struct {
        uint8_t wpCount = 0;
        Waypoint wpCache[20];
        uint8_t wpCacheCount = 0;
    } waypoints{};

public:
    Configuration();

    void load(uint8_t pIndex = 0);

    void write(uint8_t pIndex = 0);

    void loadWaypoint();

    void addWaypoint(Waypoint &wp);

    void writeWaypoint();

    void clearWaypoint();

    void getWaypoint(uint8_t index, Waypoint *wp);

private:
    void loadDefault();

    static uint8_t calcChecksum(uint8_t *payload, uint8_t size);
};


#endif //AUTOPILOT_CONFIGURATION_H
