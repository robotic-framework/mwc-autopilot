#include "i2c.h"

void I2C::write(uint8_t innerAddress, uint8_t value)
{
  Wire.beginTransmission(deviceAddress);
  Wire.write(innerAddress);
  Wire.write(value);
  Wire.endTransmission();
}

void I2C::read(uint8_t address, uint8_t num, uint8_t _buff[])
{
  Wire.beginTransmission(this->deviceAddress); // start transmission to device
  Wire.write(address);                         // sends address to read from
  Wire.endTransmission();                      // end transmission

  Wire.beginTransmission(this->deviceAddress); // start transmission to device
  Wire.requestFrom(this->deviceAddress, num);  // request bytes from device

  int i = 0;
  while (Wire.available()) // device may send less than requested (abnormal)
  {
    _buff[i] = Wire.read(); // receive a byte
    i++;
  }
  if (i != num)
  {
    errCode = 1;
  }

  Wire.endTransmission(); // end transmission
}
