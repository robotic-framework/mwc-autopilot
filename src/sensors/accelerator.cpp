#include "accelerator.h"

void Accelerator::Update(uint32_t currentTime)
{
    this->getRawData();

    if (calibrateSteps > 0)
    {
        calibration(currentTime);
    }

    data[ROLL] -= offset[ROLL];
    data[PITCH] -= offset[PITCH];
    data[YAW] -= offset[YAW];
}

void Accelerator::Calibration()
{
    calibrateSteps = 512;
    for (size_t i = 0; i < 3; i++)
    {
        offsetTotal[i] = 0;
    }
}

void Accelerator::calibration(uint32_t currentTime)
{
    // each read is spaced by 100ms
    if (currentTime < stepTime)
    {
        return;
    }
    stepTime = currentTime + 10000;

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
