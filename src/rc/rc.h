//
// Created by 李翌文 on 2021/8/11.
//

#ifndef AUTOPILOT_RC_H
#define AUTOPILOT_RC_H

#include "stdint.h"
#include "type_def.h"

class RC {
protected:
    int16_t command[RC_AXIS_COUNT]{};
public:
    RC() {
        for (short & i : command) {
            i = 0;
        }
    }

    virtual void update(uint32_t currentTime) = 0;

    int16_t getCommand(e_rc_axis axis) {
        return command[axis];
    }

    void setCommand(e_rc_axis axis, int16_t value) {
        command[axis] = value;
    }
};

#endif //AUTOPILOT_RC_H
