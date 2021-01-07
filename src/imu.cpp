#include "imu.h"

int32_t __attribute__((noinline)) mul(int16_t a, int16_t b)
{
    int32_t r;
    MultiS16X16to32(r, a, b); //r = (int32_t)a*b; without asm requirement
    return r;
}

// Rotate Estimated vector(s) with small angle approximation, according to the gyro data
void rotateV32(t_int32_t_vector *v, int16_t *delta)
{
    int16_t X = v->V16.X;
    int16_t Y = v->V16.Y;
    int16_t Z = v->V16.Z;

    v->V32.Z -= mul(delta[ROLL], X) + mul(delta[PITCH], Y);
    v->V32.X += mul(delta[ROLL], Z) - mul(delta[YAW], Y);
    v->V32.Y += mul(delta[PITCH], Z) + mul(delta[YAW], X);
}

//return angle , unit: 1/10 degree
int16_t _atan2(int32_t y, int32_t x)
{
    float z = y;
    int16_t a;
    uint8_t c;
    c = abs(y) < abs(x);
    if (c)
    {
        z = z / x;
    }
    else
    {
        z = x / z;
    }
    a = 2046.43 * (z / (3.5714 + z * z));
    if (c)
    {
        if (x < 0)
        {
            if (y < 0)
                a -= 1800;
            else
                a += 1800;
        }
    }
    else
    {
        a = 900 - a;
        if (y < 0)
            a -= 1800;
    }
    return a;
}

float invSqrt(float x)
{
    union
    {
        int32_t i;
        float f;
    } conv;
    conv.f = x;
    conv.i = 0x5f1ffff9 - (conv.i >> 1);
    return conv.f * (1.68191409f - 0.703952253f * x * conv.f * conv.f);
}

IMU::IMU()
{
    estimatedGyroData = {0, 0, (int32_t)ACC_1G_LSB << 16};
#if !SENSOR_MAG
    estimatedMagData = {0, (int32_t)1 << 24, 0};
#endif
#if defined(ACC_ADXL345)
    acc = new ADXL345(ADXL345_DEVICE);
#endif
#if defined(ACC_BMA180)
    acc = new BMA180(BMA180_DEVICE);
#endif

#if defined(GYRO_ITG3205)
    gyro = new ITG3205(ITG3205_DEVICE);
#endif

#if defined(MAG_HMC5883L)
    mag = new HMC5883L(HMC5883L_DEVICE);
#endif

#if defined(BARO_BMP085)
    baro = new BMP085(BMP085_DEVICE);
#endif

    alt.Alt = 0;
    alt.Vario = 0;
}

void IMU::Init()
{
    Wire.begin();

    while (1)
    {
#if SENSOR_ACC
        acc->Init();
#endif

#if SENSOR_GYRO
        gyro->Init();
#endif

#if SENSOR_MAG
        mag->Init();
#endif

#if SENSOR_BARO
        baro->Init();
#endif

        if (I2C::GetErrCount() == 0)
        {
            break;
        }
        blinkLED(2, 100, 3);
        delay(1000);
    }

    blinkLED(2, 50, 10);
    LEDPIN_OFF;
}

void IMU::AccCalibration()
{
#if SENSOR_ACC
    acc->Calibration();
#endif

#if SENSOR_GYRO
    gyro->Calibration();
#endif
}

void IMU::MagCalibration()
{
#if SENSOR_MAG
    mag->Calibration();
#endif
}

void IMU::BaroCalibration()
{
#if SENSOR_BARO
    baro->Calibration(200);
#endif
}

// Attention: the 'length' must be consistent with the length of the array pointed to by the 'buf'
void IMU::GetRawData(int16_t *buf, uint8_t length)
{
    uint8_t stepLength = min(3, length);
    for (size_t i = 0; i < stepLength; i++)
    {
#if SENSOR_ACC || SENSOR_GYRO
        *(buf + i) = accSmooth[i];
#else
        *(buf + i) = 0;
#endif
    }
    if (stepLength < 3)
    {
        return;
    }

    length -= stepLength;
    stepLength = min(3, length);
    for (size_t i = 0; i < stepLength; i++)
    {
#if SENSOR_GYRO
        *(buf + 3 + i) = gyroWeighted[i];
#else
        *(buf + 3 + i) = 0;
#endif
    }

    if (stepLength < 3)
    {
        return;
    }

    length -= stepLength;
    stepLength = min(3, length);

#if SENSOR_MAG
    mag->GetData(buf + 6, stepLength);
#else
    for (size_t i = 0; i < stepLength; i++)
    {
        *(buf + 6 + i) = 0;
    }
#endif
}

void IMU::GetAccData(int16_t *buf, uint8_t length)
{
    length = min(length, 3);
    for (size_t i = 0; i < length; i++)
    {
#if SENSOR_ACC
        *(buf + i) = accSmooth[i];
#else
        *(buf + i) = 0;
#endif
    }
}

