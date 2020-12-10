#include "gyroscope.h"

void Gyroscope::Update()
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

void Gyroscope::Calibration()
{
  calibrateSteps = 512;
  for (size_t i = 0; i < 3; i++)
  {
    offsetTotal[i] = 0;
  }
}

void Gyroscope::calibration()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    offsetTotal[i] += data[i];
    offset[i] = offsetTotal[i] >> 9;
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
