//
// Created by 李翌文 on 2021/8/2.
//

#ifndef AUTOPILOT_MOTORS_IMPLEMENT_H
#define AUTOPILOT_MOTORS_IMPLEMENT_H

#include "acs/motors.h"

class MotorSITLImpl : public Motors {
public:
    explicit MotorSITLImpl(Configuration *c);

    void init() override;

    void mixPID(PIDController *pid) override;

    void updateMotors(uint32_t currentTime) override;

    void getMotors(uint16_t *buf, uint8_t length) override;

    void writeMotorsThrottle(uint16_t throttle) override;
};


#endif //AUTOPILOT_MOTORS_IMPLEMENT_H
