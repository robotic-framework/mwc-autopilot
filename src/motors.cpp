#include "motors.h"
#include "def.h"

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

extern IMU imu;
extern bool angleMode;
extern bool horizenMode;
extern bool arm;
extern int16_t rcCommand[4];
extern PID pid[PIDITEMS];

#if defined(PROMINI)
uint8_t Motors::Pins[8] = {9, 10, 11, 3, 6, 5, A2, 12};
#endif
#if defined(MEGA)
uint8_t Motors::Pins[8] = {3, 5, 6, 2, 7, 8, 9, 10};
#endif

int16_t Motors::errorAltI = 0;

Motors::Motors()
{
}

Motors::~Motors()
{
}

void Motors::Init()
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
    // init 16bit timer 3
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
    // init 16bit timer 4
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
    // use loop to stop init, never start
    while (1)
    {
        delay(5000);
        blinkLED(4, 20, 2);
    }
#endif // ENABLE_ECS_CALIBRATION

    this->WriteMotorsTrottle(MINCOMMAND);
}

void Motors::UpdatePID(uint32_t currentTime)
{
    if (arm)
    {
        applyPID(currentTime);
        mixPID();
    }
}

void Motors::UpdateMotors(uint32_t currentTime)
{
    if (arm)
    {
        writeMotors();
    }
}

