#include "motors.h"

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
  this->WriteMotorsTrottle(MINCOMMAND);
}

uint16_t *Motors::GetMotors()
{
  return this->motors;
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
    OCR1A = this->motors[0]>>3; //  pin 9
  }
  if (this->motorCount > 1)
  {
    OCR1B = this->motors[1]>>3; //  pin 10
  }
  if (this->motorCount > 2)
  {
    OCR2A = this->motors[2]>>3; //  pin 11
  }
  if (this->motorCount > 3)
  {
    OCR2B = this->motors[3]>>3; //  pin 3
  }
}