void IMU::GetGyroData(int16_t *buf, uint8_t length)
{
    length = min(length, 3);
    for (size_t i = 0; i < length; i++)
    {
#if SENSOR_GYRO
        *(buf + i) = gyroWeighted[i];
#else
        *(buf + i) = 0;
#endif
    }
}

void IMU::GetMagData(int16_t *buf, uint8_t length)
{
#if SENSOR_MAG
    mag->GetData(buf, length);
#else
    length = min(length, 3);
    for (size_t i = 0; i < length; i++)
    {
        *(buf + i) = 0;
    }
#endif
}

void IMU::GetBaroData(int16_t *ct, int32_t *cp)
{
#if SENSOR_BARO
    *ct = baro->GetCTData();
    *cp = baro->GetCPData();
#else
    *ct = 0;
    *cp = 0;
#endif
}

void IMU::GetAttitude(int16_t *buf, uint8_t length)
{
    if (length < 3)
    {
        if (length > 1)
        {
            *buf = att.Angle[ROLL];
            *(buf + 1) = att.Angle[PITCH];
        }
        else
        {
            *buf = att.Angle[ROLL];
        }
    }
    else
    {
        *buf = att.Angle[ROLL];
        *(buf + 1) = att.Angle[PITCH];
        *(buf + 2) = att.Heading;
    }
}

void IMU::GetAltitude(int32_t *alt, int16_t *vario)
{
    *alt = this->alt.Alt;
    *vario = this->alt.Vario;
}

void IMU::GetAltitude(int32_t *alt)
{
    *alt = this->alt.Alt;
}

void IMU::SetAltitudeVario(int16_t vario)
{
    alt.Vario = vario;
}

void IMU::UpdateAcc(uint32_t currentTime)
{
#if SENSOR_ACC
    // update sensors data
    acc->Update(currentTime);
#endif
}

void IMU::UpdateGyro(uint32_t currentTime)
{
#if SENSOR_GYRO
    // calc gyro weights
    gyro->Update(currentTime);
    gyro->GetData(gyroWeight, 3);

    // update gyroscope
    int16_t currentGyro[3];
    gyro->Update(currentTime);
    gyro->GetData(currentGyro, 3);
    for (size_t i = 0; i < 3; i++)
    {
        gyroWeight[i] = currentGyro[i] + gyroWeight[i];
        gyroWeighted[i] = (gyroWeight[i] + gyroPrevWeight[i]) / 3;
        gyroPrevWeight[i] = gyroWeight[i] >> 1;
    }
#endif
}

void IMU::UpdateMag(uint32_t currentTime)
{
#if SENSOR_MAG
    mag->Update();
#endif
}

void IMU::UpdateBaro(uint32_t currentTime)
{
#if SENSOR_BARO
    baro->Update(currentTime);
#endif
}

void IMU::UpdateSonar(uint32_t currentTime)
{
#if SENSOR_SONAR
    sonar->Update();
#endif
}

