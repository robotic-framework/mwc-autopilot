#ifndef ACC_H_
#define ACC_H_

#include <stdint.h>
#include "i2c.h"
#include "../config.h"

#define ACC_ORIENTATION(X, Y, Z) \
    {                            \
        data[ROLL] = -X;         \
        data[PITCH] = -Y;        \
        data[YAW] = Z;           \
    }

class Accelerator : public I2C
{
private:
    uint16_t calibrateSteps;
    uint32_t stepTime = 0;
    int16_t offset[3];
    uint32_t offsetTotal[3];

    void calibration(uint32_t stepTime);

protected:
    int16_t data[3];

    virtual void getRawData() = 0;

public:
    Accelerator(uint8_t address) : I2C(address){};

    void Update(uint32_t currentTime);
    void Calibration();
    bool IsCalibrating();
    void GetData(int16_t *buf, uint8_t length);
};

#endif // ACC_H_
