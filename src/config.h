#ifndef CONFIG_H_
#define CONFIG_H_

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
#define ACC_ADXL345

#define GYRO_ITG3205

#define MAG_HMC5883L

/**
 * definition
 */
#define LEDPIN_PINMODE pinMode(13, OUTPUT);
#define LEDPIN_TOGGLE PINB |= 1 << 5; //switch LEDPIN state (digital PIN 13)
#define LEDPIN_OFF PORTB &= ~(1 << 5);
#define LEDPIN_ON PORTB |= (1 << 5);

#if defined(QUADX)
#define MOTOR_COUNT 4
#define ROBOT_TYPE 3
#else
#define MOTOR_COUNT 4
#define ROBOT_TYPE 3
#endif // QUADX

#if defined(ACC_ADXL345)
#define SENSOR_ACC 1
#define ACC_1G_LSB 265 // full scale lsb
#else
#define SENSOR_ACC 0
#define ACC_1G_LSB 0
#endif

#if defined(MAG_HMC5883L)
#define SENSOR_MAG 1
#else
#define SENSOR_MAG 0
#endif // MAG_HMC5883L

#define SENSOR_BARO 0
#define SENSOR_GPS 0
#define SENSOR_SONAR 0

#define ACCZ_25DEG (int16_t)(ACC_1G_LSB * 0.90631) // 0.90631 = cos(25deg) (cos(theta) of accZ comparison)

#endif // CONFIG_H_
