#include "i2c.h"

uint16_t I2C::errCount = 0;

void I2C::write(uint8_t innerAddress, uint8_t value)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(innerAddress);
    Wire.write(value);
    errCode = (I2CError)Wire.endTransmission();
    if (errCode != OK)
    {
        errCount++;
    }
}

void I2C::read(uint8_t address, uint8_t num, uint8_t _buff[])
{
    Wire.beginTransmission(this->deviceAddress); // start transmission to device
    Wire.write(address);                         // sends address to read from
    errCode = (I2CError)Wire.endTransmission();                      // end transmission
    if (errCode != OK)
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
    if (errCode != OK)
    {
        errCount++;
        return;
    }
}

uint8_t I2C::read8(uint8_t address)
{
    uint8_t buff[1];
    read(address, 1, buff);
    return buff[0];
}

uint16_t I2C::read16(uint8_t address)
{
    uint8_t raw[2];
    
    read(address, 2, raw);
    return raw[1] | (raw[0] << 8);
}

uint32_t I2C::read32(uint8_t address)
{
    uint32_t val;
    uint8_t raw[4];

    read(address, 4, raw);
    return raw[3] | (raw[2] << 8) | (raw[1] << 16) | (raw[0] << 24);
}
