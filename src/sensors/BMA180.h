#ifndef BMA180_H_
#define BMA180_H_

#include <stdint.h>
#include "accelerator.h"

#define BMA180_DEVICE 0x40 // BMA180 device address
#define BMA180_TO_READ 6   // num of bytes we are going to read each time (two bytes for each axis)
#define BMA180_DATAX0 0x02

class BMA180 : public Accelerator
{
private:
    /* data */
    uint8_t _buff[BMA180_TO_READ];

protected:
    void getRawData() override;
    void init() override;

public:
    BMA180(uint8_t address) : Accelerator(address) {}
};

#endif // BMA180_H_
