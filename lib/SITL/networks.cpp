//
// Created by 李翌文 on 2021/7/14.
//

#include "networks.h"
#include "config.h"
#include <iostream>
#include <thread>

sockpp::tcp_connector conn;
PacketFunc requestPackager;
ParseFunc responseParser;

void connectSimulator(const string &host, const uint16_t &port);

void readSimulator();

void receiveStream();

bool initNetwork(const string &host, const uint16_t &port, ParseFunc receiver, PacketFunc packager) {
    if (host.empty() || !port) {
        cerr << "Unable to start server, must specify host and port" << endl;
        return false;
    }

    requestPackager = packager;
    responseParser = receiver;

    thread connectThr(connectSimulator, host, port);
    connectThr.detach();
    return true;
}

void request(message *msg) {
    if (conn.is_connected()) {
        uint8_t data[255];
        uint8_t length = requestPackager(msg, data, sizeof(data));
        conn.write(data, length);
    }
}

void connectSimulator(const string &host, const uint16_t &port) {
    cout << "connectSimulator host: " << host << ", port: " << port << endl;
    while (true) {
        try {
            if (!conn.connect(sockpp::inet_address(host, port))) {
                cerr << "Unable to connecting to Simulator server at "
                     << sockpp::inet_address(host, port)
                     << "\n\twith message: \"" << conn.last_error_str() << "\"" << endl;
                sleep(1);
            } else {
                cout << "Simulator server connected" << endl;

                thread receiveThr(receiveStream);
                receiveThr.detach();
                break;
            }
        } catch (...) {
            cout << "connectSimulator oops!" << endl;
        }
    }
}

void receiveStream() {
    int length;
    uint8_t buf[255];
    try {
        while ((length = conn.read(buf, sizeof(buf))) > 0) {
            responseParser(buf, length);
        }
    } catch (...) {
        cout << "receiveStream oops!" << endl;
    }
}
