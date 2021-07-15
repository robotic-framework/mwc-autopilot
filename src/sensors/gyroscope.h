#if !defined(GYROSCOPE_H_)
#define GYROSCOPE_H_

#include <stdint.h>
#include "i2c.h"
#include "config.h"
#include "type_def.h"

#define GYRO_ORIENTATION(X, Y, Z) \
  {                               \
    data[ROLL] = Y;               \
    data[PITCH] = -X;             \
    data[YAW] = -Z;               \
  }

class Gyroscope : public I2C
{
private:
  uint16_t calibrateSteps = 0;
  uint32_t offsetTotal[3];
  int16_t prevData[3] = {0, 0, 0};

  void _calibration();

protected:
  int16_t data[3];

  virtual void getRawData() = 0;

public:
  Gyroscope(uint8_t address) : I2C(address) {}

  void update(uint32_t currentTime);
  void calibration();
  bool isCalibrating();
  void getData(int16_t *buf, uint8_t length);
  void setData(int16_t *data, uint8_t length);
};

#endif // GYROSCOPE_H_
