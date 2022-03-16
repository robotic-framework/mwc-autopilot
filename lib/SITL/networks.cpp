//
// Created by 李翌文 on 2021/7/14.
//

#include "networks.h"
#include <iostream>
#include <thread>

sockpp::udp_socket conn;
PacketFunc requestPackager;
ParseFunc responseParser;
sockpp::inet_address outAddr;

void receiveStream();

bool initNetwork(const string &host, const uint16_t &in_port, const uint16_t &out_port, ParseFunc receiver, PacketFunc packager) {
    if (host.empty() || !in_port || !out_port) {
        cerr << "Unable to start UDP server, must specify host and in_port and out_port" << endl;
        return false;
    }

    if (!conn) {
        cerr << "Unable to start UDP server, " << conn.last_error_str() << endl;
        return false;
    }

    sockpp::inet_address inAddr("127.0.0.1", in_port);
    outAddr.create(host, out_port);
    if (!conn.bind(inAddr)) {
        cerr << "Unable to start UDP server, cannot bind to " << inAddr.to_string() << endl;
        return false;
    }
    cout << "UDP server bind at " << inAddr.to_string() << endl;

    requestPackager = packager;
    responseParser = receiver;

    cout << "UDP server initialized." << endl;

    thread receiveThr(receiveStream);
    receiveThr.detach();
    return true;
}

void request(message *msg) {
    if (!conn) return;

    uint8_t data[255];
    uint8_t length = requestPackager(msg, data, sizeof(data));
    conn.send_to(data, length, outAddr);
}

void receiveStream() {
    int length;
    uint8_t buf[255];

    cout << "UDP server start listening..." << endl;
    try {
        while ((length = conn.recv_from(buf, sizeof(buf))) > 0) {
            responseParser(buf, length);
        }
    } catch (...) {
        cout << "receiveStream oops!" << endl;
    }
}
