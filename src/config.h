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

#endif // CONFIG_H_
