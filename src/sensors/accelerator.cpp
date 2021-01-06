#include "accelerator.h"

void Accelerator::Update(uint32_t currentTime)
{
    this->getRawData();

    if (calibrateSteps > 0)
    {
        calibration(currentTime);
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        data[i] -= offset[i];
    }
}

void Accelerator::Calibration()
{
    calibrateSteps = 512;
    for (size_t i = 0; i < 3; i++)
    {
        offsetTotal[i] = 0;
    }
}

bool Accelerator::IsCalibrating()
{
    return (calibrateSteps > 0);
}

void Accelerator::calibration(uint32_t currentTime)
{
    LEDPIN_ON
    for (uint8_t i = 0; i < 3; i++)
    {
        offsetTotal[i] += data[i];
        offset[i] = offsetTotal[i] >> 9;
    }

    if (calibrateSteps == 1)
    {
        // calibration done
        LEDPIN_OFF
        offset[YAW] -= ACC_1G_LSB;
    }
    calibrateSteps--;
}

void Accelerator::GetData(int16_t *buf, uint8_t length)
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
