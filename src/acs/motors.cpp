#include "motors.h"
#include "global.h"

#define PIDMIX(X, Y, Z) rcCommand[THROTTLE] + pidOffset[ROLL] * X + pidOffset[PITCH] * Y + pidOffset[YAW] * Z

#define ACC_Z_DEADBAND (ACC_1G_LSB >> 5) // was 40 instead of 32 now

#define applyDeadband(value, deadband) \
    if (abs(value) < deadband)         \
    {                                  \
        value = 0;                     \
    }                                  \
    else if (value > 0)                \
    {                                  \
        value -= deadband;             \
    }                                  \
    else if (value < 0)                \
    {                                  \
        value += deadband;             \
    }

#if defined(PROMINI)
uint8_t Motors::Pins[8] = {9, 10, 11, 3, 6, 5, A2, 12};
#endif
#if defined(MEGA)
uint8_t Motors::Pins[8] = {3, 5, 6, 2, 7, 8, 9, 10};
#endif


Motors::Motors(Configuration *conf)
{
    this->conf = conf;
}

Motors::~Motors()
{
}

void Motors::init()
{
    for (uint8_t i = 0; i < MOTOR_COUNT; i++)
    {
        pinMode(Motors::Pins[i], OUTPUT);
    }

#if defined(PROMINI)
#if MOTOR_COUNT > 0
    TCCR1A |= _BV(COM1A1); // connect pin 9 to timer 1 channel A
#endif
#if MOTOR_COUNT > 1
    TCCR1A |= _BV(COM1B1); // connect pin 10 to timer 1 channel B
#endif
#if MOTOR_COUNT > 2
    TCCR2A |= _BV(COM2A1); // connect pin 11 to timer 2 channel A
#endif
#if MOTOR_COUNT > 3
    TCCR2A |= _BV(COM2B1); // connect pin 3 to timer 2 channel B
#endif
#endif
#if defined(MEGA)
#if MOTOR_COUNT > 0
    // _init 16bit timer 3
    TCCR3A |= (1 << WGM31); // phase correct mode
    TCCR3A &= ~(1 << WGM30);
    TCCR3B |= (1 << WGM33);
    TCCR3B &= ~(1 << CS31); // no prescaler
    ICR3 |= 0x3FFF;         // TOP to 16383;

    TCCR3A |= _BV(COM3C1); // connect pin 3 to timer 3 channel C
#endif
#if MOTOR_COUNT > 1
    TCCR3A |= _BV(COM3A1); // connect pin 5 to timer 3 channel A
#endif
#if MOTOR_COUNT > 2
    // _init 16bit timer 4
    TCCR4A |= (1 << WGM41); // phase correct mode
    TCCR4A &= ~(1 << WGM40);
    TCCR4B |= (1 << WGM43);
    TCCR4B &= ~(1 << CS41); // no prescaler
    ICR4 |= 0x3FFF;         // TOP to 16383;

    TCCR4A |= _BV(COM4A1); // connect pin 6 to timer 4 channel A
#endif
#if MOTOR_COUNT > 3
    TCCR3A |= _BV(COM3B1); // connect pin 2 to timer 3 channel B
#endif
#if MOTOR_COUNT > 4
    TCCR4A |= _BV(COM4B1); // connect pin 7 to timer 4 channel B
    TCCR4A |= _BV(COM4C1); // connect pin 8 to timer 4 channel C
#endif
#if MOTOR_COUNT > 6
    // timer 2 is a 8bit timer so we cant change its range
    TCCR2A |= _BV(COM2B1); // connect pin 9 to timer 2 channel B
    TCCR2A |= _BV(COM2A1); // connect pin 10 to timer 2 channel A
#endif
#endif

#if defined(ENABLE_ECS_CALIBRATION)
    WriteMotorsTrottle(ECS_CALIBRATE_HIGH);
    blinkLED(2, 20, 2);
    delay(4000);
    WriteMotorsTrottle(ECS_CALIBRATE_LOW);
    blinkLED(3, 20, 2);
    // use loop to stop _init, never start
    while (1)
    {
        delay(5000);
        blinkLED(4, 20, 2);
    }
#endif // ENABLE_ECS_CALIBRATION

    this->writeMotorsThrottle(MINCOMMAND);
}

void Motors::mixPID(PIDController *pid)
{
    if (!conf->arm)
    {
        return;
    }

    uint16_t maxMotor;
    uint8_t i;

    motors[0] = pid->mixPID(-1, 1, -1);
    motors[1] = pid->mixPID(-1, -1, 1);
    motors[2] = pid->mixPID(1, 1, 1);
    motors[3] = pid->mixPID(1, -1, -1);

    // Serial.print(", throttle: ");
    // Serial.print(rcCommand[THROTTLE]);
    // Serial.print(", motors: (");
    // Serial.print(motors[0]);
    // Serial.print(", ");
    // Serial.print(motors[1]);
    // Serial.print(", ");
    // Serial.print(motors[2]);
    // Serial.print(", ");
    // Serial.print(motors[3]);
    // Serial.println(") ");

    maxMotor = motors[0];
    for (i = 0; i < MOTOR_COUNT; i++)
    {
        if (motors[i] > maxMotor)
        {
            maxMotor = motors[i];
        }
    }
    for (i = 0; i < MOTOR_COUNT; i++)
    {
        if (maxMotor > MAXTHROTTLE)
        {
            motors[i] -= maxMotor - MAXTHROTTLE;
        }
        motors[i] = constrain(motors[i], MINTHROTTLE, MAXTHROTTLE);

        if (!conf->arm)
        {
            motors[i] = MINCOMMAND;
        }
    }
}

void Motors::updateMotors(uint32_t currentTime)
{
    if (conf->arm)
    {
        writeMotors();
    }
    else
    {
        writeMotorsThrottle(MINCOMMAND);
    }
}

void Motors::getMotors(uint16_t *buf, uint8_t length)
{
    length = min(length, 8);
    for (uint8_t i = 0; i < length; i++)
    {
        *buf++ = motors[i];
    }
}

uint8_t Motors::getMotorCount()
{
    return MOTOR_COUNT;
}

void Motors::writeMotorsThrottle(uint16_t throttle)
{
    for (uint8_t i = 0; i < MOTOR_COUNT; i++)
    {
        this->motors[i] = throttle;
    }
    this->writeMotors();
}

void Motors::writeMotors()
{
#if defined(PROMINI)
#if MOTOR_COUNT > 0
    OCR1A = this->motors[0] >> 3; // pin 9
#endif
#if MOTOR_COUNT > 1
    OCR1B = this->motors[1] >> 3; // pin 10
#endif
#if MOTOR_COUNT > 2
    OCR2A = this->motors[2] >> 3; // pin 11
#endif
#if MOTOR_COUNT > 3
    OCR2B = this->motors[3] >> 3; // pin 3
#endif
#endif
#if defined(MEGA)
#if MOTOR_COUNT > 0
    OCR3C = this->motors[0] << 3; // pin 3
#endif
#if MOTOR_COUNT > 1
    OCR3A = this->motors[1] << 3; // pin 5
#endif
#if MOTOR_COUNT > 2
    OCR4A = this->motors[2] << 3; // pin 6
#endif
#if MOTOR_COUNT > 3
    OCR3B = this->motors[3] << 3; // pin 2
#endif
#if MOTOR_COUNT > 4
    OCR4B = motor[4] << 3; // pin 7
    OCR4C = motor[5] << 3; // pin 8
#endif
#if MOTOR_COUNT > 6
    OCR2B = motor[6] >> 3; // pin 9
    OCR2A = motor[7] >> 3; // pin 10
#endif
#endif
}