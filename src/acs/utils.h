//
// Created by 李翌文 on 2021/6/11.
//

#ifndef AUTOPILOT_UTILS_H
#define AUTOPILOT_UTILS_H

#include <Arduino.h>
#include "../type_def.h"

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

int32_t __attribute__((noinline)) mul(int16_t a, int16_t b) {
    int32_t r;
    MultiS16X16to32(r, a, b); //r = (int32_t)a*b; without asm requirement
    return r;
}

// Rotate Estimated vector(s) with small angle approximation, according to the gyro data
void rotateV32(t_int32_t_vector *v, int16_t *delta) {
    int16_t X = v->V16.X;
    int16_t Y = v->V16.Y;
    int16_t Z = v->V16.Z;

    v->V32.Z -= mul(delta[ROLL], X) + mul(delta[PITCH], Y);
    v->V32.X += mul(delta[ROLL], Z) - mul(delta[YAW], Y);
    v->V32.Y += mul(delta[PITCH], Z) + mul(delta[YAW], X);
}

//return angle , unit: 1/10 degree
int16_t _atan2(int32_t y, int32_t x) {
    float z = y;
    int16_t a;
    uint8_t c;
    c = abs(y) < abs(x);
    if (c) {
        z = z / x;
    } else {
        z = x / z;
    }
    a = 2046.43 * (z / (3.5714 + z * z));
    if (c) {
        if (x < 0) {
            if (y < 0)
                a -= 1800;
            else
                a += 1800;
        }
    } else {
        a = 900 - a;
        if (y < 0)
            a -= 1800;
    }
    return a;
}

float invSqrt(float x) {
    union {
        int32_t i;
        float f;
    } conv;
    conv.f = x;
    conv.i = 0x5f1ffff9 - (conv.i >> 1);
    return conv.f * (1.68191409f - 0.703952253f * x * conv.f * conv.f);
}

#endif //AUTOPILOT_UTILS_H
