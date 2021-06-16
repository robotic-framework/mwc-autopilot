#ifndef BAROMETER_H_
#define BAROMETER_H_

#include <stdint.h>
#include "i2c.h"
#include "../definition.h"

#define BAROMETER_HISTORY_SIZE 21

class Barometer : public I2C
{
private:
    void calibration(uint32_t stepTime);

protected:
    uint16_t calibrateSteps;
    uint32_t stepTime = 0;
    float logBaroGroundPressureSum, baroGroundTemperatureScale;

    union
    {
        uint16_t val;
        uint8_t raw[2];
    } ut;

    union
    {
        uint32_t val;
        uint8_t raw[4];
    } up;

    int16_t ct;
    int32_t cp, ccp;

    int32_t historyPressure[BAROMETER_HISTORY_SIZE];
    uint8_t historyPressureIndex;

    virtual void getRawData(uint32_t currentTime) = 0;

public:
    Barometer(uint8_t address) : I2C(address),
                                 historyPressureIndex(0),
                                 ccp(0),
                                 ct(0),
                                 cp(0)
    {
        for (uint8_t i = 0; i < BAROMETER_HISTORY_SIZE; i++)
        {
            historyPressure[i] = 0;
        }
    };

    void Update(uint32_t currentTime);
    void Calibration(uint16_t steps);
    bool IsCalibrating();
    uint16_t GetUTData();
    uint32_t GetUPData();
    int16_t GetCTData();
    int32_t GetCPData();
    int32_t GetCCPData();
    float GetLogBaroGroundPressureSum() { return logBaroGroundPressureSum; }
    float GetBaroGroundTemperatureScale() { return baroGroundTemperatureScale; };
};

#endif // BAROMETER_H_
