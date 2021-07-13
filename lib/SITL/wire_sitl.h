//
// Created by 李翌文 on 2021/7/13.
//

#ifndef AUTOPILOT_WIRE_SITL_H
#define AUTOPILOT_WIRE_SITL_H

#if defined(SITL)
#include "wire_implement.h"
#else
#include "../../../../../../.platformio/packages/framework-arduino-avr/libraries/Wire/src/Wire.h"
#endif

#endif //AUTOPILOT_WIRE_SITL_H
