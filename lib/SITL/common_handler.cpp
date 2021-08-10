//
// Created by 李翌文 on 2021/7/27.
//

#include "common_handler.h"
#include "stdio.h"

CommonHandler handler;

void CommonHandler::msgSimImuHandler(msg_response_sim_imu msg) {
    read((uint8_t *) acc, (uint8_t *) msg.acc, 6);
    read((uint8_t *) gyro, (uint8_t *) msg.gyro, 6);
    read((uint8_t *) mag, (uint8_t *) msg.mag, 6);
    ct = msg.ct;
    cp = msg.cp;
    ccp = msg.ccp;
    read((uint8_t *) att, (uint8_t *) msg.att, 6);
    alt = msg.alt;
    vario = msg.vario;
}

void CommonHandler::msgSimAccHandler(msg_response_sim_acc msg) {

}
