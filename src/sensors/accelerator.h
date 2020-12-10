#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>
#include <Arduino.h>
#include "i2c.h"
#include "../config.h"

#define ACC_ORIENTATION(X, Y, Z) {data[ROLL] = -X; data[PITCH] = -Y; data[YAW] = Z;}

class Accelerator : public I2C
{
private:
  uint16_t calibrateSteps;
  int16_t offset[3];
  uint32_t offsetTotal[3];

  void calibration();

protected:
  int16_t data[3];

  virtual void getRawData() = 0;

public:
  Accelerator(uint8_t address) : I2C(address){};

  void Update();
  void Calibration();
  void GetData(int16_t *buf, uint8_t length);
};

#endif // SENSOR_H_
