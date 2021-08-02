//
// Created by 李翌文 on 2021/7/27.
//

#include "common_handler.h"
#include "stdio.h"

CommonHandler handler;

void CommonHandler::msgSimImuHandler(msg_response_sim_imu msg) {
    printf("acc:[roll=%d, pitch=%d, yaw=%d]\n", msg.acc[0], msg.acc[1], msg.acc[2]);
    printf("gyro:[roll=%d, pitch=%d, yaw=%d]\n", msg.gyro[0], msg.gyro[1], msg.gyro[2]);
    printf("mag:[roll=%d, pitch=%d, yaw=%d]\n", msg.mag[0], msg.mag[1], msg.mag[2]);
    printf("barometer:[ct=%d, cp=%d, ccp=%d]\n", msg.ct, msg.cp, msg.ccp);
    printf("att:[roll=%d, pitch=%d, yaw=%d]\n", msg.att[0], msg.att[1], msg.att[2]);
    printf("alt:[alt=%d, vario=%d]\n", msg.alt, msg.vario);

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
