#include "magnetometer.h"
#include "../global.h"

extern Configuration conf;

uint8_t Magnetometer::update(uint32_t currentTime)
{
    this->getRawData();

    for (uint8_t i = 0; i < 3; i++)
    {
        data[i] *= magGain[i];
        if (!calibrating)
        {
            data[i] -= conf.raw.magOffset[i];
        }
    }

    if (calibrating)
    {
        _calibration(currentTime);
    }

    return 1;
}

void Magnetometer::calibration()
{
    calibrating = true;
}

bool Magnetometer::isCalibrating()
{
    return calibrating;
}

void Magnetometer::_calibration(uint32_t stepTime)
{
    static int16_t magZeroTempMin[3], magZeroTempMax[3];

    if (calibrationTime == 0)
    {
        calibrationTime = stepTime;
    }

    // 30s: you have 30s to turn the multi in all directions
    if ((stepTime - calibrationTime) < 30000000)
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            LEDPIN_TOGGLE;
            // it happens only in the first step, initialize the zero
            if (stepTime == calibrationTime)
            {
                magZeroTempMin[i] = data[i];
                magZeroTempMax[i] = data[i];
            }

            if (data[i] < magZeroTempMin[i])
            {
                magZeroTempMin[i] = data[i];
            }
            if (data[i] > magZeroTempMax[i])
            {
                magZeroTempMax[i] = data[i];
            }
            conf.raw.magOffset[i] = (magZeroTempMin[i] + magZeroTempMax[i]) >> 1;
        }
    }
    else
    {
        calibrating = false;
        calibrationTime = 0;
        LEDPIN_OFF
    }
}

void Magnetometer::getData(int16_t *buf, uint8_t length)
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
