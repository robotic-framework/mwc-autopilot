#if !defined(GLOBAL_H_)
#define GLOBAL_H_

#include "config.h"
#if defined(SITL)
#include "SITL/ardu_sitl.h"
#else
#include <Arduino.h>
#endif

#include "stdint.h"
#include "definition.h"
#include "configuration/configuration.h"
#include "acs/controller.h"
#if GPS_ENABLED
#include "nav/navigation.h"
#endif

#endif // GLOBAL_H_
