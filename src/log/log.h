//
// Created by 李翌文 on 2021/6/28.
//

#ifndef AUTOPILOT_LOG_H
#define AUTOPILOT_LOG_H

#include <stdint.h>
#include "definition.h"

namespace Log {

    void debugStart(void);

    uint8_t debug(const char str[]);
    uint8_t debug(char c);
    uint8_t debug(unsigned char c, int base = 10);
    uint8_t debug(int c, int base = 10);
    uint8_t debug(unsigned int c, int base = 10);
    uint8_t debug(long c, int base = 10);
    uint8_t debug(unsigned long c, int base = 10);
    uint8_t debug(double c, int base = 2);

    uint8_t debugln(const char str[]);
    uint8_t debugln(char c);
    uint8_t debugln(unsigned char c, int base = 10);
    uint8_t debugln(int c, int base = 10);
    uint8_t debugln(unsigned int c, int base = 10);
    uint8_t debugln(long c, int base = 10);
    uint8_t debugln(unsigned long c, int base = 10);
    uint8_t debugln(double c, int base = 2);
    uint8_t debugln(void);

    void infoStart(void);

    uint8_t info(const char str[]);
    uint8_t info(char c);
    uint8_t info(unsigned char c, int base = 10);
    uint8_t info(int c, int base = 10);
    uint8_t info(unsigned int c, int base = 10);
    uint8_t info(long c, int base = 10);
    uint8_t info(unsigned long c, int base = 10);
    uint8_t info(double c, int base = 2);

    uint8_t infoln(const char str[]);
    uint8_t infoln(char c);
    uint8_t infoln(unsigned char c, int base = 10);
    uint8_t infoln(int c, int base = 10);
    uint8_t infoln(unsigned int c, int base = 10);
    uint8_t infoln(long c, int base = 10);
    uint8_t infoln(unsigned long c, int base = 10);
    uint8_t infoln(double c, int base = 2);
    uint8_t infoln(void);

    void errorStart(void);

    uint8_t error(const char str[]);
    uint8_t error(char c);
    uint8_t error(unsigned char c, int base = 10);
    uint8_t error(int c, int base = 10);
    uint8_t error(unsigned int c, int base = 10);
    uint8_t error(long c, int base = 10);
    uint8_t error(unsigned long c, int base = 10);
    uint8_t error(double c, int base = 2);

    uint8_t errorln(const char str[]);
    uint8_t errorln(char c);
    uint8_t errorln(unsigned char c, int base = 10);
    uint8_t errorln(int c, int base = 10);
    uint8_t errorln(unsigned int c, int base = 10);
    uint8_t errorln(long c, int base = 10);
    uint8_t errorln(unsigned long c, int base = 10);
    uint8_t errorln(double c, int base = 2);
    uint8_t errorln(void);

}

#endif //AUTOPILOT_LOG_H
