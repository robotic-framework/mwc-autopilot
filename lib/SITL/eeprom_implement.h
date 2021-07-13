//
// Created by 李翌文 on 2021/7/13.
//

#ifndef AUTOPILOT_EEPROM_IMPLEMENT_H
#define AUTOPILOT_EEPROM_IMPLEMENT_H

#include <stdint.h>

void eeprom_read_block (void *__dst, const void *__src, uint8_t __n);
void eeprom_update_block (const void *__src, void *__dst, uint8_t __n);

#endif //AUTOPILOT_EEPROM_IMPLEMENT_H
