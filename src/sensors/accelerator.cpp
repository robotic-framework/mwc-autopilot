#include "accelerator.h"

void Accelerator::Update()
{
  this->getRawData();
  if (calibrateSteps > 0)
  {
    calibration();
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

void Accelerator::calibration()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    offsetTotal[i] += data[i];
    offset[i] = offsetTotal[i] >> 9;
  }

  if (calibrateSteps == 1)
  {
    // calibration done
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
