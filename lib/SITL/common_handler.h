//
// Created by 李翌文 on 2021/7/27.
//

#ifndef AUTOPILOT_COMMON_HANDLER_H
#define AUTOPILOT_COMMON_HANDLER_H

#include "protocol/handler.h"

class CommonHandler : public MsgHandler {
public:
    int16_t acc[3] = {0, 0, 0};
    int16_t gyro[3] = {0, 0, 0};
    int16_t mag[3] = {0, 0, 0};
    int16_t ct = 0;
    int32_t cp = 0;
    int32_t ccp = 0;
    int16_t att[3];
    int32_t alt;
    int16_t vario;

    int16_t command[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
public:
    void msgSimImuHandler(msg_response_sim_imu msg) override;

    void msgSimCommandHandler(msg_response_command msg) override;
};

extern CommonHandler handler;

#endif //AUTOPILOT_COMMON_HANDLER_H
