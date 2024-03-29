//
// Created by 李翌文 on 2021/7/27.
//

#ifndef AUTOPILOT_HANDLER_H
#define AUTOPILOT_HANDLER_H

#include "message.h"

class MsgHandler {
public:
    virtual void msgSimImuHandler(msg_response_sim_imu msg) = 0;

    virtual void msgSimCommandHandler(msg_response_command msg) = 0;
};

#endif //AUTOPILOT_HANDLER_H
