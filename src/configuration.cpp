//
// Created by 李翌文 on 2021/2/4.
//

#include "configuration.h"

Configuration::Configuration() : arm(0),
                                 angleMode(1),
                                 horizonMode(1),
                                 baroMode(0),
                                 altHold(0) {

}

void Configuration::Load() {
    loadDefault();
}

void Configuration::loadDefault() {
    pid[ROLL].P = 33;
    pid[ROLL].I = 30;
    pid[ROLL].D = 23;
    pid[PITCH].P = 33;
    pid[PITCH].I = 30;
    pid[PITCH].D = 23;
    pid[YAW].P = 68;
    pid[YAW].I = 45;
    pid[YAW].D = 0;
    pid[PIDLEVEL].P = 90;
    pid[PIDLEVEL].I = 10;
    pid[PIDLEVEL].D = 100;
    pid[PIDALT].P = 64;
    pid[PIDALT].I = 25;
    pid[PIDALT].D = 24;
}
