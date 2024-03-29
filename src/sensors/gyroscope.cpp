#include "gyroscope.h"
#include "global.h"

void Gyroscope::update(uint32_t currentTime)
{
    this->getRawData();
    if (calibrateSteps > 0)
    {
        _calibration();
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        data[i] -= conf.raw.gyroOffset[i];
        data[i] = constrain(data[i], prevData[i] - 800, prevData[i] + 800);
        prevData[i] = data[i];
    }
}

void Gyroscope::calibration()
{
    calibrateSteps = 512;
    for (uint8_t i = 0; i < 3; i++)
    {
        offsetTotal[i] = 0;
    }
}

bool Gyroscope::isCalibrating()
{
    return (calibrateSteps > 0);
}

void Gyroscope::_calibration()
{
    LEDPIN_ON
    for (uint8_t i = 0; i < 3; i++)
    {
        offsetTotal[i] += data[i];
        conf.raw.gyroOffset[i] = offsetTotal[i] >> 9;
    }

    if (calibrateSteps == 1)
    {
        // _calibration done
        LEDPIN_OFF
    }
    calibrateSteps--;
}

void Gyroscope::getData(int16_t *buf, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        if (i > 2)
        {
            break;
        }

        *(buf + i) = this->data[i];
    }
}

void Gyroscope::setData(int16_t *data, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        if (i > 2)
        {
            break;
        }

        this->data[i] = *(data + i);
    }
}
