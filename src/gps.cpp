//
// Created by 李翌文 on 2021/1/8.
//

#include "config.h"
#if GPS_ENABLED

#include "gps.h"
#include "led.h"

GPS::GPS() : hasFix(false) {
    gps = new TinyGPSPlus;
}

void GPS::Init() {
    GPS_SERIAL_DEVICE.begin(GPS_BAUD);
    while (!GPS_SERIAL_DEVICE) {}
    blinkLED(2, 20, 10);
    LEDPIN_OFF
}

void GPS::Update(uint32_t currentTime) {
    while (GPS_SERIAL_DEVICE.available()) {
        gps->encode(GPS_SERIAL_DEVICE.read());
    }

    if (gps->satellites.isValid() &&
        gps->location.isValid() &&
        gps->altitude.isValid() &&
        gps->speed.isValid() &&
        gps->course.isValid()) {
        satellites = gps->satellites.value();

        pos[LAT] = gps->location.lat();
        pos[LON] = gps->location.lng();

        if (gps->sentencesWithFix() > 0) {
            hasFix = true;
        } else {
            hasFix = false;
        }

        altitude = gps->altitude.value();
        groundSpeed = gps->speed.mps() * 100;
        groundCourse = gps->course.deg() * 10;

        isNewData = true;
    }
}

#endif