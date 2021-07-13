//
// Created by 李翌文 on 2021/7/13.
//

#ifndef AUTOPILOT_GPS_SITL_H
#define AUTOPILOT_GPS_SITL_H

#if defined(SITL)
#include "gps_implement.h"
#else
#include "TinyGPS++.h"
#endif

#endif //AUTOPILOT_GPS_SITL_H
