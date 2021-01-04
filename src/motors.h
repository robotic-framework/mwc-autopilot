#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include <Arduino.h>

class Motors
{
private:
    uint16_t motors[8] = {};
    int16_t pidOffset[3];
    int32_t prop;

    void writeMotors();
    void applyPID();
    void mixPID();

public:
    static uint8_t Pins[8];

    Motors();
    ~Motors();

    void Init();
    void UpdatePID(uint32_t currentTime);
    void UpdateMotors(uint32_t currentTime);
    void GetMotors(uint16_t *buf, uint8_t length);
    uint8_t GetMotorCount();
    void WriteMotorsTrottle(uint16_t throttle);
};

#endif /* MOTORS_H_ */
