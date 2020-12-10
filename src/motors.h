#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include <Arduino.h>
#include "config.h"

class Motors
{
private:
  uint16_t motors[8] = {};
  uint8_t motorCount;
  void writeMotors();

public:
  static uint8_t Pins[8];
  
  Motors(uint8_t motorCount);
  ~Motors();

  void Init();
  uint16_t *GetMotors();
  uint8_t GetMotorCount();
  void WriteMotorsTrottle(uint16_t throttle);
};

#endif /* MOTORS_H_ */
