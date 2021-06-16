#include "pid.h"
#include "utils.h"

#define ACC_Z_DEADBAND (ACC_1G_LSB >> 5) // was 40 instead of 32 now

#define applyDeadband(value, deadband) \
    if (abs(value) < (deadband))         \
    {                                  \
        (value) = 0;                     \
    }                                  \
    else if ((value) > 0)                \
    {                                  \
        (value) -= (deadband);             \
    }                                  \
    else if ((value) < 0)                \
    {                                  \
        (value) += (deadband);             \
    }

extern int16_t rcCommand[4];

int16_t PIDController::errorAltI = 0;

PIDController::PIDController(Configuration *conf) :
        isBaroModeChanged(1),
        initialThrottleHold(0) {
    lastBaroMode = conf->baroMode;
}

void PIDController::Update(uint32_t currentTime) {
    uint8_t axis;
    int16_t error, errorAngle;
    int16_t rc;
    int16_t pidOffsetAlt;
    static int16_t lastGyro[2] = {0, 0};
    static int16_t errorGyroI[2] = {0, 0};
    static int32_t errorGyroI_YAW;
    static int16_t errorAngleI[2] = {0, 0};
    int16_t PTerm, ITerm, DTerm, PTermACC, ITermACC;
    int16_t delta;

    static uint16_t previousTime = 0;
    uint16_t deltaTime;

    static int16_t delta1[2], delta2[2];

    if (previousTime == 0) {
        previousTime = currentTime;
        return;
    }
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;

    if (conf->horizonMode) {
        prop = min(max(abs(rcCommand[PITCH]), abs(rcCommand[ROLL])), 512);
    }

    int16_t gyroData[3];
    int16_t attitude[3];
    imu->GetGyroData(gyroData, 3);
    aa->GetAttitude(attitude, 3);

    // ROLL & PITCH
    for (axis = 0; axis < 2; axis++) {
        rc = rcCommand[axis] << 1;
        error = rc - gyroData[axis];
        errorGyroI[axis] = constrain(errorGyroI[axis] + error, -16000, 16000);
        if (abs(gyroData[axis]) > 640) {
            errorGyroI[axis] = 0;
        }
        ITerm = (errorGyroI[axis] >> 7) * conf->raw.pid[axis].I >> 6;
        PTerm = mul(rc, conf->raw.pid[axis].P) >> 6;

        if (conf->angleMode || conf->horizonMode) {
            errorAngle = constrain(rc, -500, 500) - attitude[axis];
            errorAngleI[axis] = constrain(errorAngleI[axis] + errorAngle, -10000, 10000);

            PTermACC = mul(errorAngle, conf->raw.pid[PIDLEVEL].P) >> 7;

            int16_t limit = conf->raw.pid[PIDLEVEL].D * 5;
            PTermACC = constrain(PTermACC, -limit, limit);

            ITermACC = mul(errorAngleI[axis], conf->raw.pid[PIDLEVEL].I) >> 12;

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
    errorGyroI_YAW += mul(error, conf->raw.pid[YAW].I);
    errorGyroI_YAW = constrain(errorGyroI_YAW, 2 - ((int32_t) 1 << 28), -2 + ((int32_t) 1 << 28));
    if (abs(rc) > 50) {
        errorGyroI_YAW = 0;
    }
    PTerm = mul(error, conf->raw.pid[YAW].P) >> 6;
    int16_t limit = GYRO_P_MAX - conf->raw.pid[YAW].D;
    PTerm = constrain(PTerm, -limit, limit);

    ITerm = constrain((int16_t) (errorGyroI_YAW >> 13), -GYRO_I_MAX, GYRO_I_MAX);
    pidOffset[YAW] = PTerm + ITerm;

    // ALT HOLD
    if (conf->baroMode != lastBaroMode) {
        isBaroModeChanged = 1;
        lastBaroMode = conf->baroMode;
    }
    if (isBaroModeChanged) {
        if (conf->baroMode) {
            errorAltI = 0;
        }
        initialThrottleHold = rcCommand[THROTTLE];
        isBaroModeChanged = 0;
    }
    if (conf->baroMode) {
        int32_t alt;
        int16_t vario;
        aa->GetAltitude(&alt, &vario);
        int16_t errorAlt = constrain(conf->altHold - alt, -300, 300);
        // Serial.print("hold: ");
        // Serial.print(altHold);
        // Serial.print(", alt: ");
        // Serial.print(alt);
        applyDeadband(errorAlt, 10);
        pidOffsetAlt = constrain(conf->raw.pid[PIDALT].P * errorAlt >> 7, -150, 150);

        errorAltI += conf->raw.pid[PIDALT].I * errorAlt >> 6;
        errorAltI = constrain(errorAltI, -30000, 30000);
        pidOffsetAlt += errorAltI >> 9;

        int16_t accZ = aa->GetACCZ();
        applyDeadband(accZ, ACC_Z_DEADBAND);

        static int32_t lastAlt;
        static float zVel = 0.f;
        int16_t altVel = mul((alt - lastAlt), 40); // altitude update interval is 40Hz
        lastAlt = alt;
        altVel = constrain(altVel, -300, 300);
        applyDeadband(altVel, 10);

        // Integrator - velocity, cm/sec
        zVel += accZ * ACC_VelScale * deltaTime;
        // apply Complimentary Filter to keep the calculated velocity based on baro velocity (i.e. near real velocity).
        // By using CF it's possible to correct the drift of integrated accZ (velocity) without loosing the phase, i.e without delay
        zVel = zVel * 0.985f + altVel * 0.015f;

        vario = zVel;
        applyDeadband(vario, 5);
        aa->SetAltitudeVario(vario);
        pidOffsetAlt -= constrain(conf->raw.pid[PIDALT].D * vario >> 4, -150, 150);
        // Serial.print(", accZ: ");
        // Serial.print(accZ);
        // Serial.print(", dt: ");
        // Serial.print(deltaTime);
        // Serial.print(", altVel: ");
        // Serial.print(altVel);
        // Serial.print(", zVel: ");
        // Serial.print(zVel);
        // Serial.print(", offsetD: ");
        // Serial.print(pidOffsetAlt);
    }

    if (conf->baroMode /* || conf.takeOffMode || conf.landingMode */) {
        // throttle compensation
        rcCommand[THROTTLE] = initialThrottleHold + pidOffsetAlt;
    }
}

uint16_t PIDController::MixPID(int8_t x, int8_t y, int8_t z) {
    return rcCommand[THROTTLE] + pidOffset[ROLL] * x + pidOffset[PITCH] * y + pidOffset[YAW] * z;
}