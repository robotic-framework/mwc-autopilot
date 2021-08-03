#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include "motors.h"

class MotorImpl : public Motors {
private:
    void writeMotors();

public:
    static uint8_t Pins[8];

    explicit MotorImpl(Configuration *conf);

    ~MotorImpl();

    void init() override;

    void mixPID(PIDController *pid) override;

    void updateMotors(uint32_t currentTime) override;

    void getMotors(uint16_t *buf, uint8_t length) override;

    void writeMotorsThrottle(uint16_t throttle) override;
};

#endif /* MOTORS_H_ */
