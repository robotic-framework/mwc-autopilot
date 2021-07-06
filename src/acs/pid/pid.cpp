#include "pid.h"
#include "acs/utils.h"

using namespace aa;

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
        isAltHoldModeChanged(true),
        initialThrottleHold(0) {
    lastAltHoldMode = conf->altHoldMode;
    this->conf = conf;
}

void PIDController::update(uint32_t currentTime) {
    uint8_t axis;
    int16_t error, errorAngle;
    int16_t rc;
    int16_t pidOffsetAlt = 0;
    static int16_t lastGyro[2] = {0, 0};
    static int16_t errorGyroI[2] = {0, 0};
    static int32_t errorGyroI_YAW = 0;
    static int16_t errorAngleI[2] = {0, 0};
    int16_t PTerm = 0, ITerm = 0, DTerm = 0, PTermACC = 0, ITermACC = 0;
    int16_t delta;

    static uint16_t previousTime = 0;
    uint16_t deltaTime;

    static int16_t delta1[2] = {0, 0}, delta2[2] = {0, 0};

    if (previousTime == 0) {
        previousTime = currentTime;
        return;
    }
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;

#if TEST_LOG_LEVEL > 0
    Log::debugStart();
    Log::debugln(">>>>>>>>>> Level PID update start <<<<<<<<<<");
#endif

    if (conf->horizonMode) {
        prop = min(max(abs(rcCommand[PITCH]), abs(rcCommand[ROLL])), 512);
    }

    int16_t gyroData[3];
    int16_t attitude[3];
    imu->getGyroData(gyroData, 3);
    aa->getAttitude(attitude, 3);

#if TEST_LOG_LEVEL > 0
    Log::debugStart();
    Log::debug("original imu data: gy_Roll: ");
    Log::debug(gyroData[ROLL]);
    Log::debug(", gy_Pitch: ");
    Log::debug(gyroData[PITCH]);
    Log::debug(", gy_Yaw: ");
    Log::debug(gyroData[YAW]);
    Log::debug(", att_Roll: ");
    Log::debug(attitude[ROLL]);
    Log::debug(", att_Pitch: ");
    Log::debug(attitude[PITCH]);
    Log::debug(", att_Yaw: ");
    Log::debugln(attitude[YAW]);
#endif

    // ROLL & PITCH
    for (axis = 0; axis < 2; axis++) {
        rc = rcCommand[axis] << 1;

        if (conf->angleMode || conf->horizonMode) {
            errorAngle = constrain(rc, -500, 500) - attitude[axis];
            errorAngleI[axis] = constrain(errorAngleI[axis] + errorAngle, -10000, 10000);

            PTermACC = mul(errorAngle, conf->raw.pid[PIDLEVEL].P) >> 7;

            int16_t limit = conf->raw.pid[PIDLEVEL].D * 5;
            PTermACC = constrain(PTermACC, -limit, limit);

            ITermACC = mul(errorAngleI[axis], conf->raw.pid[PIDLEVEL].I) >> 12;

            ITerm = ITermACC + ((ITerm - ITermACC) * prop >> 9);
            PTerm = PTermACC + ((PTerm - PTermACC) * prop >> 9);
        } else {
            error = rc - gyroData[axis];
            errorGyroI[axis] = constrain(errorGyroI[axis] + error, -16000, 16000);
            if (abs(gyroData[axis]) > 640) {
                errorGyroI[axis] = 0;
            }
            ITerm = (errorGyroI[axis] >> 7) * conf->raw.pid[axis].I >> 6;
            PTerm = mul(rc, conf->raw.pid[axis].P) >> 6;
        }

#if TEST_LOG_LEVEL > 0
        Log::debugStart();
        Log::debug("axis: ");
        Log::debug(axis);
        Log::debug(", rcCommand: ");
        Log::debug(rcCommand[axis]);
        Log::debug(", rc: ");
        Log::debug(rc);
        Log::debug(", attitude: ");
        Log::debug(attitude[axis]);
        Log::debug(", errorAngle: ");
        Log::debug(errorAngle);
        Log::debug(", errorAngleI: ");
        Log::debug(errorAngleI[axis]);
        Log::debug(", pid.I: ");
        Log::debug(conf->raw.pid[PIDLEVEL].I);
        Log::debug(", ITermACC: ");
        Log::debug(ITermACC);
        Log::debug(", prop: ");
        Log::debug(prop);
        Log::debug(", ITerm: ");
        Log::debugln(ITerm);
#endif

        // PTerm -= mul(gyroData[axis], dynP[axis]) >> 6;

        delta = gyroData[axis] - lastGyro[axis];
        lastGyro[axis] = gyroData[axis];
        DTerm = delta1[axis] + delta2[axis] + delta;
        delta2[axis] = delta1[axis];
        delta1[axis] = delta;

        // DTerm = mul(DTerm, dynD[axis]) >> 5;
        pidOffset[axis] = PTerm + ITerm - DTerm;

#if TEST_LOG_LEVEL > 0
        Log::debugStart();
        Log::debug("axis: ");
        Log::debug(axis);
        Log::debug(", P: ");
        Log::debug(PTerm);
        Log::debug(", I: ");
        Log::debug(ITerm);
        Log::debug(", D: ");
        Log::debugln(DTerm);
#endif
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

#if TEST_LOG_LEVEL > 0
    Log::debugStart();
    Log::debug("axis: ");
    Log::debug(YAW);
    Log::debug(", P: ");
    Log::debug(PTerm);
    Log::debug(", I: ");
    Log::debug(ITerm);
    Log::debug(", D: ");
    Log::debugln(DTerm);

    Log::debugStart();
    Log::debug("Roll: ");
    Log::debug(pidOffset[ROLL]);
    Log::debug(", Pitch: ");
    Log::debug(pidOffset[PITCH]);
    Log::debug(", Yaw: ");
    Log::debugln(pidOffset[YAW]);

    Log::debugStart();
    Log::debugln(">>>>>>>>>> Level PID update end <<<<<<<<<<");

    Log::debugStart();
    Log::debugln(">>>>>>>>>> AltHold PID update start <<<<<<<<<<");
#endif

    // ALT HOLD
    if (conf->altHoldMode != lastAltHoldMode) {
        isAltHoldModeChanged = true;
        lastAltHoldMode = conf->altHoldMode;
    }
    if (isAltHoldModeChanged) {
        if (conf->altHoldMode) {
            errorAltI = 0;
        }
        initialThrottleHold = rcCommand[THROTTLE];
        isAltHoldModeChanged = false;
    }
    if (conf->altHoldMode) {
        int32_t alt;
        int16_t vario;
        aa->getAltitude(&alt, &vario);
        int16_t errorAlt = constrain(conf->altHold - alt, -300, 300);

#if TEST_LOG_LEVEL > 0
        Log::debugStart();
        Log::debug("hold: ");
        Log::debug(conf->altHold);
        Log::debug(", alt: ");
        Log::debugln(alt);
#endif

        applyDeadband(errorAlt, 10);
        pidOffsetAlt = constrain(conf->raw.pid[PIDALT].P * errorAlt >> 7, -150, 150);

        errorAltI += conf->raw.pid[PIDALT].I * errorAlt >> 6;
        errorAltI = constrain(errorAltI, -30000, 30000);
        pidOffsetAlt += errorAltI >> 9;

        int16_t accZ = aa->getAccZ();
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
        aa->setAltitudeVario(vario);
        pidOffsetAlt -= constrain(conf->raw.pid[PIDALT].D * vario >> 4, -150, 150);

#if TEST_LOG_LEVEL > 0
        Log::debugStart();
        Log::debug(", accZ: ");
        Log::debug(accZ);
        Log::debug(", dt: ");
        Log::debug(deltaTime);
        Log::debug(", altVel: ");
        Log::debug(altVel);
        Log::debug(", zVel: ");
        Log::debug(zVel);
        Log::debug(", offsetD: ");
        Log::debugln(pidOffsetAlt);
#endif
    }

    if (conf->altHoldMode /* || conf.takeOffMode || conf.landingMode */) {
        // throttle compensation
        rcCommand[THROTTLE] = initialThrottleHold + pidOffsetAlt;
    }

#if TEST_LOG_LEVEL > 0
    Log::debugStart();
    Log::debugln(">>>>>>>>>> AltHold PID update end <<<<<<<<<<");
#endif
}

uint16_t PIDController::mixPID(int8_t x, int8_t y, int8_t z) {
#if TEST_LOG_LEVEL > 0
    Log::debugStart();
    Log::debugln(">>>>>>>>>> Mix motors with PID start <<<<<<<<<<");
    Log::debug("rcCommand[throttle]: ");
    Log::debug(rcCommand[THROTTLE]);
    Log::debug(", pidOffset[roll]: ");
    Log::debug(pidOffset[ROLL]);
    Log::debug(", x: ");
    Log::debug(x);
    Log::debug(", pidOffset[pitch]: ");
    Log::debug(pidOffset[PITCH]);
    Log::debug(", y: ");
    Log::debug(y);
    Log::debug(", pidOffset[yaw]: ");
    Log::debug(pidOffset[YAW]);
    Log::debug(", z: ");
    Log::debug(z);
    Log::debug(", motors: ");
    Log::debugln(rcCommand[THROTTLE] + pidOffset[ROLL] * x + pidOffset[PITCH] * y + pidOffset[YAW] * z);
    Log::debugStart();
    Log::debugln(">>>>>>>>>> Mix motors with PID end <<<<<<<<<<");
    Log::debug("\n\n\n");
#endif
    return rcCommand[THROTTLE] + pidOffset[ROLL] * x + pidOffset[PITCH] * y + pidOffset[YAW] * z;
}