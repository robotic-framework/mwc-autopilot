#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include <Arduino.h>

#include "config.h"

void blinkLED(uint8_t num, uint8_t ontime,uint8_t repeat);

#endif // LED_H_
