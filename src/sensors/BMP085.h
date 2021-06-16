#ifndef BMP085_H_
#define BMP085_H_

#include "barometer.h"

#define BMP085_DEVICE 0x77  // BMP085 device address
#define BMP085_CAL_AC1 0xAA //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC2 0xAC //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC3 0xAE //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC4 0xB0 //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC5 0xB2 //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC6 0xB4 //!< R   Calibration data (16 bits)
#define BMP085_CAL_B1 0xB6  //!< R   Calibration data (16 bits)
#define BMP085_CAL_B2 0xB8  //!< R   Calibration data (16 bits)
#define BMP085_CAL_MB 0xBA  //!< R   Calibration data (16 bits)
#define BMP085_CAL_MC 0xBC  //!< R   Calibration data (16 bits)
#define BMP085_CAL_MD 0xBE  //!< R   Calibration data (16 bits)

#define BMP085_ULTRALOWPOWER 0 //!< Ultra low power mode
#define BMP085_STANDARD 1      //!< Standard mode
#define BMP085_HIGHRES 2       //!< High-res mode
#define BMP085_ULTRAHIGHRES 3  //!< Ultra high-res mode
#define BMP085_OVERSAMPLING BMP085_ULTRAHIGHRES

#define BMP085_CONTROL 0xF4         //!< Control register
#define BMP085_TEMPDATA 0xF6        //!< Temperature data register
#define BMP085_PRESSUREDATA 0xF6    //!< Pressure data register
#define BMP085_READTEMPCMD 0x2E     //!< Read temperature control register value
#define BMP085_READPRESSURECMD 0x34 //!< Read pressure control register value

class BMP085 : public Barometer
{
private:
    struct
    {
        int16_t ac1, ac2, ac3;
        uint16_t ac4, ac5, ac6;
        int16_t b1, b2, mb, mc, md;
        uint8_t state;
        uint32_t deadline;
    } ctx;

private:
    void startReadUT();
    void startReadUP();
    void calcCompensated();
    void calcCoordinatedPressure();
    int32_t computeB5();

public:
    BMP085(uint8_t address) : Barometer(address) {}

protected:
    void getRawData(uint32_t currentTime) override;
    void init() override;
};

#endif
