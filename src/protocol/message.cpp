//
// Created by 李翌文 on 2021/7/21.
//

#include "message.h"

void msg_response_sim_imu::decode(uint8_t *buf, uint8_t length) {
    if (length < 40) return;

    read((uint8_t *) acc, buf, 0, 6);
    read((uint8_t *) gyro, buf, 6, 6);
    read((uint8_t *) mag, buf, 12, 6);
    read((uint8_t *) &ct, buf, 18, 2);
    read((uint8_t *) &cp, buf, 20, 4);
    read((uint8_t *) &ccp, buf, 24, 4);
    read((uint8_t *) &att, buf, 28, 6);
    read((uint8_t *) &alt, buf, 34, 4);
    read((uint8_t *) &vario, buf, 38, 2);
}

uint8_t msg_response_sim_imu::encode(uint8_t *buf, uint8_t maxLength) {
    uint8_t length = read(buf, (uint8_t *) acc, 0, 6);
    length += read(buf, (uint8_t *) gyro, 6, 6);
    length += read(buf, (uint8_t *) mag, 12, 6);
    length += read(buf, (uint8_t *) &ct, 18, 2);
    length += read(buf, (uint8_t *) &cp, 20, 4);
    length += read(buf, (uint8_t *) &ccp, 24, 4);
    length += read(buf, (uint8_t *) &att, 28, 6);
    length += read(buf, (uint8_t *) &alt, 34, 4);
    length += read(buf, (uint8_t *) &vario, 38, 2);

    return length;
}

uint8_t msg_request_sim_imu::encode(uint8_t *buf, uint8_t maxLength) {
    return 0;
}

void msg_request_sim_imu::decode(uint8_t *buf, uint8_t length) {}

void msg_request_control::decode(uint8_t *buf, uint8_t length) {}

uint8_t msg_request_control::encode(uint8_t *buf, uint8_t maxLength) {
    return read(buf, (uint8_t *)motors, 0, 16);
}

void msg_request_command::decode(uint8_t *buf, uint8_t length) {
    if (length < 24) return;
    read((uint8_t *) command, buf, 0, 24);
}

uint8_t msg_request_command::encode(uint8_t *buf, uint8_t maxLength) {
    return read(buf, (uint8_t *) command, 0, 24);
}
