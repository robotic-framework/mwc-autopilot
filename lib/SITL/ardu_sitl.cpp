//
// Created by 李翌文 on 2021/7/13.
//

#include "ardu_sitl.h"
#include <unistd.h>
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();

#if defined(SITL)
void pinMode(uint8_t pin, uint8_t mode) {}
unsigned long millis() {
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
}
unsigned long micros() {
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}
void delay(unsigned long ms) {
    usleep(ms * 1000);
}
#endif