//
// Created by 李翌文 on 2021/7/21.
//

#include "protocol.h"

void Protocol::receiveStream(uint8_t *buf, uint8_t length) {
    for (int i = 0; i < length; i++) {
        buffer[bufferIndex] = *buf;
        bufferLength++;

        if (decode(*buf)) {
            processPacket(buffer, bufferLength);
            bufferIndex = 0;
            bufferLength = 0;
        } else {
            if (bufferIndex >= MAX_BUFFER_SIZE - 1) {
                bufferIndex = 0;
                bufferLength = 0;
            } else {
                bufferIndex++;
            }
        }
        buf++;
    }
}

uint8_t Protocol::packetRequest(message *msg, uint8_t *buf, uint8_t length) {
    return encode(msg, buf, length);
}
