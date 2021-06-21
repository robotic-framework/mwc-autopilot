#include "ADXL345.h"

void ADXL345::_init()
{
  delay(10);
  write(ADXL345_POWER_CTL, 0x08);
  write(ADXL345_DATA_FORMAT, 0x0B); // 0000 1011 - +/-16g
  write(ADXL345_BW_RATE, 0x09); // 0000 1001 - rate=50hz, bw=20hz
}

void ADXL345::getRawData()
{
  read(ADXL345_DATAX0, ADXL345_TO_READ, _buff);
  ACC_ORIENTATION(
      ((_buff[1] << 8) | _buff[0]),
      ((_buff[3] << 8) | _buff[2]),
      ((_buff[5] << 8) | _buff[4]));
}
