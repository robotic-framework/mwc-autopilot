//
// Created by 李翌文 on 2021/8/11.
//

#include "rc_sitl_implement.h"
#include "common_handler.h"
#include "networks.h"

RCSITLImplement::RCSITLImplement() = default;

void RCSITLImplement::update(uint32_t currentTime) {
    request(new msg_request_command);
    read((uint8_t *) command, (uint8_t *) handler.command, 24);
}

