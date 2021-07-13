#include "serializer_msp.h"
#if PROTOCOL_ID == 1
#if !DEBUG


extern ACSController acs;
extern uint16_t cycleTime;
extern Configuration conf;
extern int16_t rcCommand[4];

#if GPS_ENABLED
extern Navigation nav;
#endif

static uint8_t inputBuffer[INPUT_BUFFER_SIZE];
static uint8_t inputBufferIndex = 0;
static uint8_t txBuffer[TX_BUFFER_SIZE];
static uint8_t txBufferHead;
uint8_t state = IDLE;
uint8_t dataSize = 0;
uint8_t dataOffset = 0;
uint8_t checksum = 0;
uint8_t command = 0;

void serialWrite() {
    PROTOCOL_SERIAL.write(txBuffer, txBufferHead);
    txBufferHead = 0;
}

void serialSerialize(uint8_t a) {
    uint8_t t = txBufferHead;
    if (t >= TX_BUFFER_SIZE)
        serialWrite();
    txBuffer[t] = a;
    t++;
    txBufferHead = t;
}

static void serialize8(uint8_t a) {
    serialSerialize(a);
    checksum ^= a;
}

static uint8_t read8() {
    return inputBuffer[inputBufferIndex++] & 0xFF;
}

static uint16_t read16() {
    uint16_t c = read8();
    c += (uint16_t) read8() << 8;
    return c;
}

static uint32_t read32() {
    uint32_t c = read16();
    c += (uint32_t) read16() << 16;
    return c;
}

static void __attribute__((noinline)) structWrite(uint8_t *cb, uint8_t siz) {
    while (siz--)
        *cb++ = read8();
}

static void headSerialResponse(uint8_t err, uint8_t s) {
    serialize8('$');
    serialize8('M');
    serialize8(err ? '!' : '>');
    checksum = 0; // start calculating a new checksum
    serialize8(s);
    serialize8(command);
}

static void headSerialReply(uint8_t s) {
    headSerialResponse(0, s);
}

static void headSerialError() {
    headSerialResponse(1, 0);
}

static void tailSerialReply() {
    serialize8(checksum);
}

static void commandReply(uint8_t *payload, uint8_t size) {
    while (size--)
        serialize8(*payload++);
}

static void responsePayload(uint8_t *payload, uint8_t size) {
    headSerialReply(size);
    commandReply(payload, size);
    tailSerialReply();
    serialWrite();
}

static void responseEmpty() {
    headSerialReply(0);
    tailSerialReply();
    serialWrite();
}

static void responseError() {
    headSerialError();
    serialWrite();
}

