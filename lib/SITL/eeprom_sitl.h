//
// Created by 李翌文 on 2021/7/13.
//

#ifndef AUTOPILOT_EEPROM_SITL_H
#define AUTOPILOT_EEPROM_SITL_H

#if defined(SITL)
#include "eeprom_implement.h"
#else
#include "../../../../../../.platformio/packages/framework-arduino-avr/libraries/EEPROM/src/EEPROM.h"
#endif

#endif //AUTOPILOT_EEPROM_SITL_H
