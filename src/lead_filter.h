//
// Created by 李翌文 on 2021/2/25.
//

#ifndef AUTOPILOT_LEADFILTER_H
#define AUTOPILOT_LEADFILTER_H

#include "stdint.h"

class LeadFilter {
public:
    LeadFilter() : _lastVelocity(0) {}

    // setup min and max radio values in CLI
    int32_t GetPosition(int32_t pos, int16_t vel, float lag_in_seconds = 1.0);

    void Clear() { _lastVelocity = 0; }

private:
    int16_t _lastVelocity;

};


#endif //AUTOPILOT_LEADFILTER_H
