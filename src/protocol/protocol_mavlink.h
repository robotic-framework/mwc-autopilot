//
// Created by 李翌文 on 2021/8/26.
//

#ifndef AUTOPILOT_PROTOCOL_MAVLINK_H
#define AUTOPILOT_PROTOCOL_MAVLINK_H

#include "protocol.h"
#include "handler.h"
#include "common/mavlink.h"

class ProtocolMavlink : public Protocol {
private:
    mavlink_message_t msg;
    mavlink_status_t status;

    MsgHandler *handler;

protected:
    bool decode(uint8_t c) override;

    uint8_t encode(message *msg, uint8_t *buf, uint8_t maxLength) override;

public:
    ProtocolMavlink(MsgHandler *msgHandler);

    void processPacket(uint8_t *buffer, uint8_t length) override;
};


#endif //AUTOPILOT_PROTOCOL_MAVLINK_H
