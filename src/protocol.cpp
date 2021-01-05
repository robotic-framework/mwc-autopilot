#if !DEBUG
#include "protocol.h"

extern IMU imu;
extern Motors motors;
extern uint16_t cycleTime;
extern bool arm;
extern bool angleMode;
extern bool horizenMode;

enum MSP_protocol_states
{
    IDLE,
    HEADER_START,
    HEADER_M,
    HEADER_ARROW,
    HEADER_SIZE,
    HEADER_CMD
};

#define INPUT_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 128

static uint8_t inputBuffer[INPUT_BUFFER_SIZE];
static uint8_t inputBufferIndex = 0;
static uint8_t txBuffer[TX_BUFFER_SIZE];
static uint8_t txBufferHead;
uint8_t state = IDLE;
uint8_t dataSize = 0;
uint8_t dataOffset = 0;
uint8_t checksum = 0;
uint8_t command = 0;

void serialWrite()
{
    Serial.write(txBuffer, txBufferHead);
    txBufferHead = 0;
}

void serialSerialize(uint8_t a)
{
    uint8_t t = txBufferHead;
    if (t >= TX_BUFFER_SIZE)
        serialWrite();
    txBuffer[t] = a;
    t++;
    txBufferHead = t;
}

static void serialize8(uint8_t a)
{
    serialSerialize(a);
    checksum ^= a;
}

static uint8_t read8()
{
    return inputBuffer[inputBufferIndex++] & 0xFF;
}

static uint16_t read16()
{
    uint16_t c = read8();
    c += (uint16_t)read8() << 8;
    return c;
}

static uint32_t read32()
{
    uint32_t c = read16();
    c += (uint32_t)read16() << 16;
    return c;
}

static void headSerialResponse(uint8_t err, uint8_t s)
{
    serialize8('$');
    serialize8('M');
    serialize8(err ? '!' : '>');
    checksum = 0; // start calculating a new checksum
    serialize8(s);
    serialize8(command);
}

static void headSerialReply(uint8_t s)
{
    headSerialResponse(0, s);
}

static void headSerialError()
{
    headSerialResponse(1, 0);
}

static void tailSerialReply()
{
    serialize8(checksum);
}

static void commandReply(uint8_t *payload, uint8_t size)
{
    while (size--)
        serialize8(*payload++);
}

static void responsePayload(uint8_t *payload, uint8_t size)
{
    headSerialReply(size);
    commandReply(payload, size);
    tailSerialReply();
    serialWrite();
}

static void responseEmpty()
{
    headSerialReply(0);
    tailSerialReply();
    serialWrite();
}

static void responseError()
{
    headSerialError();
    serialWrite();
}

void evaluateCommand(uint8_t cmd)
{
    uint32_t tmp = 0;

    switch (cmd)
    {
    case MSP_IDENT:
        struct
        {
            uint8_t version, type, mspVersion;
            uint32_t capability;
        } identity;

        identity.version = VERSION;
        identity.type = ROBOT_TYPE;
        identity.mspVersion = MSP_VERSION;
        identity.capability = 0;
        responsePayload((uint8_t *)&identity, 7);
        break;

    case MSP_STATUS:
        struct
        {
            uint16_t cycleTime, i2cErrorCount, sensor;
            uint32_t flag;
            uint8_t set;
        } stat;
        stat.cycleTime = cycleTime;
        stat.i2cErrorCount = I2C::GetErrCount();
        stat.sensor = SENSOR_ACC | SENSOR_BARO << 1 | SENSOR_MAG << 2 | SENSOR_GPS << 3 | SENSOR_SONAR << 4 | SENSOR_GYRO << 5;

#if SENSOR_ACC
        if (angleMode)
        {
            tmp |= 1 << ANGLE;
        }
        if (horizenMode)
        {
            tmp |= 1 << HORIZEN;
        }

#endif
        if (arm)
        {
            tmp |= 1 << ARM;
        }
        stat.flag = tmp;
        stat.set = 0;

        responsePayload((uint8_t *)&stat, 11);
        break;

    case MSP_RAW_IMU:
        struct
        {
            int16_t acc[3];
            int16_t gyro[3];
            int16_t mag[3];
        } imuData;

        imu.GetRawData(imuData.acc, 9);
        responsePayload((uint8_t *)&imuData, 18);
        break;

    case MSP_ATTITUDE:
        int16_t att[3];
        imu.GetAttitude(att, 3);
        responsePayload((uint8_t *)att, 6);
        break;

    case MSP_ALTITUDE:
        struct
        {
            int32_t alt;
            int16_t vario;
        } altData;
        imu.GetAltitude(&altData.alt, &altData.vario);
        responsePayload((uint8_t *)&altData, 6);
        break;

    case MSP_ACC_CALIBRATION:
        responseEmpty();
        if (!arm)
        {
            imu.AccCalibration();
        }
        break;

    case MSP_MAG_CALIBRATION:
        responseEmpty();
        // TODO remove arm = true;
        arm = true;
        if (!arm)
        {
            imu.MagCalibration();
        }
        break;

    case MSP_MOTOR:
        uint16_t motorData[8];
        motors.GetMotors(motorData, 8);
        responsePayload((uint8_t *)motorData, 16);
        break;

    case MSP_ARM:
        responseEmpty();
        arm = true;
        LEDPIN_ON
        break;

    case MSP_DIS_ARM:
        responseEmpty();
        arm = false;
        LEDPIN_OFF
        break;
    }
}

void serialInit()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }
    blinkLED(2, 20, 10);
    LEDPIN_OFF;
}

void protocolHandler()
{
    uint8_t a = Serial.available();
    while (a--)
    {
        uint8_t c = Serial.read();
        if (state == IDLE)
        {
            if (c == '$')
            {
                state = HEADER_START;
            }
        }
        else if (state == HEADER_START)
        {
            // expect 'M'
            state = (c == 'M') ? HEADER_M : IDLE;
        }
        else if (state == HEADER_M)
        {
            // expect '<'
            state = (c == '<') ? HEADER_ARROW : IDLE;
        }
        else if (state == HEADER_ARROW)
        {
            // expect size
            if (c > INPUT_BUFFER_SIZE)
            {
                // invalid size
                state = IDLE;
                continue;
            }

            dataSize = c;
            checksum = c;
            dataOffset = 0;
            state = HEADER_SIZE;
        }
        else if (state == HEADER_SIZE)
        {
            // expect cmd
            checksum ^= c;
            command = c;
            state = HEADER_CMD;
        }
        else if (state == HEADER_CMD)
        {
            if (dataOffset < dataSize)
            {
                checksum ^= c;
                inputBuffer[dataOffset++] = c;
            }
            else
            {
                // the last byte is checksum, check if equel
                if (checksum == c)
                {
                    evaluateCommand(command);
                }
                state = IDLE;
                a = 0;
            }
        }
    }
}
#endif