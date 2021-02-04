#if !defined(DEF_H_)
#define DEF_H_

#include "stdint.h"
#include "config.h"
#include "motors.h"
#include "imu.h"
#if GPS_ENABLED
#include "navigation.h"
#endif
#include "configuration.h"

void
loadDefaults();

#endif // DEF_H_
