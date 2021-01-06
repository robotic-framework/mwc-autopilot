#if !defined(MEGANETOMETER_H_)
#define MEGANETOMETER_H_

#include <stdint.h>
#include "i2c.h"
#include "../config.h"

#define MAG_ORIENTATION(X, Y, Z) \
    {                            \
        data[ROLL] = X;          \
        data[PITCH] = Y;         \
        data[YAW] = -Z;          \
    }

class Meganetometer : public I2C
{
private:
    bool calibrating;
    uint32_t stepTime = 0;
    uint32_t calibrationTime = 0;
    int16_t offset[3];

    void calibration(uint32_t stepTime);

protected:
    int16_t data[3];
    float magGain[3] = {1.0, 1.0, 1.0};

    virtual void getRawData() = 0;

public:
    Meganetometer(uint8_t address) : I2C(address){};

    uint8_t Update(uint32_t currentTime);
    void Calibration();
    bool IsCalibrating();
    void GetData(int16_t *buf, uint8_t length);
};

#endif // MEGANETOMETER_H_
