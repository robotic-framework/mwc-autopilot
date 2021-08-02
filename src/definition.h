//
// Created by 李翌文 on 2021/6/10.
//

#include "config.h"
#if defined(SITL)
#include "ardu_sitl.h"
#else
#include <Arduino.h>
#endif

#ifndef AUTOPILOT_DEFINITION_H
#define AUTOPILOT_DEFINITION_H

#define min2(a,b) ((a)<(b)?(a):(b))
#define max2(a,b) ((a)>(b)?(a):(b))

/**
 * definition (do not modify anything)
 */

/**************************************************************************************/
/***************             Proc specific definitions             ********************/
/**************************************************************************************/
// Proc auto detection
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
#define PROMINI
#endif
#if defined(__AVR_ATmega32U4__) || defined(TEENSY20)
#define PROMICRO
#endif
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
#define MEGA
#endif
#if defined(STM32F4xx)
#define STM32
#endif

#if defined(PROMINI)
#define MAX_PROFILES 3
#define LEDPIN_PINMODE pinMode(13, OUTPUT);
#define LEDPIN_TOGGLE PINB |= 1 << 5; //switch LEDPIN state (digital PIN 13)
#define LEDPIN_OFF PORTB &= ~(1 << 5);
#define LEDPIN_ON PORTB |= (1 << 5);

#if defined(GPS_SERIAL)
#if GPS_SERIAL == 1
#define GPS_SERIAL_DEVICE Serial
#else
#error "Pro mini do not supportted Serial port more than 1"
#endif
#endif

#if defined(BLE_SERIAL)
#if BLE_SERIAL == 1
#define PROTOCOL_SERIAL Serial
#else
#error "Pro mini do not supportted Serial port more than 1"
#endif
#endif

#endif


#if defined(PROMINI)
#define MAX_PROFILES 3
#endif

#if defined(MEGA)
#define MAX_PROFILES 10
#define LEDPIN_PINMODE   \
    pinMode(13, OUTPUT); \
    pinMode(30, OUTPUT);
#define LEDPIN_TOGGLE \
    PINB |= (1 << 7); \
    PINC |= (1 << 7);
#define LEDPIN_ON      \
    PORTB |= (1 << 7); \
    PORTC |= (1 << 7);
#define LEDPIN_OFF      \
    PORTB &= ~(1 << 7); \
    PORTC &= ~(1 << 7);

#if defined(GPS_SERIAL)
#if GPS_SERIAL == 1
#define GPS_SERIAL_DEVICE Serial1
#elif GPS_SERIAL == 2
#define GPS_SERIAL_DEVICE Serial2
#elif GGPS_SERIAL == 3
#define GPS_SERIAL_DEVICE Serial3
#else
#error "Mega do not supportted Serial port more than 3"
#endif
#endif

#if defined(BLE_SERIAL)
#if BLE_SERIAL == 1
#define PROTOCOL_SERIAL Serial1
#elif BLE_SERIAL == 2
#define PROTOCOL_SERIAL Serial2
#elif BLE_SERIAL == 3
#define PROTOCOL_SERIAL Serial3
#else
#error "Mega do not supportted Serial port more than 3"
#endif
#else
#define PROTOCOL_SERIAL Serial
#endif
#endif

#if defined(STM32)
#define MAX_PROFILES 10

#if defined(GPS_SERIAL)
#if GPS_SERIAL == 1
#define GPS_SERIAL_DEVICE Serial1
#elif GPS_SERIAL == 2
#define GPS_SERIAL_DEVICE Serial2
#elif GGPS_SERIAL == 3
#define GPS_SERIAL_DEVICE Serial3
#else
#error "Mega do not supportted Serial port more than 3"
#endif
#endif

#if defined(BLE_SERIAL)
#if BLE_SERIAL == 1
#define PROTOCOL_SERIAL Serial1
#elif BLE_SERIAL == 2
#define PROTOCOL_SERIAL Serial2
#elif BLE_SERIAL == 3
#define PROTOCOL_SERIAL Serial3
#else
#error "Mega do not supportted Serial port more than 3"
#endif
#else
#define PROTOCOL_SERIAL Serial
#endif
#endif

#if defined(SITL)
#define MAX_PROFILES 10
#define LEDPIN_PINMODE ;
#define LEDPIN_TOGGLE ;
#define LEDPIN_ON ;
#define LEDPIN_OFF ;

#if defined(GPS_SERIAL)
#if GPS_SERIAL == 1
#define GPS_SERIAL_DEVICE SerialSITL1
#elif GPS_SERIAL == 2
#define GPS_SERIAL_DEVICE SerialSITL2
#elif GGPS_SERIAL == 3
#define GPS_SERIAL_DEVICE SerialSITL3
#else
#error "SITL do not supportted Serial port more than 3"
#endif
#endif

#if defined(BLE_SERIAL)
#if BLE_SERIAL == 1
#define PROTOCOL_SERIAL SerialSITL1
#elif BLE_SERIAL == 2
#define PROTOCOL_SERIAL SerialSITL2
#elif BLE_SERIAL == 3
#define PROTOCOL_SERIAL SerialSITL3
#else
#error "Mega do not supportted Serial port more than 3"
#endif
#else
#define PROTOCOL_SERIAL SerialSITL
#endif
#endif

#if defined(QUADX)
#define MOTOR_COUNT 4
#define ROBOT_TYPE 3
#else
#error "you have to define a robot type"
#endif // QUADX

// sensor lsb
#if defined(ACC_ADXL345)
#define ACC_1G_LSB 265 // full scale lsb
#endif
#if defined(ACC_BMA180)
#define ACC_1G_LSB 255
#endif
#if !defined(ACC_1G_LSB)
#define ACC_1G_LSB 256
#endif
#define ACCZ_25DEG (int16_t)(ACC_1G_LSB * 0.90631) // 0.90631 = cos(25deg) (cos(theta) of accZ comparison)
#define ACC_VelScale (9.80665f / 10000.0f / ACC_1G_LSB)

// gyro scale
#if defined(GYRO_ITG3205)
#define GYRO_SCALE (4 / 14.375 * PI / 180.0 / 1000000.0) // 14.375 LSB = 1 deg/s
#endif

// sensor flag
#if defined(ACC_ADXL345) || defined(ACC_BMA180) || defined(SITL)
#define SENSOR_ACC 1
#else
#define SENSOR_ACC 0
#endif

#if defined(GYRO_ITG3205) || defined(SITL)
#define SENSOR_GYRO 1
#else
#define SENSOR_GYRO 0
#endif

#if defined(MAG_HMC5883L) || defined(SITL)
#define SENSOR_MAG 1
#else
#define SENSOR_MAG 0
#endif

#if defined(BARO_BMP085) || defined(SITL)
#define SENSOR_BARO 1
#else
#define SENSOR_BARO 0
#endif

#define SENSOR_GPS 0
#define SENSOR_SONAR 0

#if defined(GPS_SERIAL) || defined(SITL)
#define GPS_ENABLED 1
#else
#define GPS_ENABLED 0
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

#endif //AUTOPILOT_DEFINITION_H
