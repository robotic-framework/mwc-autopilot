//
// Created by 李翌文 on 2021/8/11.
//

#ifndef AUTOPILOT_RC_IMPL_H
#define AUTOPILOT_RC_IMPL_H

#include "rc.h"

class RCImpl : public RC {
public:
    RCImpl();

    void update(uint32_t currentTime) override;
};


#endif //AUTOPILOT_RC_IMPL_H
