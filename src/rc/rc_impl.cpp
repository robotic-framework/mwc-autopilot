//
// Created by 李翌文 on 2021/8/11.
//

#include "rc_impl.h"
#include "config.h"

RCImpl::RCImpl() : RC() {
    command[THROTTLE] = MINTHROTTLE;
}

void RCImpl::update(uint32_t currentTime) {

}
