//
// Created by 李翌文 on 2021/7/13.
//

#ifndef AUTOPILOT_WIRE_IMPLEMENT_H
#define AUTOPILOT_WIRE_IMPLEMENT_H

#include <stdint.h>

#define ADXL345_DEVICE 0x53
#define BMA180_DEVICE 0x40
#define BMP085_DEVICE 0x77
#define HMC5883L_DEVICE 0x1E
#define ITG3205_DEVICE 0x68

typedef enum {
    DEVICE_ACC,
    DEVICE_GYRO,
    DEVICE_MAG,
    DEVICE_BAROMETER
} wire_device_type_e;

union raw_acc {
    int16_t data[3];
    uint8_t raw[6];
};
union raw_gyro {
    int16_t data[3];
    uint8_t raw[6];
};
union raw_mag {
    int16_t data[3];
    uint8_t raw[6];
};
union raw_barometer {

};

class WireImplement {
private:
    wire_device_type_e currentDevice;
    bool _available = false;

    raw_acc accData;
    raw_gyro gyroData;
    raw_mag magData;

public:
    void begin();

    void beginTransmission(uint8_t);

    void beginTransmission(int);

    uint8_t endTransmission(void);

    uint8_t requestFrom(uint8_t, uint8_t);

    uint8_t write(uint8_t);

    uint8_t write(const uint8_t *, uint8_t);

    int read(void);

    int available(void);
};

extern WireImplement Wire;

#endif //AUTOPILOT_WIRE_IMPLEMENT_H
