#include "BMP085.h"

void BMP085::init()
{
    // read calibration
    ctx.ac1 = read16(BMP085_CAL_AC1);
    ctx.ac2 = read16(BMP085_CAL_AC2);
    ctx.ac3 = read16(BMP085_CAL_AC3);
    ctx.ac4 = read16(BMP085_CAL_AC4);
    ctx.ac5 = read16(BMP085_CAL_AC5);
    ctx.ac6 = read16(BMP085_CAL_AC6);
    ctx.b1 = read16(BMP085_CAL_B1);
    ctx.b2 = read16(BMP085_CAL_B2);
    ctx.mb = read16(BMP085_CAL_MB);
    ctx.mc = read16(BMP085_CAL_MC);
    ctx.md = read16(BMP085_CAL_MD);

    startReadUT();
}

void BMP085::getRawData(uint32_t currentTime)
{
    if (currentTime < ctx.deadline)
    {
        return;
    }

    ctx.deadline = currentTime + 6000;
    if (ctx.state == 0)
    {
        ut.val = read16(BMP085_TEMPDATA);
        startReadUP();
        calcCoordinatedPressure();
        ctx.state = 1;

        #if BMP085_OVERSAMPLING == BMP085_STANDARD
        ctx.deadline += 3000; // 6000+3000=9000 1.5ms margin according to the spec (7.5ms P convetion time with BMP085_STANDARD)
        #endif
        #if BMP085_OVERSAMPLING == BMP085_HIGHRES
        ctx.deadline += 9000; // 6000+9000=15000 1.5ms margin according to the spec (13.5ms P convetion time with BMP085_STANDARD)
        #endif
        #if BMP085_OVERSAMPLING == BMP085_ULTRAHIGHRES
        ctx.deadline += 21000; // 6000+21000=27000 1.5ms margin according to the spec (25.5ms P convetion time with BMP085_ULTRAHIGHRES)
        #endif
    }
    else
    {
        up.val = read16(BMP085_PRESSUREDATA);
        up.val <<= 8;
        up.val |= read8(BMP085_PRESSUREDATA + 2);
        up.val >>= (8 - BMP085_OVERSAMPLING);
        startReadUT();
        calcCompensated();
        ctx.state = 0;
    }
}

void BMP085::startReadUT()
{
    write(BMP085_CONTROL, BMP085_READTEMPCMD);
}

void BMP085::startReadUP()
{
    write(BMP085_CONTROL, BMP085_READPRESSURECMD + (BMP085_OVERSAMPLING << 6));
}

int32_t BMP085::computeB5()
{
    int32_t x1 = ((int32_t)ut.val - ctx.ac6) * ctx.ac5 >> 15;
    int32_t x2 = ((int32_t)ctx.mc << 11) / (x1 + ctx.md);
    return x1 + x2;
}

void BMP085::calcCompensated()
{
    // temperature
    int32_t b5 = computeB5();
    ct = (b5 * 10 + 8) >> 4;

    // pressure
    int32_t b6 = b5 - 4000;
    int32_t x1 = ((int32_t)ctx.b2 * ((b6 * b6) >> 12)) >> 11;
    int32_t x2 = ((int32_t)ctx.ac2 * b6) >> 11;
    int32_t x3 = x1 + x2;
    int32_t b3 = ((((int32_t)ctx.ac1 * 4 + x3) << BMP085_OVERSAMPLING) + 2) / 4;

    x1 = ((int32_t)ctx.ac3 * b6) >> 13;
    x2 = ((int32_t)ctx.b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    uint32_t b4 = ((uint32_t)ctx.ac4 * (uint32_t)(x3 + 32768)) >> 15;
    uint32_t b7 = (up.val - b3) * (uint32_t)(50000 >> BMP085_OVERSAMPLING);
    int32_t p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;

    cp = p + ((x1 + x2 + 3791) >> 4);
}

void BMP085::calcCoordinatedPressure()
{
    uint8_t nextIndex = historyPressureIndex + 1;
    if (nextIndex == BARO_HISTORY_SIZE)
    {
        nextIndex = 0;
    }
    historyPressure[historyPressureIndex] = cp;
    ccp += historyPressure[historyPressureIndex];
    ccp -= historyPressure[nextIndex];
    historyPressureIndex = nextIndex;
}
