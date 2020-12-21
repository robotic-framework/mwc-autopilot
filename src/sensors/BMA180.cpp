#include "BMA180.h"

void BMA180::init()
{
    delay(10);
    write(0x0D, 1 << 4);
    delay(10);

    uint8_t tmp[1], control;
    read(0x20, 1, tmp);
    control = tmp[0];
    control = control & 0x0F;
    control = control | (0x00 << 4);
    write(0x20, control);
    delay(10);

    read(0x30, 1, tmp);
    control = tmp[0];
    control = control & 0xFC;
    control = control | 0x00;
    write(0x30, control);
    delay(10);

    read(0x35, 1, tmp);
    control = tmp[0];
    control = control & 0xF1;
    control = control | (0x05 << 1);
    write(0x35, control);
    delay(10);
}

void BMA180::getRawData()
{
    read(BMA180_DATAX0, BMA180_TO_READ, _buff);
    ACC_ORIENTATION(
        ((_buff[1] << 8) | _buff[0]) >> 4,
        ((_buff[3] << 8) | _buff[2]) >> 4,
        ((_buff[5] << 8) | _buff[4]) >> 4);
}
