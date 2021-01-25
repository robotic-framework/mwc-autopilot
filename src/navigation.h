//
// Created by 李翌文 on 2021/1/11.
//

#ifndef AUTOPILOT_NAVIGATION_H
#define AUTOPILOT_NAVIGATION_H

#include "config.h"
#if GPS_ENABLED

#include "stdint.h"
#include "gps.h"

enum NavState {
    NAV_STATE_NONE = 0,
    NAV_STATE_RTH_START,
    NAV_STATE_RTH_ENROUTE,
    NAV_STATE_HOLD_INFINIT,
    NAV_STATE_HOLD_TIMED,
    NAV_STATE_WP_ENROUTE,
    NAV_STATE_PROCESS_NEXT,
    NAV_STATE_DO_JUMP,
    NAV_STATE_LAND_START,
    NAV_STATE_LAND_IN_PROGRESS,
    NAV_STATE_LANDED,
    NAV_STATE_LAND_SETTLE,
    NAV_STATE_LAND_START_DESCENT
};

enum GPSMode {
    GPS_MODE_NONE = 0,
    GPS_MODE_HOLD,
    GPS_MODE_RTH,
    GPS_MODE_NAV
};

class Navigation {

private:
    GPS *gps;
    NavState state;
    GPSMode mode;

    int32_t home[POS_LENGTH];
    int32_t prevPos[POS_LENGTH] = {0, 0};
    int32_t errorDistance[POS_LENGTH];
    uint16_t distanceToHome; // distance to home  - unit: meter
    int16_t directionToHome; // direction to home - unit: degree
    int16_t calcSpeed[POS_LENGTH] = {0, 0};

    bool isSetHome;
    float scaleLonDown;
    uint32_t prevTime;
    float deltaTime;

private:
    void calcLongitudeScaling(int32_t lat);

    void calcVelocityByDeltaTime(const int32_t *posLat, const int32_t *posLon);

    void getBearing(const int32_t *targetLat,
                    const int32_t *targetLon,
                    const int32_t *sourceLat,
                    const int32_t *sourceLon,
                    int32_t *bearing) const;

    static void getDistance(const int32_t *targetLat,
                            const int32_t *targetLon,
                            const int32_t *sourceLat,
                            const int32_t *sourceLon,
                            uint32_t *distance);

    void calcErrorDistance(const int32_t *targetLat,
                           const int32_t *targetLon,
                           const int32_t *sourceLat,
                           const int32_t *sourceLon);

    void navigation(const int32_t *posLat, const int32_t *posLon, uint32_t currentTime);

    void resetHome(const int32_t *posLat, const int32_t *posLon);

public:
    Navigation();

    void ResetHome();

    void UpdateGPS(uint32_t currentTime) { gps->Update(currentTime); }

    void Update(uint32_t currentTime);
};

#endif

#endif //AUTOPILOT_NAVIGATION_H
