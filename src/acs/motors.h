#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include <Arduino.h>
#include "pid.h"
#include "../configuration/configuration.h"

class Motors
{
private:
    Configuration *conf;
    PIDController *pid;
    uint16_t motors[8] = {};

    void writeMotors();
    void mixPID();

public:
    static uint8_t Pins[8];

    Motors(Configuration *conf);
    ~Motors();

    void Init();
    void UpdatePID(uint32_t currentTime);
    void UpdateMotors(uint32_t currentTime);
    void GetMotors(uint16_t *buf, uint8_t length);
    uint8_t GetMotorCount();
    void WriteMotorsThrottle(uint16_t throttle);
};

#endif /* MOTORS_H_ */
