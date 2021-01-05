#ifndef IMU_H_
#define IMU_H_

#include <Wire.h>
#include "config.h"
#include "def.h"
#include "led.h"

#if SENSOR_ACC
#include "sensors/accelerator.h"
#endif
#if SENSOR_GYRO
#include "sensors/gyroscope.h"
#endif
#if SENSOR_MAG
#include "sensors/meganetometer.h"
#endif
#if SENSOR_BARO
#include "sensors/baro.h"
#endif

#if defined(ACC_ADXL345)
#include "sensors/ADXL345.h"
#endif
#if defined(ACC_BMA180)
#include "sensors/BMA180.h"
#endif

#if defined(GYRO_ITG3205)
#include "sensors/ITG3205.h"
#endif

#if defined(MAG_HMC5883L)
#include "sensors/HMC5883L.h"
#endif

#if defined(BARO_BMP085)
#include "sensors/BMP085.h"
#endif

/* Set the Low Pass Filter factor for ACC
   Increasing this value would reduce ACC noise (visible in GUI), but would increase ACC lag time
   Comment this if  you do not want filter at all.
   unit = n power of 2 */
// this one is also used for ALT HOLD calculation, should not be changed
#ifndef ACC_LPF_FACTOR
#define ACC_LPF_FACTOR 4 // that means a LPF of 16
#endif

/* Set the Gyro Weight for Gyro/Acc complementary filter
   Increasing this value would reduce and delay Acc influence on the output of the filter*/
#ifndef GYR_CMPF_FACTOR
#define GYR_CMPF_FACTOR 10 //  that means a CMP_FACTOR of 1024 (2^10)
#endif

/* Set the Gyro Weight for Gyro/Magnetometer complementary filter
   Increasing this value would reduce and delay Magnetometer influence on the output of the filter*/
#define GYR_CMPFM_FACTOR 8 // that means a CMP_FACTOR of 256 (2^8)

#define MultiS16X16to32(longRes, intIn1, intIn2) \
    asm volatile(                                \
        "clr r26 \n\t"                           \
        "mul %A1, %A2 \n\t"                      \
        "movw %A0, r0 \n\t"                      \
        "muls %B1, %B2 \n\t"                     \
        "movw %C0, r0 \n\t"                      \
        "mulsu %B2, %A1 \n\t"                    \
        "sbc %D0, r26 \n\t"                      \
        "add %B0, r0 \n\t"                       \
        "adc %C0, r1 \n\t"                       \
        "adc %D0, r26 \n\t"                      \
        "mulsu %B1, %A2 \n\t"                    \
        "sbc %D0, r26 \n\t"                      \
        "add %B0, r0 \n\t"                       \
        "adc %C0, r1 \n\t"                       \
        "adc %D0, r26 \n\t"                      \
        "clr r1 \n\t"                            \
        : "=&r"(longRes)                         \
        : "a"(intIn1),                           \
          "a"(intIn2)                            \
        : "r26")

typedef struct
{
    int32_t X, Y, Z;
} t_int32_t_vector_def;

typedef struct
{
    uint16_t XL;
    int16_t X;
    uint16_t YL;
    int16_t Y;
    uint16_t ZL;
    int16_t Z;
} t_int16_t_vector_def;

// note: we use implicit first 16 MSB bits 32 -> 16 cast. ie V32.X>>16 = V16.X
typedef union
{
    int32_t A32[3];
    t_int32_t_vector_def V32;
    int16_t A16[6];
    t_int16_t_vector_def V16;
} t_int32_t_vector;

typedef struct
{
    int16_t Angle[2]; // absolute angle inclination in multiple of 0.1 degree    180 deg = 1800
    int16_t Heading;
} Attitude;
typedef struct
{
    int32_t Alt;
    int16_t Vario;
} Altitude;

int32_t __attribute__((noinline)) mul(int16_t a, int16_t b);
void rotateV32(t_int32_t_vector *v, int16_t *delta);
int16_t _atan2(int32_t y, int32_t x);
float invSqrt(float x);

extern uint8_t smallAngle25;

class IMU
{
private:
    // sensors
#if SENSOR_ACC
    Accelerator *acc;
#endif

#if SENSOR_GYRO
    Gyroscope *gyro;
#endif

#if SENSOR_MAG
    Meganetometer *mag;
#endif

#if SENSOR_BARO
    Baro *baro;
#endif

    // indicators
    Attitude att;
    Altitude alt;
    int16_t gyroWeighted[3];

    int16_t gyroPrevWeight[3] = {0, 0, 0};
    int16_t gyroWeight[3];
    uint32_t estPrevTime;
    int16_t accSmooth[3];
    uint32_t accLPF[3];

    t_int32_t_vector estimatedGyroData;
    t_int32_t_vector estimatedMagData;

public:
    IMU();

    void Init();
    void UpdateAcc(uint32_t currentTime);
    void UpdateGyro(uint32_t currentTime);
    void UpdateMag(uint32_t currentTime);
    void UpdateBaro(uint32_t currentTime);
    void UpdateSonar(uint32_t currentTime);
    void UpdateAttitude(uint32_t currentTime);
    void UpdateAltitude(uint32_t currentTime);

    void GetRawData(int16_t *buf, uint8_t length);
    void GetAccData(int16_t *buf, uint8_t length);
    void GetGyroData(int16_t *buf, uint8_t length);
    void GetMagData(int16_t *buf, uint8_t length);
    void AccCalibration();
    void MagCalibration();
    void GetAttitude(int16_t *buf, uint8_t length);
    void GetAltitude(int32_t *alt, int16_t *vario);
};

#endif // IMU_H_
