//
// Created by 李翌文 on 2021/6/11.
//

#ifndef AUTOPILOT_MWC_ALGORITHM_H
#define AUTOPILOT_MWC_ALGORITHM_H

#include "attitude_algorithm.h"
#include "../../definition.h"
#include "../../configuration/configuration.h"

/* Set the Low Pass Filter factor for ACC
   Increasing this value would reduce ACC noise (visible in GUI), but would increase ACC lag time
   Comment this if  you do not want filter at all.
   unit = n power of 2 */
// this one is also used for ALT HOLD calculation, should not be changed
#ifndef ACC_LPF_FACTOR
#define ACC_LPF_FACTOR 4 // that means a LPF of 16
#endif

/* Set the Gyro Weight for Gyro/Acc complementary filter
   Increasing this value would reduce and delay Acc influence on the output of the filter*/
#ifndef GYR_CMPF_FACTOR
#define GYR_CMPF_FACTOR 10 //  that means a CMP_FACTOR of 1024 (2^10)
#endif

/* Set the Gyro Weight for Gyro/Magnetometer complementary filter
   Increasing this value would reduce and delay Magnetometer influence on the output of the filter*/
#define GYR_CMPFM_FACTOR 8 // that means a CMP_FACTOR of 256 (2^8)

class MWCAlgorithm : public AttitudeAlgorithm {
private:
    Configuration *conf;
    uint32_t estPrevTime;

    t_int32_t_vector estimatedGyroData;
    t_int32_t_vector estimatedMagData;

#if defined(TEST_ALTHOLD)
    int32_t testAltBase;
#endif

public:
    void UpdateAttitude(uint32_t currentTime) final;

    void UpdateAltitude(uint32_t currentTime) final;

    #if defined(TEST_ALTHOLD)
    void SetTestAltBase(uint16_t a);
    #endif

public:
    MWCAlgorithm(Configuration *conf);
};


#endif //AUTOPILOT_MWC_ALGORITHM_H