void evaluateCommand(uint8_t cmd) {
    uint32_t tmp = 0;

    switch (cmd) {
        case MSP_IDENT: {
            struct {
                uint8_t version, type, mspVersion;
                uint32_t capability;
            } identity;

            identity.version = VERSION;
            identity.type = ROBOT_TYPE;
            identity.mspVersion = MSP_VERSION;
            identity.capability = 0;
            responsePayload((uint8_t *) &identity, 7);
            break;
        }

        case MSP_STATUS: {
            struct {
                uint16_t cycleTime, i2cErrorCount, sensor;
                uint32_t flag;
                uint8_t set;
            } stat;
            stat.cycleTime = cycleTime;
            stat.i2cErrorCount = I2C::getErrCount();
            stat.sensor = SENSOR_ACC | SENSOR_BARO << 1 | SENSOR_MAG << 2 | SENSOR_GPS << 3 | SENSOR_SONAR << 4 |
                          SENSOR_GYRO << 5;

#if SENSOR_ACC
            if (conf.angleMode) {
                tmp |= 1 << ANGLE;
            }
            if (conf.horizonMode) {
                tmp |= 1 << HORIZEN;
            }

#endif
            if (conf.arm) {
                tmp |= 1 << ARM;
            }
            stat.flag = tmp;
            stat.set = 0;

            responsePayload((uint8_t *) &stat, 11);
            break;
        }

        case MSP_RAW_IMU: {
            struct {
                int16_t acc[3];
                int16_t gyro[3];
                int16_t mag[3];
            } imuData;

            acs.getRawData(imuData.acc, 9);
            responsePayload((uint8_t *) &imuData, 18);
            break;
        }

        case MSP_RAW_BARO: {
            struct {
                int16_t ct;
                int32_t cp;
                int32_t ccp;
            } baroRaw;

            acs.getBaroData(&baroRaw.ct, &baroRaw.cp, &baroRaw.ccp);
            responsePayload((uint8_t *) &baroRaw, 10);
            break;
        }

        case MSP_ATTITUDE: {
            int16_t att[3];
            acs.getAttitude(att, 3);
            responsePayload((uint8_t *) att, 6);
            break;
        }

        case MSP_ALTITUDE: {
            struct {
                int32_t alt;
                int16_t vario;
            } altData;
            acs.getAltitude(&altData.alt, &altData.vario);
            responsePayload((uint8_t *) &altData, 6);
            break;
        }

        case MSP_ACC_CALIBRATION: {
            if (!conf.arm) {
                acs.accCalibration();
            }
            responseEmpty();
            break;
        }

        case MSP_MAG_CALIBRATION: {
            if (!conf.arm) {
                acs.magCalibration();
                acs.baroCalibration();
            }
            responseEmpty();
            break;
        }

        case MSP_MOTOR: {
            uint16_t motorData[8];
            acs.getMotors(motorData, 8);
            responsePayload((uint8_t *) motorData, 16);
            break;
        }

        case MSP_ARM: {
            conf.arm = true;
            LEDPIN_ON
            responseEmpty();
            break;
        }

        case MSP_DIS_ARM: {
            conf.arm = false;
            LEDPIN_OFF
            responseEmpty();
            break;
        }

        case MSP_ALT_HOLD: {
            conf.altHoldMode = true;
            acs.getAltitude(&conf.altHold);
            responsePayload((uint8_t *) &conf.altHold, 4);
            break;
        }

        case MSP_ALT_UNLOCK: {
            conf.altHoldMode = false;
            conf.altHold = 0;
            responseEmpty();
            break;
        }

        case MSP_PID: {
            responsePayload((uint8_t *) &conf.raw.pid, 3 * PIDITEMS);
            break;
        }

        case MSP_SET_PID: {
            structWrite((uint8_t *) &conf.raw.pid, 3 * PIDITEMS);
            responseEmpty();
            break;
        }

        case MSP_CONFIG: {
            responsePayload((uint8_t *) &conf.raw, sizeof conf.raw);
            break;
        }

        case MSP_FLASH_CONFIG: {
            conf.write(0);
            break;
        }

        case MSP_WP: {

            break;
        }

        case MSP_SET_WP: {
            break;
        }

#if GPS_ENABLED
        case MSP_START_NAV: {
            nav.start();
            responseEmpty();
            break;
        }

        case MSP_STOP_NAV: {
            nav.stop();
            responseEmpty();
            break;
        }
#endif

#if defined(TEST_ALTHOLD)
        case MSP_TEST_ALTHOLD: {
            responseEmpty();
            uint16_t p = read16();
            acs.setTestAltBase(p);
            break;
        }
#endif

#if defined(TEST_RCCOMMAND)
        case MSP_TEST_RCCOMMAND: {
            responseEmpty();
            rcCommand[0] = read16();
            rcCommand[1] = read16();
            rcCommand[2] = read16();
            rcCommand[3] = read16();

#if TEST_LOG_LEVEL > 0
            Log::debugStart();
            Log::debug("rcCommand: ");
            Log::debug(rcCommand[0]);
            Log::debug(", ");
            Log::debug(rcCommand[1]);
            Log::debug(", ");
            Log::debug(rcCommand[2]);
            Log::debug(", ");
            Log::debugln(rcCommand[3]);
#endif
            break;
        }
#endif
    }
}

void serialInit() {
    PROTOCOL_SERIAL.begin(PROTOCOL_BAUD);
    while (!PROTOCOL_SERIAL) {
    }
    blinkLED(2, 20, 10);
    LEDPIN_OFF
}

void protocolHandler() {
    uint8_t a = PROTOCOL_SERIAL.available();
    while (a--) {
        uint8_t c = PROTOCOL_SERIAL.read();
        if (state == IDLE) {
            if (c == '$') {
                state = HEADER_START;
            }
        } else if (state == HEADER_START) {
            // expect 'M'
            state = (c == 'M') ? HEADER_M : IDLE;
        } else if (state == HEADER_M) {
            // expect '<'
            state = (c == '<') ? HEADER_ARROW : IDLE;
        } else if (state == HEADER_ARROW) {
            // expect size
            if (c > INPUT_BUFFER_SIZE) {
                // invalid size
                state = IDLE;
                continue;
            }

            dataSize = c;
            checksum = c;
            dataOffset = 0;
            state = HEADER_SIZE;
        } else if (state == HEADER_SIZE) {
            // expect cmd
            checksum ^= c;
            command = c;
            state = HEADER_CMD;
        } else if (state == HEADER_CMD) {
            if (dataOffset < dataSize) {
                checksum ^= c;
                inputBuffer[dataOffset++] = c;
            } else {
                // the last byte is checksum, check if equel
                if (checksum == c) {
                    inputBufferIndex = 0;
                    evaluateCommand(command);
                }
                state = IDLE;
                a = 0;
            }
        }
    }
}

#endif
#endif
