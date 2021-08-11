//
// Created by 李翌文 on 2021/8/11.
//

#include "rc_commander.h"
#include "config.h"

#if defined(SITL)

#include "rc_sitl_implement.h"

#else
#include "rc_impl.h"
#endif

void RCCommander::init() {
#if defined(SITL)
    rc = new RCSITLImplement();
#else
    rc = new RCImpl();
#endif
}

void RCCommander::update(uint32_t currentTime) {
    rc->update(currentTime);
}

int16_t RCCommander::getCommand(e_rc_axis axis) {
    return rc->getCommand(axis);
}

#if defined(TEST_RCCOMMAND)

void RCCommander::setCommand(e_rc_axis axis, int16_t value) {
    return rc->setCommand(axis, value);
}

#endif

