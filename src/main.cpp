#include <Arduino.h>
#include "def.h"
#if !DEBUG
#include "protocol.h"
#else
#include "avr8-stub.h"
#endif

uint32_t currentTime = 0;
uint16_t previousTime = 0;

extern Motors motors;
extern IMU imu;
extern uint16_t cycleTime;

void setup()
{
#if DEBUG
    debug_init();
#endif
    LEDPIN_PINMODE
    motors.Init();
    imu.Init();

#if !DEBUG
    serialInit();
#endif
    loadDefaults();
}

void loop()
{
#if !DEBUG
    protocolHandler();
#endif
    while (1)
    {
        currentTime = micros();
        cycleTime = currentTime - previousTime;
#if defined(LOOP_TIME)
        if (cycleTime >= LOOP_TIME)
            break;
#else
        break;
#endif
    }
    previousTime = currentTime;
    
    imu.Update(currentTime);
    motors.Update();
}