//
// Created by 李翌文 on 2021/7/21.
//

#ifndef AUTOPILOT_PROTOCOL_H
#define AUTOPILOT_PROTOCOL_H

#include <stdint.h>
#include "message.h"

typedef struct {
    uint8_t length;
    msg_type_e type;
    uint8_t *payload;
} msg_packet_t;

#define MAX_BUFFER_SIZE 1024

class Protocol {
private:
    uint8_t buffer[MAX_BUFFER_SIZE];
    uint16_t bufferIndex = 0;
    uint16_t bufferLength = 0;

protected:
    virtual bool decode(uint8_t c) = 0;

    virtual uint8_t encode(message *msg, uint8_t *buf, uint8_t maxLength) = 0;

    virtual void processPacket(uint8_t *buf, uint8_t length) = 0;

public:
    void receiveStream(uint8_t *buf, uint8_t length);

    uint8_t packetRequest(message *msg, uint8_t *buf, uint8_t length);
};


#endif //AUTOPILOT_PROTOCOL_H
