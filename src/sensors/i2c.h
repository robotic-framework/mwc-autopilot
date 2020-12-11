#if !defined(I2C_H_)
#define I2C_H_

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>

enum RC
{
  ROLL,
  PITCH,
  YAW,
  THROTTLE,
  AUX1,
  AUX2,
  AUX3,
  AUX4,
  AUX5,
  AUX6,
  AUX7,
  AUX8
};

class I2C
{
public:
  I2C(uint8_t deviceAddress)
  {
    this->deviceAddress = deviceAddress;
  }

  void Init()
  {
    this->init();
  }

  uint8_t GetErrCode()
  {
    return errCode;
  }

protected:
  uint8_t deviceAddress;
  uint8_t errCode;

  void write(uint8_t innerAddress, uint8_t value);
  void read(uint8_t address, uint8_t num, uint8_t _buff[]);

  virtual void init() = 0;
};

#endif // I2C_H_
