#if !defined(I2C_H_)
#define I2C_H_

#include "../definition.h"
#if defined(SITL)
#include "../../lib/SITL/wire_implement.h"
#else
#include <Wire.h>
#endif
#include <stdint.h>

enum I2CError
{
    OK,
    DATA_TO_LONG,
    ADDRESS_NACK,
    DATA_NACK,
    UNKNOWN_ERR,
    TIMEOUT,
    DATA_NOT_ENOUGH
};

class I2C
{
public:
    I2C(uint8_t deviceAddress)
    {
        this->deviceAddress = deviceAddress;
    }

    void init()
    {
        this->_init();
    }

    I2CError getErrCode()
    {
        return errCode;
    }

    static uint16_t getErrCount()
    {
        return errCount;
    }

protected:
    uint8_t deviceAddress;
    I2CError errCode;

    void write(uint8_t innerAddress, uint8_t value);
    void read(uint8_t address, uint8_t num, uint8_t _buff[]);

    uint8_t read8(uint8_t address);
    uint16_t read16(uint8_t address);
    uint32_t read32(uint8_t address);

    virtual void _init() = 0;

private:
    static uint16_t errCount;
};

#endif // I2C_H_