void Motors::applyPID(uint32_t currentTime)
{
    uint8_t axis;
    int16_t error, errorAngle;
    int16_t rc;
    int16_t pidOffsetAlt;
    static int16_t lastGyro[2] = {0, 0};
    static int16_t errorGyroI[2] = {0, 0};
    static int32_t errorGyroI_YAW;
    static int16_t errorAngleI[2] = {0, 0};
    int16_t PTerm = 0, ITerm = 0, DTerm, PTermACC, ITermACC;
    int16_t delta;

    static uint16_t previousTime = 0;
    uint16_t deltaTime;

    static int16_t delta1[2], delta2[2];

    if (previousTime == 0)
    {
        previousTime = currentTime;
        return;
    }
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;

    if (horizenMode)
    {
        prop = min(max(abs(rcCommand[PITCH]), abs(rcCommand[ROLL])), 512);
    }

    int16_t gyroData[3];
    int16_t attitude[3];
    imu.GetGyroData(gyroData, 3);
    imu.GetAttitude(attitude, 3);

    // ROLL & PITCH
    for (axis = 0; axis < 2; axis++)
    {
        rc = rcCommand[axis] << 1;
        error = rc - gyroData[axis];
        errorGyroI[axis] = constrain(errorGyroI[axis] + error, -16000, 16000);
        if (abs(gyroData[axis]) > 640)
        {
            errorGyroI[axis] = 0;
        }
        ITerm = (errorGyroI[axis] >> 7) * pid[axis].I >> 6;
        PTerm = mul(rc, pid[axis].P) >> 6;

        if (angleMode || horizenMode)
        {
            errorAngle = constrain(rc, -500, 500) - attitude[axis];
            errorAngleI[axis] = constrain(errorAngleI[axis] + errorAngle, -10000, 10000);

            PTermACC = mul(errorAngle, pid[PIDLEVEL].P) >> 7;

            int16_t limit = pid[PIDLEVEL].D * 5;
            PTermACC = constrain(PTermACC, -limit, limit);

            ITermACC = mul(errorAngleI[axis], pid[PIDLEVEL].I) >> 12;

            ITerm = ITermACC + ((ITerm - ITermACC) * prop >> 9);
            PTerm = PTermACC + ((PTerm - PTermACC) * prop >> 9);
        }

        // PTerm -= mul(gyroData[axis], dynP[axis]) >> 6;

        delta = gyroData[axis] - lastGyro[axis];
        lastGyro[axis] = gyroData[axis];
        DTerm = delta1[axis] + delta2[axis] + delta;
        delta2[axis] = delta1[axis];
        delta1[axis] = delta;

        // DTerm = mul(DTerm, dynD[axis]) >> 5;
        pidOffset[axis] = PTerm + ITerm - DTerm;
    }

    // YAW
#define GYRO_P_MAX 300
#define GYRO_I_MAX 250

    rc = mul(rcCommand[YAW], 30) >> 5;
    error = rc - gyroData[YAW];
    errorGyroI_YAW += mul(error, pid[YAW].I);
    errorGyroI_YAW = constrain(errorGyroI_YAW, 2 - ((int32_t)1 << 28), -2 + ((int32_t)1 << 28));
    if (abs(rc) > 50)
    {
        errorGyroI_YAW = 0;
    }
    PTerm = mul(error, pid[YAW].P) >> 6;
    int16_t limit = GYRO_P_MAX - pid[YAW].D;
    PTerm = constrain(PTerm, -limit, limit);

    ITerm = constrain((int16_t)(errorGyroI_YAW >> 13), -GYRO_I_MAX, GYRO_I_MAX);
    pidOffset[YAW] = PTerm + ITerm;

    // ALT HOLD
    if (baroMode != lastBaroMode)
    {
        isBaroModeChanged = 1;
        lastBaroMode = baroMode;
    }
    if (isBaroModeChanged)
    {
        if (baroMode)
        {
            errorAltI = 0;
        }
        initialThrottleHold = rcCommand[THROTTLE];
        isBaroModeChanged = 0;
    }
    if (baroMode)
    {
        int32_t alt;
        int16_t vario;
        imu.GetAltitude(&alt, &vario);
        int16_t errorAlt = constrain(altHold - alt, -300, 300);
        applyDeadband(errorAlt, 10);
        pidOffsetAlt = constrain(pid[PIDALT].P * errorAlt >> 7, -150, 150);

        errorAltI += pid[PIDALT].I * errorAlt >> 6;
        errorAltI = constrain(errorAltI, -30000, 30000);
        pidOffsetAlt += errorAltI >> 9;

        int16_t accZ = imu.GetACCZ();
        applyDeadband(accZ, ACC_Z_DEADBAND);

        static int32_t lastAlt;
        static float vel = 0.f;
        int16_t altVel = mul((alt - lastAlt), 40); // altitude update interval is 40Hz
        lastAlt = alt;
        altVel = constrain(altVel, -300, 300);
        applyDeadband(altVel, 10);

        // Integrator - velocity, cm/sec
        vel += accZ * ACC_VelScale * deltaTime;
        // apply Complimentary Filter to keep the calculated velocity based on baro velocity (i.e. near real velocity).
        // By using CF it's possible to correct the drift of integrated accZ (velocity) without loosing the phase, i.e without delay
        vel = vel * 0.985f + altVel * 0.015f;

        vario = vel;
        applyDeadband(vario, 5);
        imu.SetAltitudeVario(vario);
        pidOffsetAlt -= constrain(pid[PIDALT].D * vario >> 4, -150, 150);
    }

    if (baroMode /* || takeOffMode || landingMode */)
    {
        // throttle compensation
        rcCommand[THROTTLE] = initialThrottleHold + pidOffsetAlt;
    }
}

void Motors::mixPID()
{
    uint16_t maxMotor;
    uint8_t i;

    motors[0] = PIDMIX(-1, 1, -1);
    motors[1] = PIDMIX(-1, -1, 1);
    motors[2] = PIDMIX(1, 1, 1);
    motors[3] = PIDMIX(1, -1, -1);

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

        if (!arm)
        {
            motors[i] = MINCOMMAND;
        }
    }
}

void Motors::GetMotors(uint16_t *buf, uint8_t length)
{
    length = min(length, 8);
    for (uint8_t i = 0; i < length; i++)
    {
        *buf++ = motors[i];
    }
}

uint8_t Motors::GetMotorCount()
{
    return MOTOR_COUNT;
}

void Motors::WriteMotorsTrottle(uint16_t throttle)
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