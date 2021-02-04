//
// Created by 李翌文 on 2021/2/4.
//

#ifndef AUTOPILOT_CONFIGURATION_H
#define AUTOPILOT_CONFIGURATION_H

#include "type_def.h"

class Configuration {
public:
    bool arm;
    bool angleMode;
    bool horizonMode;
    bool baroMode;
    int32_t altHold;
    PID pid[PIDITEMS];

public:
    Configuration();

    void Load();

private:
    void loadDefault();
};


#endif //AUTOPILOT_CONFIGURATION_H
