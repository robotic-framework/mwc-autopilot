#ifndef BAROMETER_H_
#define BAROMETER_H_

#include <stdint.h>
#include "i2c.h"
#include "definition.h"

#define BAROMETER_HISTORY_SIZE 21

class Barometer : public I2C
{
private:
    void _calibration(uint32_t stepTime);

protected:
    uint16_t calibrateSteps = 0;
    uint32_t stepTime = 0;

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
                                 ct(0),
                                 cp(0),
                                 ccp(0),
                                 historyPressureIndex(0)
    {
        for (uint8_t i = 0; i < BAROMETER_HISTORY_SIZE; i++)
        {
            historyPressure[i] = 0;
        }
    };

    void update(uint32_t currentTime);
    void calibration(uint16_t steps);
    bool isCalibrating();
    uint16_t getUTData();
    uint32_t getUPData();
    int16_t getCTData();
    int32_t getCPData();
    int32_t getCCPData();
};

#endif // BAROMETER_H_
