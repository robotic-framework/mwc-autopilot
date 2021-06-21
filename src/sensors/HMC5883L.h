#if !defined(HMC5883L_H_)
#define HMC5883L_H_

#include "magnetometer.h"

#define HMC5883L_DEVICE 0x1E
#define HMC5883L_TO_READ 6
#define HMC5883L_CONFIG_REGA 0x00
#define HMC5883L_CONFIG_REGB 0x01
#define HMC5883L_CONFIG_REG_MODE 0x02
#define HMC5883L_CONFIG_REG_DATA 0x03

#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03
#define HMC5883L_X_SELF_TEST_GAUSS (+1.16)
#define HMC5883L_Y_SELF_TEST_GAUSS (+1.16)
#define HMC5883L_Z_SELF_TEST_GAUSS (+1.08)

class HMC5883L : public Magnetometer
{
private:
    uint8_t _buff[HMC5883L_TO_READ];
    int32_t magTotal[3] = {0, 0, 0};

    void setScale(float gauss);
    bool biasCollect(uint8_t bias);

protected:
    void getRawData() override;
    void _init() override;

public:
    HMC5883L(uint8_t address) : Magnetometer(address){};
};

#endif // HMC5883L_H_
