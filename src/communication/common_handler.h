//
// Created by 李翌文 on 2021/7/27.
//

#ifndef AUTOPILOT_COMMON_HANDLER_H
#define AUTOPILOT_COMMON_HANDLER_H

#include "handler.h"

class CommonHandler: public MsgHandler {
public:
    void msgSimImuHandler(msg_sim_imu msg) override;
    void msgSimAccHandler(msg_sim_acc msg) override;
};


#endif //AUTOPILOT_COMMON_HANDLER_H
