#include "gyroscope.h"
#include "../global.h"

extern Configuration conf;

void Gyroscope::Update(uint32_t currentTime)
{
    this->getRawData();
    if (calibrateSteps > 0)
    {
        calibration();
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        data[i] -= conf.raw.gyroOffset[i];
        data[i] = constrain(data[i], prevData[i] - 800, prevData[i] + 800);
        prevData[i] = data[i];
    }
}

void Gyroscope::Calibration()
{
    calibrateSteps = 512;
    for (size_t i = 0; i < 3; i++)
    {
        offsetTotal[i] = 0;
    }
}

bool Gyroscope::IsCalibrating()
{
    return (calibrateSteps > 0);
}

void Gyroscope::calibration()
{
    LEDPIN_ON
    for (uint8_t i = 0; i < 3; i++)
    {
        offsetTotal[i] += data[i];
        conf.raw.gyroOffset[i] = offsetTotal[i] >> 9;
    }

    if (calibrateSteps == 1)
    {
        // calibration done
        LEDPIN_OFF
    }
    calibrateSteps--;
}

void Gyroscope::GetData(int16_t *buf, uint8_t length)
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

void Gyroscope::SetData(int16_t *data, uint8_t length)
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
