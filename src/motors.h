#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include <Arduino.h>

extern int32_t altHold;
extern bool baroMode;

class Motors
{
private:
    uint16_t motors[8] = {};
    int16_t pidOffset[3];
    int16_t pidOffsetAlt;
    int32_t prop;
    static int16_t errorAltI;

    void writeMotors();
    void applyPID(uint32_t currentTime);
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
