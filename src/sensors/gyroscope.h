#if !defined(GYROSCOPE_H_)
#define GYROSCOPE_H_

#include <stdint.h>
#include "i2c.h"
#include "../config.h"

#define GYRO_ORIENTATION(X, Y, Z) \
  {                               \
    data[ROLL] = Y;               \
    data[PITCH] = -X;             \
    data[YAW] = -Z;               \
  }

class Gyroscope : public I2C
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
  Gyroscope(uint8_t address) : I2C(address) {}

  void Update(uint32_t currentTime);
  void Calibration();
  void GetData(int16_t *buf, uint8_t length);
  void SetData(int16_t *data, uint8_t length);
};

#endif // GYROSCOPE_H_
