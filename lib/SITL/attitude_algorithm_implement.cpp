//
// Created by 李翌文 on 2021/7/30.
//

#include "attitude_algorithm_implement.h"
#include "common_handler.h"

void AAImpl::updateAttitude(uint32_t currentTime) {
    read((uint8_t *)att.Angle, (uint8_t *)handler.att, 4);
    att.Heading = handler.att[2];
}

void AAImpl::updateAltitude(uint32_t currentTime) {
    alt.Alt = handler.alt;
    alt.Vario = handler.vario;
}
