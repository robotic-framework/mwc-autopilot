//
// Created by 李翌文 on 2021/7/30.
//

#ifndef AUTOPILOT_ATTITUDE_ALGORITHM_IMPLEMENT_H
#define AUTOPILOT_ATTITUDE_ALGORITHM_IMPLEMENT_H

#include "../../src/acs/attitude_algorithm/attitude_algorithm.h"

class AAImpl : public AttitudeAlgorithm {
    void updateAttitude(uint32_t currentTime) override;

    void updateAltitude(uint32_t currentTime) override;
};


#endif //AUTOPILOT_ATTITUDE_ALGORITHM_IMPLEMENT_H
