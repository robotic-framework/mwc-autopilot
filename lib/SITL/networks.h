//
// Created by 李翌文 on 2021/7/14.
//

#ifndef AUTOPILOT_NETWORKS_H
#define AUTOPILOT_NETWORKS_H

#include <stdint.h>
#include <functional>
#include "sockpp/tcp_connector.h"
#include "sockpp/tcp_acceptor.h"
#include "message.h"

using namespace std;

extern sockpp::tcp_connector conn;

typedef std::function<void (uint8_t *buf, uint8_t length)> ParseFunc;

typedef std::function<uint8_t (message *msg, uint8_t *buf, uint8_t size)> PacketFunc;

bool initNetwork(const string &host, const uint16_t &port, ParseFunc parser, PacketFunc packager);

void send(message *msg);

#endif //AUTOPILOT_NETWORKS_H
