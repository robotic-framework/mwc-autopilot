//
// Created by 李翌文 on 2021/8/11.
//

#ifndef AUTOPILOT_RC_COMMANDER_H
#define AUTOPILOT_RC_COMMANDER_H

#include "stdint.h"
#include "config.h"
#include "type_def.h"
#include "rc.h"

class RCCommander {
private:
    RC *rc = nullptr;

public:
    void init();
    void update(uint32_t currentTime);
    int16_t getCommand(e_rc_axis axis);

#if defined(TEST_RCCOMMAND)
    void setCommand(e_rc_axis axis, int16_t value);
#endif
};


#endif //AUTOPILOT_RC_COMMANDER_H
