//
// Created by 李翌文 on 2021/7/21.
//

#ifndef AUTOPILOT_MESSAGE_H
#define AUTOPILOT_MESSAGE_H

#include <stdint.h>

static uint8_t __attribute__((noinline)) read(uint8_t *target, uint8_t *source, uint8_t sourceLength) {
    uint8_t length = sourceLength;
    while (sourceLength--)
        *target++ = *source++;
    return length;
}

static uint8_t __attribute__((noinline))
read(uint8_t *target, uint8_t *source, uint8_t sourceOffset, uint8_t sourceLength) {
    uint8_t length = sourceLength;
    source += sourceOffset;
    while (sourceLength--)
        *target++ = *source++;
    return length;
}

static uint8_t __attribute__((noinline))
read(uint8_t *target, uint8_t targetOffset, uint8_t *source, uint8_t sourceOffset, uint8_t sourceLength) {
    uint8_t length = sourceLength;
    source += sourceOffset;
    target += targetOffset;
    while (sourceLength--)
        *target++ = *source++;
    return length;
}

typedef enum : uint8_t {
    TYPE_SIM_IMU,
    TYPE_SIM_ACC,
    TYPE_SIM_GYRO,
    TYPE_SIM_MAG,
    TYPE_SIM_BAROMETER,
    TYPE_SIM_CONTROL,
} msg_type_e;

inline const char *msg_type_e_to_string(msg_type_e v) {
    switch (v) {
        case TYPE_SIM_IMU:
            return "TYPE_SIM_IMU";
        case TYPE_SIM_ACC:
            return "TYPE_SIM_ACC";
        case TYPE_SIM_GYRO:
            return "TYPE_SIM_GYRO";
        case TYPE_SIM_MAG:
            return "TYPE_SIM_MAG";
        case TYPE_SIM_BAROMETER:
            return "TYPE_SIM_BAROMETER";
        case TYPE_SIM_CONTROL:
            return "TYPE_SIM_CONTROL";
        default:
            return "[Unknown msg_type]";
    }
}

struct message {
    msg_type_e type;

    explicit message(msg_type_e t) : type(t) {}

    virtual void decode(uint8_t *buf, uint8_t length) = 0;

    virtual uint8_t encode(uint8_t *buf, uint8_t maxLength) = 0;
};

struct msg_request_sim_imu : message {
    msg_request_sim_imu() : message(TYPE_SIM_IMU) {}

    void decode(uint8_t *buf, uint8_t length) override;

    uint8_t encode(uint8_t *buf, uint8_t maxLength) override;
};

// length 28
struct msg_response_sim_imu : message {
    int16_t acc[3];
    int16_t gyro[3];
    int16_t mag[3];
    int16_t ct;
    int32_t cp;
    int32_t ccp;
    int16_t att[3];
    int32_t alt;
    int16_t vario;

    msg_response_sim_imu() : message(TYPE_SIM_IMU) {}

    void decode(uint8_t *buf, uint8_t length) override;

    uint8_t encode(uint8_t *buf, uint8_t maxLength) override;
};

struct msg_response_sim_acc : message {
    int16_t acc[3];

    msg_response_sim_acc() : message(TYPE_SIM_ACC) {}

    void decode(uint8_t *buf, uint8_t length) override;

    uint8_t encode(uint8_t *buf, uint8_t maxLength) override;
};

struct msg_response_sim_gyro : message {
    int16_t gyro[3];

    msg_response_sim_gyro() : message(TYPE_SIM_GYRO) {}

    void decode(uint8_t *buf, uint8_t length) override;

    uint8_t encode(uint8_t *buf, uint8_t maxLength) override;
};

struct msg_request_control : message {
    uint16_t motors[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    msg_request_control() : message(TYPE_SIM_CONTROL) {}

    void decode(uint8_t *buf, uint8_t length) override;

    uint8_t encode(uint8_t *buf, uint8_t maxLength) override;
};

#endif //AUTOPILOT_MESSAGE_H
