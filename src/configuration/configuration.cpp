//
// Created by 李翌文 on 2021/2/4.
//

#include <EEPROM.h>
#include "configuration.h"

Configuration::Configuration() : arm(false),
                                 angleMode(true),
                                 horizonMode(true),
                                 baroMode(false),
                                 altHold(0) {

}

void Configuration::loadDefault() {
    raw.pid[ROLL].P = 33;
    raw.pid[ROLL].I = 30;
    raw.pid[ROLL].D = 23;
    raw.pid[PITCH].P = 33;
    raw.pid[PITCH].I = 30;
    raw.pid[PITCH].D = 23;
    raw.pid[YAW].P = 68;
    raw.pid[YAW].I = 45;
    raw.pid[YAW].D = 0;
    raw.pid[PIDLEVEL].P = 90;
    raw.pid[PIDLEVEL].I = 10;
    raw.pid[PIDLEVEL].D = 100;
    raw.pid[PIDALT].P = 64;
    raw.pid[PIDALT].I = 25;
    raw.pid[PIDALT].D = 24;
}

void Configuration::Load(uint8_t pIndex) {
    if (pIndex >= MAX_PROFILES) {
        return;
    }
    eeprom_read_block(&raw, reinterpret_cast<const void *>(pIndex * sizeof(raw)), sizeof(raw));
    if (calcChecksum(reinterpret_cast<uint8_t *>(&raw), sizeof(raw)) != raw.checksum) {
        loadDefault();
    }
}

void Configuration::Write(uint8_t pIndex) {
    if (pIndex >= MAX_PROFILES) {
        return;
    }
    raw.checksum = calcChecksum(reinterpret_cast<uint8_t *>(&raw), sizeof(raw));
    eeprom_update_block(&raw, reinterpret_cast<void *>(pIndex * sizeof(raw)), sizeof(raw));
}

uint8_t Configuration::calcChecksum(uint8_t *payload, uint8_t size) {
    uint8_t checksum = 0x55;
    while (--size) {
        checksum += *payload++;
    }
    return checksum;
}
