#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include "pid/pid.h"
#include "../configuration/configuration.h"

class Motors
{
private:
    Configuration *conf;
    uint16_t motors[8] = {};

    void writeMotors();

public:
    static uint8_t Pins[8];

    Motors(Configuration *conf);
    ~Motors();

    void init();
    void mixPID(PIDController *pid);
    void updateMotors(uint32_t currentTime);
    void getMotors(uint16_t *buf, uint8_t length);
    static uint8_t getMotorCount();
    void writeMotorsThrottle(uint16_t throttle);
};

#endif /* MOTORS_H_ */
