//
// Created by 李翌文 on 2021/8/26.
//

#include "protocol_mavlink.h"

ProtocolMavlink::ProtocolMavlink(MsgHandler *msgHandler) {
    handler = msgHandler;
}

bool ProtocolMavlink::decode(uint8_t c) {
    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
        return true;
    }
    return false;
}

uint8_t ProtocolMavlink::encode(message *msg, uint8_t *buf, uint8_t maxLength) {
    return 0;
}

void ProtocolMavlink::processPacket(uint8_t *buffer, uint8_t length) {
    switch (msg.msgid) {
        case MAVLINK_MSG_ID_HIL_SENSOR:
            mavlink_hil_sensor_t sensor;
            mavlink_msg_hil_sensor_decode(&msg, &sensor);

            msg_response_sim_imu data;
            data.gyro[0] = (int16_t)sensor.xgyro;
            data.gyro[1] = (int16_t)sensor.ygyro;
            data.gyro[2] = (int16_t)sensor.zgyro;
            handler->msgSimImuHandler(data);
            break;
    }
}
