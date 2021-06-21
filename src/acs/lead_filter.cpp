//
// Created by 李翌文 on 2021/2/25.
//

#include "lead_filter.h"

int32_t LeadFilter::getPosition(int32_t pos, int16_t vel, float lag_in_seconds) {

    int16_t accel_contribution = (vel - _lastVelocity) * lag_in_seconds * lag_in_seconds;
    int16_t vel_contribution = vel * lag_in_seconds;

    // store velocity for next iteration
    _lastVelocity = vel;

    return pos + vel_contribution + accel_contribution;
}
