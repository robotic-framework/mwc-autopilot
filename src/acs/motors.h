//
// Created by 李翌文 on 2021/8/2.
//

#ifndef AUTOPILOT_MOTORS_H
#define AUTOPILOT_MOTORS_H

#include <stdint.h>
#include "acs/pid/pid.h"

class Motors {
protected:
    Configuration *conf;
    uint16_t motors[8] = {0, 0, 0, 0, 0, 0, 0, 0};

public:
    explicit Motors(Configuration *c): conf(c) {};
    virtual void init() = 0;
    virtual void mixPID(PIDController *pid) = 0;
    virtual void updateMotors(uint32_t currentTime) = 0;
    virtual void getMotors(uint16_t *buf, uint8_t length) = 0;
    virtual void writeMotorsThrottle(uint16_t throttle) = 0;

    static uint8_t getMotorCount() {
        return MOTOR_COUNT;
    };
};

#endif //AUTOPILOT_MOTORS_H
