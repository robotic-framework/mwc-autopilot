//
// Created by 李翌文 on 2020/12/30.
//

#ifndef AUTOPILOT_TASKS_H
#define AUTOPILOT_TASKS_H

#include "stdint.h"
#include "global.h"

#if defined(SITL)
#include "networks.h"
#endif

#define TASK_PERIOD_HZ(hz) (1000000 / (hz))
#define TASK_PERIOD_MS(ms) ((ms) * 1000)
#define TASK_PERIOD_US(us) (us)

typedef enum {
    TASK_PRIORITY_IDLE = 0,     // Disables dynamic scheduling, task is executed only if no other task is active this cycle
    TASK_PRIORITY_LOW = 1,
    TASK_PRIORITY_MEDIUM = 3,
    TASK_PRIORITY_MEDIUM_HIGH = 4,
    TASK_PRIORITY_HIGH = 5,
    TASK_PRIORITY_REALTIME = 6,
    TASK_PRIORITY_MAX = 255
} TaskPriority;

typedef enum {
    TASK_SYSTEM,
    TASK_ALL,
    TASK_RC,
#if SENSOR_ACC
    TASK_GET_ACC,
#endif
#if SENSOR_GYRO
    TASK_GET_GYRO,
#endif
#if SENSOR_MAG
    TASK_GET_MAG,
#endif
#if SENSOR_BARO
    TASK_GET_BARO,
#endif
#if SENSOR_SONAR
    TASK_GET_SONAR,
#endif
#if SENSOR_GPS
    TASK_GET_GPS,
#endif
#if SENSOR_ACC || SENSOR_GYRO
    TASK_UPDATE_ATT,
#endif
#if SENSOR_BARO || SENSOR_SONAR
    TASK_UPDATE_ALT,
#endif
#if SENSOR_ACC || SENSOR_GYRO
    TASK_MOTOR_PID,
#endif
    TASK_UPDATE_MOTORS,
#if GPS_ENABLED
    TASK_GPS,
    TASK_NAV,
#endif
#if defined(SITL)
    TASK_SIM_REQUEST_IMU,
    TASK_SIM_REQUEST_CONTROL,
#endif
    TASK_COUNT,
    TASK_NONE = TASK_COUNT,
    TASK_SELF
} TaskId_e;

typedef struct {
    const char *taskName;

    bool (*checkFunc)(uint32_t currentTimeUs, int32_t currentDeltaTimeUs);

    void (*taskFunc)(uint32_t currentTimeUs);

    const uint8_t staticPriority;
    int32_t desiredPeriod;
    uint16_t taskAgeCycles;
    uint16_t dynamicPriority;
    uint32_t lastExecutedAt;
} Task_t;

#if defined(SITL)
extern sockpp::udp_socket conn;
#endif

class Tasks {
public:
    Tasks();

    ~Tasks();

public:

    void setTaskEnabled(TaskId_e taskID, bool enabled);

    void schedule();

private:
    static Task_t tasks[TASK_COUNT];
    Task_t *queue[TASK_COUNT + 1];
    int queueSize = 0;
    int queuePos = 0;
    Task_t *currentTask = NULL;

    uint32_t totalWaitingTasks;
    uint32_t totalWaitingTasksSamples;

private:
    void init();

    bool queueAdd(Task_t *task);

    bool queueContains(Task_t *task);

    bool queueRemove(Task_t *task);

    void queueClear();

    Task_t *queueFirst();

    Task_t *queueNext();
};


#endif //AUTOPILOT_TASKS_H
