//
// Created by 李翌文 on 2021/8/27.
//

#ifndef AUTOPILOT_IO_SERVICE_H
#define AUTOPILOT_IO_SERVICE_H

#include "stdint.h"

class IOService {
public:
    uint8_t send(uint8_t *buf, uint8_t size);
};

extern IOService IO;

#endif //AUTOPILOT_IO_SERVICE_H
