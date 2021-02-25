#ifndef CONFIG_H_
#define CONFIG_H_

#define DEBUG 0
#define TEST_ALTHOLD

/**
 * BLE Protocol
 */
//#define BLE_SERIAL 2
#define BLE_BAUD 115200

#define VERSION 100
/**
 * Robot type
 */
// #define QUADCROSS
#define QUADX
// #define HEX6

/**
 * Motor config
 */
/* Set the minimum throttle command sent to the ESC (Electronic Speed Controller)
       This is the minimum value that allow motors to run at a idle speed  */
//#define MINTHROTTLE 1300 // for Turnigy Plush ESCs 10A
//#define MINTHROTTLE 1120 // for Super Simple ESCs 10A
//#define MINTHROTTLE 1064 // special ESC (simonk)
//#define MINTHROTTLE 1050 // for brushed ESCs like ladybird
#define MINTHROTTLE 1150 // (*) (**)

/* this is the maximum value for the ESCs at full power, this value can be increased up to 2000 */
#define MAXTHROTTLE 1850

/* this is the value for the ESCs when they are not armed
       in some cases, this value must be lowered down to 900 for some specific ESCs, otherwise they failed to initiate */
#define MINCOMMAND 1000

// #define ENABLE_ECS_CALIBRATION
#define ECS_CALIBRATE_LOW MINCOMMAND
#define ECS_CALIBRATE_HIGH 2000

#define LOOP_TIME 2800

/**
 * Sensor
 */
// Accelerator
// #define ACC_ADXL345
#define ACC_BMA180

// Gyroscope
#define GYRO_ITG3205

// Meganetometer
// #define MAG_HMC5883L

// Baro
#define BARO_BMP085

/**
 * GPS
 */
#define GPS_SERIAL 1
#define GPS_BAUD 38400
#define GPS_LAG 1.0f

/**
 * Mission
 */
// if we are within this distance to a waypoint then we consider it reached (distance is in cm)
#define MISSION_WP_RADIUS 100 // (cm)

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

#if defined(PROMINI)
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

#if defined(MEGA)
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
#if defined(ACC_ADXL345) || defined(ACC_BMA180)
#define SENSOR_ACC 1
#else
#define SENSOR_ACC 0
#endif

#if defined(GYRO_ITG3205)
#define SENSOR_GYRO 1
#else
#define SENSOR_GYRO 0
#endif

#if defined(MAG_HMC5883L)
#define SENSOR_MAG 1
#else
#define SENSOR_MAG 0
#endif

#if defined(BARO_BMP085)
#define SENSOR_BARO 1
#else
#define SENSOR_BARO 0
#endif

#define SENSOR_GPS 0
#define SENSOR_SONAR 0

#if defined(GPS_SERIAL)
#define GPS_ENABLED 1
#else
#define GPS_ENABLED 0
#endif

#endif // CONFIG_H_
