#include "i2c.h"

uint16_t I2C::errCount = 0;

void I2C::write(uint8_t innerAddress, uint8_t value)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(innerAddress);
    Wire.write(value);
    errCode = (I2CError)Wire.endTransmission();
    if (errCode != SUCCESS)
    {
        errCount++;
    }
}

void I2C::read(uint8_t address, uint8_t num, uint8_t _buff[])
{
    Wire.beginTransmission(this->deviceAddress); // start transmission to device
    Wire.write(address);                         // sends address to read from
    errCode = (I2CError)Wire.endTransmission();                      // end transmission
    if (errCode != SUCCESS)
    {
        errCount++;
        return;
    }

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
        errCode = DATA_NOT_ENOUGH;
    }

    errCode = (I2CError)Wire.endTransmission(); // end transmission
    if (errCode != SUCCESS)
    {
        errCount++;
        return;
    }
}