void IMU::UpdateAttitude(uint32_t currentTime)
{
// check if is in calibration
#if SENSOR_ACC
    if (acc->IsCalibrating())
    {
        return;
    }
#endif
#if SENSOR_GYRO
    if (gyro->IsCalibrating())
    {
        return;
    }
#endif
#if SENSOR_MAG
    if (mag->IsCalibrating())
    {
        return;
    }
#endif

    uint8_t axis;
    float invGyro; // 1/|G|
    int32_t accMag = 0;
    int32_t accZTmp = 0;
    static int16_t accZoffset = 0;
    int16_t deltaAngle[3];

    if (estPrevTime == 0)
    {
        estPrevTime = currentTime;
        return;
    }

    // Serial.print(currentTime - estPrevTime);
    // Serial.println("> ");

    // unit: radian per bit, scaled by 2^16 for further multiplication
    // with a delta time of 3000 us, and GYRO scale of most gyros, scale = a little bit less than 1
    float scale = (currentTime - estPrevTime) * (GYRO_SCALE * 65536);
    estPrevTime = currentTime;

    // Serial.print(scale);
    // Serial.print("  ");
// Initialization
#if SENSOR_ACC && SENSOR_GYRO
    int16_t accData[3] = {0, 0, 0};
    int16_t gyroData[3];
    
    acc->GetData(accData, 3);
    gyro->GetData(gyroData, 3);
    for (axis = 0; axis < 3; axis++)
    {
        // valid as long as LPF_FACTOR is less than 15
        accSmooth[axis] = accLPF[axis] >> ACC_LPF_FACTOR;
        accLPF[axis] += accData[axis] - accSmooth[axis];
        // used to calculate later the magnitude of acc vector
        accMag += mul(accSmooth[axis], accSmooth[axis]);
        // unit: radian scaled by 2^16
        // imu.gyroADC[axis] is 14 bit long, the scale factor ensure deltaGyroAngle16[axis] is still 14 bit long
        deltaAngle[axis] = gyroData[axis] * scale;
        // Serial.print("(G:");
        // Serial.print(gyroData[axis]);
        // Serial.print(", D:");
        // Serial.print(deltaAngle[axis]);
        // Serial.print(", E:");
        // Serial.print(estimatedGyroData.A16[axis * 2 + 1]);
        // Serial.print(") ");
    }
#endif

    // we rotate the intermediate 32 bit vector with the radian vector (deltaAngle), scaled by 2^16
    // however, only the first 16 MSB of the 32 bit vector is used to compute the result
    // it is ok to use this approximation as the 16 LSB are used only for the complementary filter part
    rotateV32(&estimatedGyroData, deltaAngle);
    rotateV32(&estimatedMagData, deltaAngle);

    // Apply complimentary filter (Gyro drift correction)
#if SENSOR_MAG
    int16_t magData[3];
    mag->GetData(magData, 3);
#endif
    for (axis = 0; axis < 3; axis++)
    {
        // If accel magnitude >1.15G or <0.85G and ACC vector outside of the limit range => we neutralize the effect of accelerometers in the angle estimation.
        // To do that, we just skip filter, as EstV already rotated by Gyro
        if ((int16_t)(accMag >> 8) > (int16_t)(0.85 * ACC_1G_LSB * ACC_1G_LSB / 256) &&
            (int16_t)(accMag >> 8) < (int16_t)(1.15 * ACC_1G_LSB * ACC_1G_LSB / 256))
        {
            estimatedGyroData.A32[axis] +=
                (int32_t)(accSmooth[axis] - estimatedGyroData.A16[2 * axis + 1]) << (16 - GYR_CMPF_FACTOR);
        }
#if SENSOR_MAG
        estimatedMagData.A32[axis] += (int32_t)(magData[axis] - estimatedMagData.A16[2 * axis + 1]) << (16 - GYR_CMPFM_FACTOR);
#endif
        accZTmp += mul(accSmooth[axis], estimatedGyroData.A16[2 * axis + 1]);
    }

    if (estimatedGyroData.V16.Z > ACCZ_25DEG)
    {
        smallAngle25 = 1;
    }
    else
    {
        smallAngle25 = 0;
    }

    // Attitude of the estimated vector
    int32_t sqrtGyroXZ = mul(estimatedGyroData.V16.X, estimatedGyroData.V16.X) +
                         mul(estimatedGyroData.V16.Z, estimatedGyroData.V16.Z);
    invGyro = invSqrt(sqrtGyroXZ + mul(estimatedGyroData.V16.Y, estimatedGyroData.V16.Y));
    att.Angle[ROLL] = _atan2(estimatedGyroData.V16.X, estimatedGyroData.V16.Z);
    att.Angle[PITCH] = _atan2(estimatedGyroData.V16.Y, invSqrt(sqrtGyroXZ) * sqrtGyroXZ);
    att.Heading = _atan2(
        mul(estimatedMagData.V16.Z, estimatedGyroData.V16.X) - mul(estimatedMagData.V16.X, estimatedGyroData.V16.Z),
        (estimatedMagData.V16.Y * sqrtGyroXZ - (mul(estimatedMagData.V16.X, estimatedGyroData.V16.X) +
                                                mul(estimatedMagData.V16.Z, estimatedGyroData.V16.Z)) *
                                                   estimatedGyroData.V16.Y) *
            invGyro);

    // Serial.print("x: ");
    // Serial.print(estimatedGyroData.V16.X);
    // Serial.print(", z: ");
    // Serial.print(estimatedGyroData.V16.Z);
    // Serial.print(", roll: ");
    // Serial.print(att.Angle[ROLL]);
    // Serial.print(", pitch: ");
    // Serial.println(att.Angle[PITCH]);

#if SENSOR_MAG
    // att.Heading += 地磁偏角 // set from GUI
#endif
    att.Heading /= 10;

    accZ = accZTmp * invGyro;
    if (!arm)
    {
        accZoffset -= accZoffset >> 3;
        accZoffset += accZ;
    }
    accZ -= accZoffset >> 3;
}

void IMU::UpdateAltitude(uint32_t currentTime)
{
    if (!baro->GetCCPData())
    {
        return;
    }
    int32_t baroAlt = (log(baro->GetCCPData()) - baro->GetLogBaroGroundPressureSum()) * baro->GetBaroGroundTemperatureScale();
    alt.Alt = (alt.Alt * 7 + baroAlt) >> 3; // additional LPF to reduce baro noise (faster by 30 µs)
}

int16_t IMU::GetACCZ()
{
    return accZ;
}