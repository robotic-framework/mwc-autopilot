//
// Created by 李翌文 on 2021/1/8.
//

#ifndef AUTOPILOT_GPS_H
#define AUTOPILOT_GPS_H

#include "definition.h"
#if GPS_ENABLED

#include <stdint.h>
#include "TinyGPS++.h"

enum GPSPos {
    LAT,
    LON,
    POS_LENGTH
};

class GPS {
private:
    TinyGPSPlus *gps;

    bool hasFix;
    uint8_t satellites = 0;
    int32_t pos[POS_LENGTH];
    uint16_t altitude;
    uint16_t groundSpeed; // cm/s
    uint16_t groundCourse; // deg * 10

    bool isNewData = false;

public:
    GPS();

    void Init();

    void Update(uint32_t currentTime);

    bool AvailableThen() {
        if (isNewData) {
            isNewData = false;
            return true;
        }
        return false;
    }

    bool HasFix() { return hasFix; }

    uint8_t GetGPSSatellites() { return satellites; }

    void GetGPSPos(int32_t *lat, int32_t *lon) {
        *lat = pos[LAT];
        *lon = pos[LON];
    }

    uint16_t GetGPSAltitude() { return altitude; }

    uint16_t GetGroundSpeed() { return groundSpeed; }

    uint16_t GetGroundCourse() { return groundCourse; }
};

#endif

#endif //AUTOPILOT_GPS_H
