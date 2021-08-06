//
// Created by 李翌文 on 2021/8/2.
//

#include "motors_implement.h"
#include "networks.h"

MotorSITLImpl::MotorSITLImpl(Configuration *c) : Motors(c) {}

void MotorSITLImpl::init() {

}

void MotorSITLImpl::mixPID(PIDController *pid) {
    if (!conf->arm) {
        return;
    }

    uint16_t maxMotor;
    uint8_t i;

    motors[0] = pid->mixPID(-1, 1, -1);
    motors[1] = pid->mixPID(-1, -1, 1);
    motors[2] = pid->mixPID(1, 1, 1);
    motors[3] = pid->mixPID(1, -1, -1);

    maxMotor = motors[0];
    for (i = 0; i < MOTOR_COUNT; i++) {
        if (motors[i] > maxMotor) {
            maxMotor = motors[i];
        }
    }
    for (i = 0; i < MOTOR_COUNT; i++) {
        if (maxMotor > MAXTHROTTLE) {
            motors[i] -= maxMotor - MAXTHROTTLE;
        }
        motors[i] = constrain(motors[i], MINTHROTTLE, MAXTHROTTLE);

        if (!conf->arm) {
            motors[i] = MINCOMMAND;
        }
    }
}

void MotorSITLImpl::updateMotors(uint32_t currentTime) {
    msg_request_control msg;
    read((uint8_t *)msg.motors, (uint8_t *)motors, 16);
    request(&msg);
}

void MotorSITLImpl::getMotors(uint16_t *buf, uint8_t length) {

}

void MotorSITLImpl::writeMotorsThrottle(uint16_t throttle) {

}
