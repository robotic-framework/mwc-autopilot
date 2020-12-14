#include "HMC5883L.h"

void HMC5883L::init()
{
    bool biasErr = true;

    setScale(1.9);
    write(HMC5883L_CONFIG_REG_MODE, Measurement_SingleShot);
    delay(100);

    // Note that the  very first measurement after a gain change maintains the same gain as the previous setting.
    // The new gain setting is effective from the second measurement and on.
    getRawData(); // Get one sample, and discard it

    if (!biasCollect(0x10 + 1))
    {
        biasErr = false;
    }
    if (!biasCollect(0x10 + 2))
    {
        biasErr = false;
    }

    if (biasErr)
    {
        for (size_t i = 0; i < 3; i++)
        {
            magGain[i] = 820.0 * HMC5883L_X_SELF_TEST_GAUSS * 2.0 * 10.0 / magTotal[i];
        }
    }

    write(HMC5883L_CONFIG_REGA, 0x70);                       // Configuration Register A  -- 0111 0000  num samples: 8 ; output rate: 15Hz ; normal measurement mode
    write(HMC5883L_CONFIG_REGB, 0x20);                       // Configuration Register B  -- 0010 0000  configuration gain 1.3Ga
    write(HMC5883L_CONFIG_REG_MODE, Measurement_Continuous); // Mode register            -- 0000 0000  continuous Conversion Mode
    delay(100);
}

void HMC5883L::getRawData()
{
    read(HMC5883L_CONFIG_REG_DATA, HMC5883L_TO_READ, _buff);
    MAG_ORIENTATION(
        ((_buff[0] << 8) | _buff[1]),
        ((_buff[4] << 8) | _buff[5]),
        ((_buff[2] << 8) | _buff[3]));
}

bool HMC5883L::biasCollect(uint8_t bias)
{
    int16_t abs;

    write(HMC5883L_CONFIG_REGA, bias);
    for (size_t i = 0; i < 10; i++)
    {
        write(HMC5883L_CONFIG_REG_MODE, 0x01); // single measurement
        delay(100);
        getRawData();
        for (size_t axis = 0; axis < 3; axis++)
        {
            abs = abs(data[axis]);
            magTotal[axis] += abs;
            if ((int16_t)(1 << 12) < abs)
            {
                return false;
            }
        }
    }

    return true;
}

void HMC5883L::setScale(float gauss)
{
    uint8_t regValue = 0x00;
    if (gauss == 0.88)
    {
        regValue = 0x00;
    }
    else if (gauss == 1.3)
    {
        regValue = 0x01;
    }
    else if (gauss == 1.9)
    {
        regValue = 0x02;
    }
    else if (gauss == 2.5)
    {
        regValue = 0x03;
    }
    else if (gauss == 4.0)
    {
        regValue = 0x04;
    }
    else if (gauss == 4.7)
    {
        regValue = 0x05;
    }
    else if (gauss == 5.6)
    {
        regValue = 0x06;
    }
    else if (gauss == 8.1)
    {
        regValue = 0x07;
    }

    // Setting is in the top 3 bits of the register.
    regValue = regValue << 5;
    write(HMC5883L_CONFIG_REGB, regValue);
}
