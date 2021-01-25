//
// Created by 李翌文 on 2021/1/11.
//

#if GPS_ENABLED

#include "Arduino.h"
#include "navigation.h"
#include "def.h"

extern bool arm;

Navigation::Navigation()
{
    gps = new GPS;
}

void Navigation::Update(uint32_t currentTime)
{
    if (!gps->AvailableThen())
    {
        return;
    }

    if (gps->HasFix() && gps->GetGPSSatellites() >= 5)
    {
        int32_t posLat, posLon;
        gps->GetGPSPos(&posLat, &posLon);

        // auto set home position
        if (!isSetHome && arm)
        {
            resetHome(&posLat, &posLon);
        }

        // calculate distance and bearings for gui and other stuff continously - From home to copter
        uint32_t dist; //temp variable to store dist to copter
        int32_t dir;   //temp variable to store dir to copter
        getBearing(&posLat, &posLon, &home[LAT], &home[LON], &dir);
        getDistance(&posLat, &posLon, &home[LAT], &home[LON], &dist);
        if (isSetHome)
        {
            distanceToHome = dist / 100;
            directionToHome = dir / 100;
        }
        else
        {
            distanceToHome = 0;
            directionToHome = 0;
        }

        uint32_t currentMillis = millis();
        if (prevTime == 0)
        {
            prevTime = currentMillis; // prevTime in millisecond.
            return;
        }

        deltaTime = (float)(currentMillis - prevTime) / 1000.f;
        prevTime = currentMillis;

        deltaTime = min(deltaTime, 1.0);
        calcVelocityByDeltaTime(&posLat, &posLon);
        navigation(&posLat, &posLon, currentTime);
    }
}

void Navigation::navigation(const int32_t *posLat, const int32_t *posLon, uint32_t currentTime)
{
    if (mode == GPS_MODE_NONE)
    {
        return;
    }
}

void Navigation::getBearing(const int32_t *targetLat,
                            const int32_t *targetLon,
                            const int32_t *sourceLat,
                            const int32_t *sourceLon,
                            int32_t *bearing) const
{
    int32_t offX = *sourceLon - *targetLon;
    int32_t offY = (*sourceLat - *targetLat) / scaleLonDown;

    *bearing = 9000 + atan2(-offY, offX) * 5729.57795f; //Convert the output radians to 100*deg
    if (*bearing < 0)
    {
        *bearing += 36000;
    }
}

void Navigation::getDistance(const int32_t *targetLat,
                             const int32_t *targetLon,
                             const int32_t *sourceLat,
                             const int32_t *sourceLon,
                             uint32_t *dist)
{
    float dLat = (float)(*sourceLat - *targetLat);
    float dLon = (float)(*sourceLon - *targetLon);
    *dist = sqrt(sq(dLat) + sq(dLon)) * 1.11318845f;
}

void Navigation::calcErrorDistance(const int32_t *targetLat,
                                   const int32_t *targetLon,
                                   const int32_t *sourceLat,
                                   const int32_t *sourceLon)
{
    errorDistance[LAT] = *targetLat - *sourceLat;
    errorDistance[LON] = (float)(*targetLon - *sourceLon) * scaleLonDown;
}

void Navigation::calcLongitudeScaling(int32_t lat)
{
    scaleLonDown = cos(lat * 1.0e-7f * 0.01745329251f);
}

void Navigation::calcVelocityByDeltaTime(const int32_t *posLat, const int32_t *posLon)
{
    static int16_t velOld[2] = {0, 0};
    if (prevPos[LAT] && prevPos[LON])
    {
        calcSpeed[LON] = (float)(*posLon - prevPos[LON]) * scaleLonDown / deltaTime;
        calcSpeed[LAT] = (float)(*posLat - prevPos[LAT]) / deltaTime;
    }

    prevPos[LAT] = *posLat;
    prevPos[LON] = *posLon;
}

void Navigation::ResetHome()
{
    int32_t posLat, posLon;
    gps->GetGPSPos(&posLat, &posLon);

    resetHome(&posLat, &posLon);
}

void Navigation::resetHome(const int32_t *posLat, const int32_t *posLon)
{
    home[LAT] = *posLat;
    home[LON] = *posLon;
    calcLongitudeScaling(*posLat);
    isSetHome = true;
}

#endif
