#include "ADXL345.h"

void ADXL345::init()
{
  write(ADXL345_POWER_CTL, 8);
  delay(10);
  write(ADXL345_DATA_FORMAT, 0x0B); // 0000 1011 - +/-16g
  delay(100);
}

void ADXL345::getRawData()
{
  read(ADXL345_DATAX0, ADXL345_TO_READ, _buff);
  ACC_ORIENTATION(
    (((int)_buff[1]) << 8) | _buff[0],
    (((int)_buff[3]) << 8) | _buff[2],
    (((int)_buff[5]) << 8) | _buff[4]);
}
