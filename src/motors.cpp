#include "motors.h"
#include "def.h"

#define PIDMIX(X, Y, Z) rcCommand[THROTTLE] + pidOffset[ROLL] * X + pidOffset[PITCH] * Y + pidOffset[YAW] * Z

extern IMU imu;
extern bool angleMode;
extern bool horizenMode;
extern bool arm;
extern int16_t rcCommand[4];
extern PID pid[PIDITEMS];

uint8_t Motors::Pins[8] = {9, 10, 11, 3, 6, 5, A2, 12};

Motors::Motors(uint8_t motorCount)
{
    this->motorCount = motorCount;
}

Motors::~Motors()
{
}

void Motors::Init()
{
    for (uint8_t i = 0; i < this->motorCount; i++)
    {
        pinMode(Motors::Pins[i], OUTPUT);
    }

    if (this->motorCount > 0)
    {
        TCCR1A |= _BV(COM1A1); // connect pin 9 to timer 1 channel A
    }
    if (this->motorCount > 1)
    {
        TCCR1A |= _BV(COM1B1); // connect pin 10 to timer 1 channel B
    }
    if (this->motorCount > 2)
    {
        TCCR2A |= _BV(COM2A1); // connect pin 11 to timer 2 channel A
    }
    if (this->motorCount > 3)
    {
        TCCR2A |= _BV(COM2B1); // connect pin 3 to timer 2 channel B
    }

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
    if (!arm)
    {
        return;
    }

    applyPID();
}

void Motors::UpdateMotors(uint32_t currentTime) {
    mixPID();

    if (arm)
    {
        writeMotors();
    }
}

void Motors::applyPID()
{
    uint8_t axis;
    int16_t error, errorAngle;
    int16_t rc;
    static int16_t lastGyro[2] = {0, 0};
    static int16_t errorGyroI[2] = {0, 0};
    static int32_t errorGyroI_YAW;
    static int16_t errorAngleI[2] = {0, 0};
    int16_t PTerm = 0, ITerm = 0, DTerm, PTermACC, ITermACC;
    int16_t delta;
    static int16_t delta1[2], delta2[2];

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
    return this->motorCount;
}

void Motors::WriteMotorsTrottle(uint16_t throttle)
{
    for (uint8_t i = 0; i < this->motorCount; i++)
    {
        this->motors[i] = throttle;
    }
    this->writeMotors();
}

void Motors::writeMotors()
{
    if (this->motorCount > 0)
    {
        OCR1A = this->motors[0] >> 3; //  pin 9
    }
    if (this->motorCount > 1)
    {
        OCR1B = this->motors[1] >> 3; //  pin 10
    }
    if (this->motorCount > 2)
    {
        OCR2A = this->motors[2] >> 3; //  pin 11
    }
    if (this->motorCount > 3)
    {
        OCR2B = this->motors[3] >> 3; //  pin 3
    }
}