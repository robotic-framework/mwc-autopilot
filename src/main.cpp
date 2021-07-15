#include "global.h"
#include "tasks/tasks.h"
#if !DEBUG
#include "communication/serializer_msp.h"
#else
#include "avr8-stub.h"
#endif

uint32_t currentTime = 0;
uint16_t previousTime = 0;

Tasks scheduler;
extern ACSController acs;
extern uint16_t cycleTime;
extern Configuration conf;

#if GPS_ENABLED
extern Navigation nav;
#endif

void setup()
{
#if DEBUG
    debug_init();
#else
    serialInit();
#endif
    LEDPIN_PINMODE

    acs.init();

#if !DEBUG && GPS_ENABLED
    nav.init();
#endif

    conf.load(0);
}

void loop()
{
#if !DEBUG
    protocolHandler();
#endif
    currentTime = micros();
    cycleTime = currentTime - previousTime;
    previousTime = currentTime;
    scheduler.Schedule();
}

#if defined(SITL)
#include <unistd.h>
#include "networks.h"

int main(int argc, char* argv[]) {
    int opt = 0;
    string host;
    uint16_t port;
    while((opt = getopt(argc, argv, "h:p:")) != -1) {
        switch (opt) {
            case 'h':
                host = string(optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
        }
    }

    if (!initNetwork(host, port)) {
        return -1;
    }

    setup();
    while (true) {
        loop();
    }
}
#endif