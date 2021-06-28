#include <Arduino.h>
#include "global.h"
#include "tasks/tasks.h"
#if !DEBUG
#include "communication/protocol.h"
#else
#include "avr8-stub.h"
#endif

uint32_t currentTime = 0;
uint16_t previousTime = 0;

Tasks scheduler;
extern ACSController acs;
extern uint16_t cycleTime;
extern Configuration conf;

#if GPS_ENABLED
extern Navigation nav;
#endif

void setup()
{
#if DEBUG
    debug_init();
#else
    serialInit();
#endif
    LEDPIN_PINMODE

    acs.init();

#if !DEBUG && GPS_ENABLED
    nav.init();
#endif

    conf.load(0);
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