//
// Created by 李翌文 on 2021/6/28.
//

#include "log.h"

size_t Log::debug(const __FlashStringHelper *ifsh) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(ifsh);
#else
    return 0;
#endif
}

size_t Log::debug(const String &str) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(str);
#else
    return 0;
#endif
}

size_t Log::debug(const char *c) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(c);
#else
    return 0;
#endif
}

size_t Log::debug(char c) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(c);
#else
    return 0;
#endif
}

size_t Log::debug(unsigned char c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::debug(int c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::debug(unsigned int c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::debug(long c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::debug(unsigned long c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::debug(double c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::debug(const Printable &p) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.print(p);
#else
    return 0;
#endif
}

size_t Log::debugln(const __FlashStringHelper *ifsh) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(ifsh);
#else
    return 0;
#endif
}

size_t Log::debugln(const String &str) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(str);
#else
    return 0;
#endif
}

size_t Log::debugln(const char *c) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(c);
#else
    return 0;
#endif
}

size_t Log::debugln(char c) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(c);
#else
    return 0;
#endif
}

size_t Log::debugln(unsigned char c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::debugln(int c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::debugln(unsigned int c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::debugln(long c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::debugln(unsigned long c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::debugln(double c, int base) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::debugln(const Printable &p) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println(p);
#else
    return 0;
#endif
}

size_t Log::debugln(void) {
#if TEST_LOG_LEVEL >= 3
    return PROTOCOL_SERIAL.println();
#else
    return 0;
#endif
}


size_t Log::info(const __FlashStringHelper *ifsh) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(ifsh);
#else
    return 0;
#endif
}

size_t Log::info(const String &str) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(str);
#else
    return 0;
#endif
}

size_t Log::info(const char *c) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(c);
#else
    return 0;
#endif
}

size_t Log::info(char c) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(c);
#else
    return 0;
#endif
}

size_t Log::info(unsigned char c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::info(int c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::info(unsigned int c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::info(long c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::info(unsigned long c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::info(double c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::info(const Printable &p) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.print(p);
#else
    return 0;
#endif
}

size_t Log::infoln(const __FlashStringHelper *ifsh) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(ifsh);
#else
    return 0;
#endif
}

size_t Log::infoln(const String &str) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(str);
#else
    return 0;
#endif
}

size_t Log::infoln(const char *c) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(c);
#else
    return 0;
#endif
}

size_t Log::infoln(char c) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(c);
#else
    return 0;
#endif
}

size_t Log::infoln(unsigned char c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::infoln(int c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::infoln(unsigned int c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::infoln(long c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::infoln(unsigned long c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::infoln(double c, int base) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::infoln(const Printable &p) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println(p);
#else
    return 0;
#endif
}

size_t Log::infoln(void) {
#if TEST_LOG_LEVEL >= 2
    return PROTOCOL_SERIAL.println();
#else
    return 0;
#endif
}


size_t Log::error(const __FlashStringHelper *ifsh) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(ifsh);
#else
    return 0;
#endif
}

size_t Log::error(const String &str) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(str);
#else
    return 0;
#endif
}

size_t Log::error(const char *c) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(c);
#else
    return 0;
#endif
}

size_t Log::error(char c) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(c);
#else
    return 0;
#endif
}

size_t Log::error(unsigned char c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::error(int c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::error(unsigned int c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::error(long c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::error(unsigned long c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::error(double c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(c, base);
#else
    return 0;
#endif
}

size_t Log::error(const Printable &p) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.print(p);
#else
    return 0;
#endif
}

size_t Log::errorln(const __FlashStringHelper *ifsh) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(ifsh);
#else
    return 0;
#endif
}

size_t Log::errorln(const String &str) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(str);
#else
    return 0;
#endif
}

size_t Log::errorln(const char *c) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(c);
#else
    return 0;
#endif
}

size_t Log::errorln(char c) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(c);
#else
    return 0;
#endif
}

size_t Log::errorln(unsigned char c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::errorln(int c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::errorln(unsigned int c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::errorln(long c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::errorln(unsigned long c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::errorln(double c, int base) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(c, base);
#else
    return 0;
#endif
}

size_t Log::errorln(const Printable &p) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println(p);
#else
    return 0;
#endif
}

size_t Log::errorln(void) {
#if TEST_LOG_LEVEL >= 1
    return PROTOCOL_SERIAL.println();
#else
    return 0;
#endif
}

void Log::debugStart(void) {
#if TEST_LOG_LEVEL >= 3
    PROTOCOL_SERIAL.print("[Debug][");
    PROTOCOL_SERIAL.print(micros());
    PROTOCOL_SERIAL.print("] ");
#endif
}

void Log::infoStart(void) {
#if TEST_LOG_LEVEL >= 2
    PROTOCOL_SERIAL.print("[Info][");
    PROTOCOL_SERIAL.print(micros());
    PROTOCOL_SERIAL.print("] ");
#endif
}

void Log::errorStart(void) {
#if TEST_LOG_LEVEL >= 1
    PROTOCOL_SERIAL.print("[Error][");
    PROTOCOL_SERIAL.print(micros());
    PROTOCOL_SERIAL.print("] ");
#endif
}

