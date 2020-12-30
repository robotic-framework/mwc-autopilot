#if !defined(ITG3205_H_)
#define ITG3205_H_

#include <stdint.h>
#include <Arduino.h>
#include "gyroscope.h"

#define ITG3205_DEVICE 0x68
#define ITG3205_GYRO_TO_READ 6
#define ITG3205_TEMP_TO_READ 2

/* ---- Registers ---- */
#define ITG3205_WHO_AM_I 0x00   // RW   SETUP: I2C address
#define ITG3205_SMPLRT_DIV 0x15 // RW   SETUP: Sample Rate Divider
#define ITG3205_DLPF_FS 0x16    // RW   SETUP: Digital Low Pass Filter/ Full Scale range
#define ITG3205_INT_CFG 0x17    // RW   Interrupt: Configuration
#define ITG3205_INT_STATUS 0x1A // R	Interrupt: Status
#define ITG3205_TEMP_OUT 0x1B   // R	SENSOR: Temperature 2bytes
#define ITG3205_GYRO_XOUT 0x1D  // R	SENSOR: Gyro X 2bytes
#define ITG3205_GYRO_YOUT 0x1F  // R	SENSOR: Gyro Y 2bytes
#define ITG3205_GYRO_ZOUT 0x21  // R	SENSOR: Gyro Z 2bytes
#define ITG3205_PWR_MGM 0x3E    // RW	Power Management

/* ---- bit maps ---- */
#define ITG3205_DLPFFS_FS_SEL 0x18           // 00011000
#define ITG3205_DLPFFS_DLPF_CFG 0x07         // 00000111
#define ITG3205_INTCFG_ACTL 0x80             // 10000000
#define ITG3205_INTCFG_OPEN 0x40             // 01000000
#define ITG3205_INTCFG_LATCH_INT_EN 0x20     // 00100000
#define ITG3205_INTCFG_INT_ANYRD_2CLEAR 0x10 // 00010000
#define ITG3205_INTCFG_ITG_RDY_EN 0x04       // 00000100
#define ITG3205_INTCFG_RAW_RDY_EN 0x01       // 00000001
#define ITG3205_INTSTATUS_ITG_RDY 0x04       // 00000100
#define ITG3205_INTSTATUS_RAW_DATA_RDY 0x01  // 00000001
#define ITG3205_PWRMGM_HRESET 0x80           // 10000000
#define ITG3205_PWRMGM_SLEEP 0x40            // 01000000
#define ITG3205_PWRMGM_STBY_XG 0x20          // 00100000
#define ITG3205_PWRMGM_STBY_YG 0x10          // 00010000
#define ITG3205_PWRMGM_STBY_ZG 0x08          // 00001000
#define ITG3205_PWRMGM_CLK_SEL 0x07          // 00000111

class ITG3205 : public Gyroscope
{
private:
  /* data */
  uint8_t _buff[ITG3205_GYRO_TO_READ];
  int16_t _prevData[3] = {0, 0, 0};

protected:
  void getRawData() override;
  void init() override;

public:
  ITG3205(uint8_t address) : Gyroscope(address){};

  void Update(uint32_t currentTime);
};

#endif // ITG3205_H_
