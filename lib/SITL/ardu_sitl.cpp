//
// Created by 李翌文 on 2021/7/13.
//

#include "ardu_sitl.h"
#include "../../src/config.h"

#if defined(SITL)
void pinMode(uint8_t pin, uint8_t mode) {}
unsigned long millis() {return 0;}
unsigned long micros() {return 0;}
void delay(unsigned long ms) {}
#endif