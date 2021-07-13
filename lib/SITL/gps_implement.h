//
// Created by 李翌文 on 2021/7/13.
//

#ifndef AUTOPILOT_GPS_IMPLEMENT_H
#define AUTOPILOT_GPS_IMPLEMENT_H

#include <stdint.h>

struct TinyGPSLocation {
public:
    bool isValid() const { return valid; }

    int32_t lat32();

    int32_t lng32();

private:
    bool valid;
};

struct TinyGPSSpeed {
public:
    bool isValid() const { return valid; }

    double mps() { return 0.0; }

private:
    bool valid;
};

struct TinyGPSCourse {
public:
    bool isValid() const { return valid; }

    double deg() { return 0.0; }

private:
    bool valid;
};

struct TinyGPSAltitude {
public:
    bool isValid() const { return valid; }

    int32_t value() { return 0; }

private:
    bool valid;
};

struct TinyGPSInteger {
public:
    bool isValid() const { return valid; }

    uint32_t value() { return 0; }

private:
    bool valid;
};

class TinyGPSPlus {
public:
    bool encode(char c); // process one character received from GPS

    TinyGPSLocation location;
    TinyGPSSpeed speed;
    TinyGPSCourse course;
    TinyGPSAltitude altitude;
    TinyGPSInteger satellites;

    uint32_t sentencesWithFix() const { return sentencesWithFixCount; }

private:
    uint32_t sentencesWithFixCount;
};


#endif //AUTOPILOT_GPS_IMPLEMENT_H
