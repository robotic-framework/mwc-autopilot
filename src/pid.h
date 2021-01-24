#if !defined(_PID_H_)
#define _PID_H_

#include "stdint.h"
#include "def.h"

extern int32_t altHold;
extern bool baroMode;
extern int16_t rcCommand[4];

class PIDController
{
private:
    int16_t pidOffset[3];
    int32_t prop;
    static int16_t errorAltI;
    uint8_t lastBaroMode;
    uint8_t isBaroModeChanged;
    int16_t initialThrottleHold;

public:
    PIDController();

    void Update(uint32_t currentTime);
    uint16_t MixPID(int8_t x, int8_t y, int8_t z);
};

#endif
