//
// Created by 李翌文 on 2021/7/14.
//

#ifndef AUTOPILOT_NETWORKS_H
#define AUTOPILOT_NETWORKS_H

#include <stdint.h>
#include "sockpp/tcp_connector.h"

using namespace std;

extern sockpp::tcp_connector conn;

bool initNetwork(string host, uint16_t port);

#endif //AUTOPILOT_NETWORKS_H
