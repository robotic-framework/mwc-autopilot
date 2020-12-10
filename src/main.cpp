#include <avr/io.h>
#include <Arduino.h>
#include "def.h"
#include "protocol.h"

uint32_t currentTime = 0;
uint16_t previousTime = 0;
uint16_t cycleTime = 0;

extern Motors motors;
extern IMU imu;

void setup()
{
  LEDPIN_PINMODE
  
  motors.Init();
  serialInit();

  imu.Init();
}

void loop()
{
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
  imu.Update();
  protocolHandler();
}