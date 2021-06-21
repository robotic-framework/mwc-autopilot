//
// Created by 李翌文 on 2021/6/11.
//

#ifndef AUTOPILOT_MWC_ALGORITHM_H
#define AUTOPILOT_MWC_ALGORITHM_H

#include "attitude_algorithm.h"
#include "../../definition.h"
#include "../../configuration/configuration.h"

class MWCAlgorithm : public AttitudeAlgorithm {
private:
    Configuration *conf;
    uint32_t estPrevTime;

    t_int32_t_vector estimatedGyroData;
    t_int32_t_vector estimatedMagData;

public:
    void updateAttitude(uint32_t currentTime) final;

    void updateAltitude(uint32_t currentTime) final;

public:
    MWCAlgorithm(Configuration *conf);
};


#endif //AUTOPILOT_MWC_ALGORITHM_H
