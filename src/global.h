#if !defined(GLOBAL_H_)
#define GLOBAL_H_

#include "config.h"
#if defined(SITL)
#include "ardu_sitl.h"
#else
#include <Arduino.h>
#endif

#include "stdint.h"
#include "definition.h"
#include "configuration/configuration.h"
#include "acs/controller.h"
#include "rc/rc_commander.h"
#if GPS_ENABLED
#include "nav/navigation.h"
#endif

extern ACSController acs;
extern uint16_t cycleTime;
extern Configuration conf;
extern RCCommander rc;

#if GPS_ENABLED
extern Navigation nav;
#endif

#endif // GLOBAL_H_
