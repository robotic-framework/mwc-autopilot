//
// Created by 李翌文 on 2021/7/14.
//

#include "networks.h"
#include <iostream>

sockpp::tcp_connector conn;

bool initNetwork(string host, uint16_t port) {
    if (host.empty() || !port) {
        cerr << "Error connecting to server, must specify host and port" << endl;
        return false;
    }
    if (!conn.connect(sockpp::inet_address(host, port))) {
        cerr << "Error connecting to server at"
        << sockpp::inet_address(host, port)
        << "\n\t" << conn.last_error_str() << endl;
        return false;
    }
    return true;
}
