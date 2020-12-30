#include <Arduino.h>
#include "def.h"
#include "tasks/tasks.h"
#if !DEBUG
#include "protocol.h"
#else
#include "avr8-stub.h"
#endif

uint32_t currentTime = 0;
uint16_t previousTime = 0;

Tasks scheduler;
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
    currentTime = micros();
    cycleTime = currentTime - previousTime;
    previousTime = currentTime;
    scheduler.Schedule();
}