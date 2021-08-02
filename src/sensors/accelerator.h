#ifndef ACC_H_
#define ACC_H_

#include <stdint.h>
#include "i2c.h"
#include "config.h"
#include "type_def.h"

#define ACC_ORIENTATION(X, Y, Z) \
    {                            \
        data[ROLL] = -(X);         \
        data[PITCH] = -(Y);        \
        data[YAW] = Z;           \
    }

class Accelerator : public I2C {
private:
    uint16_t calibrateSteps = 0;
    uint32_t stepTime = 0;
    uint32_t offsetTotal[3];

    void _calibration(uint32_t stepTime);

protected:
    int16_t data[3];

    virtual void getRawData() = 0;

public:
    Accelerator(uint8_t address) : I2C(address) {};

    void update(uint32_t currentTime);

    void calibration();

    bool isCalibrating();

    void getData(int16_t *buf, uint8_t length);
};

#endif // ACC_H_
