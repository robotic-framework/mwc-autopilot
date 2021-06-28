//
// Created by 李翌文 on 2021/6/28.
//

#ifndef AUTOPILOT_LOG_H
#define AUTOPILOT_LOG_H

#include <Print.h>
#include "definition.h"

namespace Log {

    void debugStart(void);

    size_t debug(const __FlashStringHelper *ifsh);
    size_t debug(const String &str);
    size_t debug(const char str[]);
    size_t debug(char c);
    size_t debug(unsigned char c, int base = DEC);
    size_t debug(int c, int base = DEC);
    size_t debug(unsigned int c, int base = DEC);
    size_t debug(long c, int base = DEC);
    size_t debug(unsigned long c, int base = DEC);
    size_t debug(double c, int base = 2);
    size_t debug(const Printable& p);

    size_t debugln(const __FlashStringHelper *ifsh);
    size_t debugln(const String &str);
    size_t debugln(const char str[]);
    size_t debugln(char c);
    size_t debugln(unsigned char c, int base = DEC);
    size_t debugln(int c, int base = DEC);
    size_t debugln(unsigned int c, int base = DEC);
    size_t debugln(long c, int base = DEC);
    size_t debugln(unsigned long c, int base = DEC);
    size_t debugln(double c, int base = 2);
    size_t debugln(const Printable& p);
    size_t debugln(void);

    void infoStart(void);

    size_t info(const __FlashStringHelper *ifsh);
    size_t info(const String &str);
    size_t info(const char str[]);
    size_t info(char c);
    size_t info(unsigned char c, int base = DEC);
    size_t info(int c, int base = DEC);
    size_t info(unsigned int c, int base = DEC);
    size_t info(long c, int base = DEC);
    size_t info(unsigned long c, int base = DEC);
    size_t info(double c, int base = 2);
    size_t info(const Printable& p);

    size_t infoln(const __FlashStringHelper *ifsh);
    size_t infoln(const String &str);
    size_t infoln(const char str[]);
    size_t infoln(char c);
    size_t infoln(unsigned char c, int base = DEC);
    size_t infoln(int c, int base = DEC);
    size_t infoln(unsigned int c, int base = DEC);
    size_t infoln(long c, int base = DEC);
    size_t infoln(unsigned long c, int base = DEC);
    size_t infoln(double c, int base = 2);
    size_t infoln(const Printable& p);
    size_t infoln(void);

    void errorStart(void);

    size_t error(const __FlashStringHelper *ifsh);
    size_t error(const String &str);
    size_t error(const char str[]);
    size_t error(char c);
    size_t error(unsigned char c, int base = DEC);
    size_t error(int c, int base = DEC);
    size_t error(unsigned int c, int base = DEC);
    size_t error(long c, int base = DEC);
    size_t error(unsigned long c, int base = DEC);
    size_t error(double c, int base = 2);
    size_t error(const Printable& p);

    size_t errorln(const __FlashStringHelper *ifsh);
    size_t errorln(const String &str);
    size_t errorln(const char str[]);
    size_t errorln(char c);
    size_t errorln(unsigned char c, int base = DEC);
    size_t errorln(int c, int base = DEC);
    size_t errorln(unsigned int c, int base = DEC);
    size_t errorln(long c, int base = DEC);
    size_t errorln(unsigned long c, int base = DEC);
    size_t errorln(double c, int base = 2);
    size_t errorln(const Printable& p);
    size_t errorln(void);

}

#endif //AUTOPILOT_LOG_H
