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
#ifdef ACC_ADXL345
  acc = new ADXL345(ADXL345_DEVICE);
#endif

#ifdef GYRO_ITG3205
  gyro = new ITG3205(ITG3205_DEVICE);
#endif
}

void IMU::Init()
{
  Wire.begin();
  acc->Init();
  gyro->Init();
}

void IMU::Update()
{
  // update sensors data
  acc->Update();

  // calc gyro weights
  gyro->Update();
  gyro->GetData(gyroWeight, 3);
  // update gyroscope
  int16_t currentGyro[3];
  gyro->Update();
  gyro->GetData(currentGyro, 3);
  for (size_t i = 0; i < 3; i++)
  {
    gyroWeight[i] = currentGyro[i] + gyroWeight[i];
    gyroWeighted[i] = (gyroWeight[i] + gyroPrevWeight[i]) / 3;
    gyroPrevWeight[i] = gyroWeight[i] >> 1;
  }

  // calculate est attitude
  calcEstimatedAttitude();
}

void IMU::Calibration()
{
  blinkLED(2, 100, 5);
  acc->Calibration();
  gyro->Calibration();
}

// Attention: the 'length' must be consistent with the length of the array pointed to by the 'buf'
void IMU::GetRawData(int16_t *buf, uint8_t length)
{
  if (length < 3)
  {
    // acc->GetData(buf, length);
    for (size_t i = 0; i < 3; i++)
    {
      *(buf + i) = accSmooth[i];
    }
    return;
  }
  else
  {
    // acc->GetData(buf, length);
    for (size_t i = 0; i < 3; i++)
    {
      *(buf + i) = accSmooth[i];
    }
    length -= 3;
  }
  if (length >= 3)
  {
    // gyro->GetData(buf + 3, length);
    for (size_t i = 0; i < 3; i++)
    {
      *(buf + 3 + i) = gyroWeighted[i];
    }
    length -= 3;
  }
  if (length > 0)
  {
    // mag.GetData(buf + 6, length);
  }
}

void IMU::calcEstimatedAttitude()
{
  uint16_t currentTime = micros();
  uint8_t axis;
  float invGyro; // 1/|G|
  int16_t accData[3];
  int16_t gyroData[3];
  int32_t accMag = 0;
  int32_t accZTmp = 0;
  int16_t deltaAngle[3];

  // unit: radian per bit, scaled by 2^16 for further multiplication
  // with a delta time of 3000 us, and GYRO scale of most gyros, scale = a little bit less than 1
  float scale = (currentTime - estPrevTime) * (GYRO_SCALE * 65536);
  estPrevTime = currentTime;

  // Initialization
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
  }

  // we rotate the intermediate 32 bit vector with the radian vector (deltaAngle), scaled by 2^16
  // however, only the first 16 MSB of the 32 bit vector is used to compute the result
  // it is ok to use this approximation as the 16 LSB are used only for the complementary filter part
  rotateV32(&estimatedGyroData, deltaAngle);
  rotateV32(&estimatedMagData, deltaAngle);

  // Apply complimentary filter (Gyro drift correction)
  for (axis = 0; axis < 3; axis++)
  {
    // If accel magnitude >1.15G or <0.85G and ACC vector outside of the limit range => we neutralize the effect of accelerometers in the angle estimation.
    // To do that, we just skip filter, as EstV already rotated by Gyro
    if ((int16_t)(accMag >> 8) > (int16_t)(0.85 * ACC_1G_LSB * ACC_1G_LSB / 256) &&
        (int16_t)(accMag >> 8) < (int16_t)(1.15 * ACC_1G_LSB * ACC_1G_LSB / 256))
    {
      estimatedGyroData.A32[axis] += (int32_t)(accSmooth[axis] - estimatedGyroData.A16[2 * axis + 1]) << (16 - GYR_CMPF_FACTOR);
    }
    accZTmp += mul(accSmooth[axis], estimatedGyroData.A16[2 * axis + 1]);
    // estimatedMagData.A32[axis] += (int32_t)(magData[axis] - estimatedMagData.A16[2 * axis + 1]) << (16 - GYR_CMPFM_FACTOR);
  }
  // Serial.print(estimatedGyroData.V16.X);
  // Serial.print(":");
  // Serial.print(estimatedGyroData.V16.Y);
  // Serial.print(":");
  // Serial.println(estimatedGyroData.V16.Z);

  if (estimatedGyroData.V16.Z > ACCZ_25DEG)
  {
    smallAngle25 = 1;
  }
  else
  {
    smallAngle25 = 0;
  }

  // Attitude of the estimated vector
  int32_t sqrtGyroXZ = mul(estimatedGyroData.V16.X, estimatedGyroData.V16.X) + mul(estimatedGyroData.V16.Z, estimatedGyroData.V16.Z);
  invGyro = invSqrt(sqrtGyroXZ + mul(estimatedGyroData.V16.Y, estimatedGyroData.V16.Y));
  att.Angle[ROLL] = _atan2(estimatedGyroData.V16.X, estimatedGyroData.V16.Z);
  att.Angle[PITCH] = _atan2(estimatedGyroData.V16.Y, invSqrt(sqrtGyroXZ) * sqrtGyroXZ);

  Serial.print(att.Angle[ROLL]);
  Serial.print(":");
  Serial.println(att.Angle[PITCH]);
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
