//
// Created by 李翌文 on 2021/7/27.
//

#include "common_handler.h"
#include "stdio.h"

void CommonHandler::msgSimImuHandler(msg_sim_imu msg) {
    printf("acc:[roll=%d, pitch=%d, yaw=%d]\n", msg.acc[0], msg.acc[1], msg.acc[2]);
    printf("gyro:[roll=%d, pitch=%d, yaw=%d]\n", msg.gyro[0], msg.gyro[1], msg.gyro[2]);
    printf("mag:[roll=%d, pitch=%d, yaw=%d]\n", msg.mag[0], msg.mag[1], msg.mag[2]);
    printf("barometer:[ct=%d, cp=%d, ccp=%d]\n", msg.ct, msg.cp, msg.ccp);
}

void CommonHandler::msgSimAccHandler(msg_sim_acc msg) {

}
