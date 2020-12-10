#include "ITG3205.h"

void ITG3205::init()
{
  write(ITG3205_PWR_MGM, 0x80);
  delay(10);
  write(ITG3205_DLPF_FS, 0x18);
  delay(10);
  write(ITG3205_PWR_MGM, 0x03);
  delay(100);
}

void ITG3205::getRawData()
{
  read(ITG3205_GYRO_XOUT, ITG3205_GYRO_TO_READ, _buff);
  GYRO_ORIENTATION(
    ((((int)_buff[0]) << 8) | _buff[1]) >> 2,
    ((((int)_buff[2]) << 8) | _buff[3]) >> 2,
    ((((int)_buff[4]) << 8) | _buff[5]) >> 2);
}

void ITG3205::Update()
{
  Gyroscope::Update();

  for (size_t i = 0; i < 3; i++)
  {
    data[i] = constrain(data[i], _prevData[i] - 800, _prevData[i] + 800);
    _prevData[i] = data[i];
  }
}
