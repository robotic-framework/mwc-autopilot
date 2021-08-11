//
// Created by 李翌文 on 2021/8/11.
//

#ifndef AUTOPILOT_RC_SITL_IMPLEMENT_H
#define AUTOPILOT_RC_SITL_IMPLEMENT_H

#include "rc/rc.h"

class RCSITLImplement : public RC {
public:
    RCSITLImplement();
    void update(uint32_t currentTime) override;
};


#endif //AUTOPILOT_RC_SITL_IMPLEMENT_H
