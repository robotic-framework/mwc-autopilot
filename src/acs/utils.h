//
// Created by 李翌文 on 2021/6/11.
//

#ifndef AUTOPILOT_UTILS_H
#define AUTOPILOT_UTILS_H

#include "global.h"
#include "type_def.h"

#define MultiS16X16to32(longRes, intIn1, intIn2) \
    asm volatile(                                \
        "clr r26 \n\t"                           \
        "mul %A1, %A2 \n\t"                      \
        "movw %A0, r0 \n\t"                      \
        "muls %B1, %B2 \n\t"                     \
        "movw %C0, r0 \n\t"                      \
        "mulsu %B2, %A1 \n\t"                    \
        "sbc %D0, r26 \n\t"                      \
        "add %B0, r0 \n\t"                       \
        "adc %C0, r1 \n\t"                       \
        "adc %D0, r26 \n\t"                      \
        "mulsu %B1, %A2 \n\t"                    \
        "sbc %D0, r26 \n\t"                      \
        "add %B0, r0 \n\t"                       \
        "adc %C0, r1 \n\t"                       \
        "adc %D0, r26 \n\t"                      \
        "clr r1 \n\t"                            \
        : "=&r"(longRes)                         \
        : "a"(intIn1),                           \
          "a"(intIn2)                            \
        : "r26")

namespace aa {
    int32_t __attribute__((noinline)) mul(int16_t a, int16_t b);

// Rotate Estimated vector(s) with small angle approximation, according to the gyro data
    void rotateV32(t_int32_t_vector *v, int16_t *delta);

//return angle , unit: 1/10 degree
    int16_t atan2(int32_t y, int32_t x);

    float invSqrt(float x);
}

#endif //AUTOPILOT_UTILS_H
