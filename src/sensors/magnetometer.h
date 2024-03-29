#if !defined(MAGNETOMETER_H_)
#define MAGNETOMETER_H_

#include <stdint.h>
#include "i2c.h"
#include "config.h"
#include "type_def.h"

#define MAG_ORIENTATION(X, Y, Z) \
    {                            \
        data[ROLL] = X;          \
        data[PITCH] = Y;         \
        data[YAW] = -Z;          \
    }

class Magnetometer : public I2C
{
private:
    bool calibrating;
    uint32_t calibrationTime = 0;

    void _calibration(uint32_t stepTime);

protected:
    int16_t data[3];
    float magGain[3] = {1.0, 1.0, 1.0};

    virtual void getRawData() = 0;

public:
    Magnetometer(uint8_t address) : I2C(address){};

    uint8_t update(uint32_t currentTime);
    void calibration();
    bool isCalibrating();
    void getData(int16_t *buf, uint8_t length);
};

#endif // MAGNETOMETER_H_
