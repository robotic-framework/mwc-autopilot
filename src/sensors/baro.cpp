#include "baro.h"

void Baro::Update(uint32_t currentTime)
{
    this->getRawData(currentTime);

    if (calibrateSteps > 0)
    {
        LEDPIN_ON;
        calibration(currentTime);
        calibrateSteps--;
        if (calibrateSteps == 0)
        {
            LEDPIN_OFF;
        }
    }
}

void Baro::Calibration(uint16_t steps)
{
    calibrateSteps = steps;
}

void Baro::calibration(uint32_t currentTime)
{
    logBaroGroundPressureSum = log(ccp);
    baroGroundTemperatureScale = ((int32_t)ct + 27315) * 29.271267f;
}

uint16_t Baro::GetUTData()
{
    return ut.val;
}

uint32_t Baro::GetUPData()
{
    return up.val;
}

int16_t Baro::GetCTData()
{
    return ct;
}

int32_t Baro::GetCPData()
{
    return cp;
}

int32_t Baro::GetCCPData()
{
    return ccp;
}
