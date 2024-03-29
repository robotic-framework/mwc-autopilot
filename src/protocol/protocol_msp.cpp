//
// Created by 李翌文 on 2021/7/21.
//

#include "protocol_msp.h"

#if defined(SITL)

#include "ardu_sitl.h"

#else
#include <Arduino.h>
#endif

#include <map>

#define INPUT_BUFFER_SIZE 64

std::map<msg_type_e, uint8_t> msg_type_msp_command_mapping = {
        {TYPE_SIM_IMU,     MSP_SIM_IMU},
        {TYPE_SIM_CONTROL, MSP_SIM_CONTROL},
        {TYPE_SIM_COMMAND, MSP_SIM_COMMAND}
};

std::map<uint8_t, msg_type_e> msp_command_msg_type_mapping = {
        {MSP_SIM_IMU,     TYPE_SIM_IMU},
        {MSP_SIM_CONTROL, TYPE_SIM_CONTROL},
        {MSP_SIM_COMMAND, TYPE_SIM_COMMAND}
};

bool ProtocolMSP::decode(uint8_t c) {
    switch (state) {
        case MSP_IDLE: {
            // expect '$'
            if (c == '$') {
                state = MSP_HEADER_START;
            }
            break;
        }
        case MSP_HEADER_START: {
            // expect 'M'
            state = (c == 'M') ? MSP_HEADER_M : MSP_IDLE;
            break;
        }
        case MSP_HEADER_M: {
            // expect '<'
            state = (c == '<') ? MSP_HEADER_ARROW : MSP_IDLE;
            break;
        }
        case MSP_HEADER_ARROW: {
            // expect size
            if (c > INPUT_BUFFER_SIZE) {
                // invalid size
                state = MSP_IDLE;
            } else {
                dataSize = c;
                checksum = c;
                state = MSP_HEADER_SIZE;
            }
            break;
        }
        case MSP_HEADER_SIZE: {
            // expect cmd
            checksum ^= c;
            dataOffset = 0;
            state = MSP_HEADER_CMD;
            break;
        }
        case MSP_HEADER_CMD: {
            if (dataOffset < dataSize) {
                checksum ^= c;
                dataOffset++;
            } else {
                // the last byte is checksum, check if equel
                state = MSP_IDLE;

                if (checksum == c) {
                    return true;
                }
            }
            break;
        }
    }
    return false;
}

void ProtocolMSP::processPacket(uint8_t *buffer, uint8_t length) {
    if (length < 6) {
        return;
    }
    msg_packet_t packet;
    uint8_t payload[length];

    // copy payload
    memcpy(payload, &buffer[5], length - 6);

    packet.length = buffer[3];
    packet.type = msp_command_msg_type_mapping[buffer[4]];
    packet.payload = payload;

    switch (packet.type) {
        case TYPE_SIM_IMU: {
            msg_response_sim_imu msg;
            msg.decode(packet.payload, packet.length);
            handler->msgSimImuHandler(msg);
            break;
        }
        case TYPE_SIM_COMMAND: {
            msg_response_command msg;
            msg.decode(packet.payload, packet.length);
            handler->msgSimCommandHandler(msg);
        }
    }
}

ProtocolMSP::ProtocolMSP(MsgHandler *msgHandler) :
        state(MSP_IDLE),
        dataSize(0),
        dataOffset(0),
        checksum(0),
        handler(msgHandler) {}

uint8_t ProtocolMSP::encode(message *msg, uint8_t *buf, uint8_t maxLength) {
    uint8_t offset = 0;
    uint8_t c = 0;
    uint8_t payload[255];
    uint8_t payloadLength = msg->encode(payload, sizeof(payload));

    read(buf, offset, (uint8_t *) "$M>", 0, 3);
    offset += 3;

    read(buf, offset, (uint8_t *) &payloadLength, 0, 1);
    offset++;

    read(buf, offset, (uint8_t *) &msg_type_msp_command_mapping[msg->type], 0, 1);
    offset++;

    read(buf, offset, payload, 0, payloadLength);
    offset += payloadLength;

    for (uint8_t i = 3; i < offset; i++) {
        c ^= buf[i];
    }

    read(buf, offset, (uint8_t *) &c, 0, 1);
    offset++;

    return offset;
}
