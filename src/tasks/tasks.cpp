//
// Created by 李翌文 on 2020/12/30.
//

#include "tasks.h"

void taskSystem(uint32_t currentTime) {
}

void taskGetAcc(uint32_t currentTime) {
    imu.UpdateAcc(currentTime);
}

void taskGetGyro(uint32_t currentTime) {
    imu.UpdateGyro(currentTime);
}

void taskGetMag(uint32_t currentTime) {
    imu.UpdateMag(currentTime);
}

void taskGetBaro(uint32_t currentTime) {
    imu.UpdateBaro(currentTime);
}

void taskGetSonar(uint32_t currentTime) {
    imu.UpdateSonar(currentTime);
}

void taskGetGPS(uint32_t currentTime) {

}

void taskUpdateAttitude(uint32_t currentTime) {
    imu.UpdateAttitude(currentTime);
}

void taskUpdateAltitude(uint32_t currentTime) {
    imu.UpdateAltitude(currentTime);
}

void taskUpdateMotorPID(uint32_t currentTime) {
    motors.UpdatePID(currentTime);
}

void taskUpdateMotors(uint32_t currentTime) {
    motors.UpdateMotors(currentTime);
}

Task_t Tasks::tasks[TASK_COUNT] = {
        [TASK_SYSTEM] = {
                .taskName = "SYSTEM",
                .checkFunc = NULL,
                .taskFunc = taskSystem,
                .staticPriority = TASK_PRIORITY_HIGH,
                .desiredPeriod = TASK_PERIOD_HZ(10),
        },
#if SENSOR_ACC
        [TASK_GET_ACC] = {
                .taskName = "ACC",
                .checkFunc = NULL,
                .taskFunc = taskGetAcc,
                .staticPriority = TASK_PRIORITY_HIGH,
                .desiredPeriod = TASK_PERIOD_US(2000),
        },
#endif
#if SENSOR_GYRO
        [TASK_GET_GYRO] = {
                .taskName = "GYRO",
                .checkFunc = NULL,
                .taskFunc = taskGetGyro,
                .staticPriority = TASK_PRIORITY_HIGH,
                .desiredPeriod = TASK_PERIOD_US(2000),
        },
#endif
#if SENSOR_MAG
        [TASK_GET_MAG] = {
                .taskName = "MAG",
                .checkFunc = NULL,
                .taskFunc = taskGetMag,
                .staticPriority = TASK_PRIORITY_MEDIUM,
                .desiredPeriod = TASK_PERIOD_HZ(100),
        },
#endif
#if SENSOR_BARO
        [TASK_GET_BARO] = {
                .taskName = "BARO",
                .checkFunc = NULL,
                .taskFunc = taskGetBaro,
                .staticPriority = TASK_PRIORITY_MEDIUM,
                .desiredPeriod = TASK_PERIOD_HZ(100),
        },
#endif
#if SENSOR_SONAR
        [TASK_GET_SONAR] = {
                .taskName = "SONAR",
                .checkFunc = NULL,
                .taskFunc = taskGetSonar,
                .staticPriority = TASK_PRIORITY_MEDIUM,
                .desiredPeriod = TASK_PERIOD_HZ(100),
        },
#endif
#if SENSOR_GPS
        [TASK_GET_GPS] = {
                .taskName = "GPS",
                .checkFunc = NULL,
                .taskFunc = taskGetGPS,
                .staticPriority = TASK_PRIORITY_MEDIUM,
                .desiredPeriod = TASK_PERIOD_HZ(100),
        },
#endif
#if SENSOR_ACC || SENSOR_GYRO
        [TASK_UPDATE_ATT] = {
                .taskName = "ATTITUDE",
                .checkFunc = NULL,
                .taskFunc = taskUpdateAttitude,
                .staticPriority = TASK_PRIORITY_HIGH,
                .desiredPeriod = TASK_PERIOD_US(2000),
        },
#endif
#if SENSOR_BARO || SENSOR_SONAR
        [TASK_UPDATE_ALT] = {
                .taskName = "ALTITUDE",
                .checkFunc = NULL,
                .taskFunc = taskUpdateAltitude,
                .staticPriority = TASK_PRIORITY_HIGH,
                .desiredPeriod = TASK_PERIOD_HZ(100),
        },
#endif
#if SENSOR_ACC || SENSOR_GYRO
        [TASK_MOTOR_PID] = {
                .taskName = "MOTOR/PID",
                .checkFunc = NULL,
                .taskFunc = taskUpdateMotorPID,
                .staticPriority = TASK_PRIORITY_HIGH,
                .desiredPeriod = TASK_PERIOD_US(2000),
        },
#endif
        [TASK_UPDATE_MOTORS] = {
                .taskName = "MOTOR",
                .checkFunc = NULL,
                .taskFunc = taskUpdateMotors,
                .staticPriority = TASK_PRIORITY_HIGH,
                .desiredPeriod = TASK_PERIOD_US(2000),
        }
};

Tasks::Tasks() {
    init();
}

Tasks::~Tasks() {}

void Tasks::init() {
    queueClear();
    queueAdd(&tasks[TASK_SYSTEM]);

#if SENSOR_ACC
    SetTaskEnabled(TASK_GET_ACC, true);
#endif
#if SENSOR_GYRO
    SetTaskEnabled(TASK_GET_GYRO, true);
#endif
#if SENSOR_MAG
    SetTaskEnabled(TASK_GET_MAG, true);
#endif
#if SENSOR_BARO
    SetTaskEnabled(TASK_GET_BARO, true);
#endif
#if SENSOR_SONAR
    SetTaskEnabled(TASK_GET_SONAR, true);
#endif
#if SENSOR_GPS
    SetTaskEnabled(TASK_GET_GPS, true);
#endif
#if SENSOR_ACC || SENSOR_GYRO
    SetTaskEnabled(TASK_UPDATE_ATT, true);
#endif
#if SENSOR_BARO || SENSOR_SONAR
    SetTaskEnabled(TASK_UPDATE_ALT, true);
#endif
#if SENSOR_ACC || SENSOR_GYRO
    SetTaskEnabled(TASK_MOTOR_PID, true);
#endif
    SetTaskEnabled(TASK_UPDATE_MOTORS, true);
}

bool Tasks::queueAdd(Task_t *task) {
    if (queueSize >= TASK_COUNT || queueContains(task)) {
        return false;
    }

    for (int i = 0; i <= queueSize; i++) {
        if (queue[i] == NULL || queue[i]->staticPriority < task->staticPriority) {
            memmove(&queue[i + 1], &queue[i], sizeof(task) * (queueSize - i));
            queue[queueSize] = task;
            ++queueSize;
            return true;
        }
    }

    return false;
}

bool Tasks::queueContains(Task_t *task) {
    for (int i = 0; i < queueSize; i++) {
        if (queue[i] == task) {
            return true;
        }
    }

    return false;
}

bool Tasks::queueRemove(Task_t *task) {
    for (int i = 0; i < queueSize; i++) {
        if (queue[i] == task) {
            memmove(&queue[i], &queue[i + 1], sizeof(task) * (queueSize - i));
            queueSize--;
            return true;
        }
    }

    return false;
}

void Tasks::queueClear() {
    memset(queue, 0, sizeof(queue));
    queuePos = 0;
    queueSize = 0;
}

Task_t *Tasks::queueFirst() {
    queuePos = 0;
    return queue[queuePos];
}

Task_t *Tasks::queueNext() {
    return queue[++queuePos];
}

void Tasks::SetTaskEnabled(TaskId_e taskID, bool enabled) {
    if (taskID == TASK_SELF || taskID < TASK_COUNT) {
        Task_t *task = taskID == TASK_SELF ? currentTask : &tasks[taskID];
        if (enabled && task->taskFunc) {
            queueAdd(task);
        } else {
            queueRemove(task);
        }
    }
}

void Tasks::Schedule() {
    const uint32_t currentTime = micros();
    uint32_t timeToNextRealtimeTask = UINT32_MAX;
    for (const Task_t *task = queueFirst();
         task != NULL && task->staticPriority >= TASK_PRIORITY_REALTIME; task = queueNext()) {
        const uint32_t nextExecuteAt = task->lastExecutedAt + task->desiredPeriod;
        if ((int32_t) (currentTime - nextExecuteAt) >= 0) {
            timeToNextRealtimeTask = 0;
        } else {
            const uint32_t newTimeInterval = nextExecuteAt - currentTime;
            timeToNextRealtimeTask = min(timeToNextRealtimeTask, newTimeInterval);
        }
    }
    const bool outsideRealtimeGuardInterval = (timeToNextRealtimeTask > 0);

    Task_t *selectedTask = NULL;
    uint16_t selectedTaskDynamicPriority = 0;

    uint16_t waitingTasks = 0;
    for (Task_t *task = queueFirst(); task != NULL; task = queueNext()) {
        if (task->checkFunc) {

        } else {
            task->taskAgeCycles = ((int32_t) (currentTime - task->lastExecutedAt)) / task->desiredPeriod;
            if (task->taskAgeCycles > 0) {
                task->dynamicPriority = 1 + task->staticPriority * task->taskAgeCycles;
                waitingTasks++;
            }

            if (task->dynamicPriority > selectedTaskDynamicPriority) {
                const bool taskCanBeChosen = (outsideRealtimeGuardInterval) ||
                                             (task->taskAgeCycles > 1) ||
                                             (task->staticPriority >= TASK_PRIORITY_REALTIME);
                if (taskCanBeChosen) {
                    selectedTaskDynamicPriority = task->dynamicPriority;
                    selectedTask = task;
                }
            }
        }
    }

    totalWaitingTasksSamples++;
    totalWaitingTasks += waitingTasks;

    currentTask = selectedTask;

    if (selectedTask) {
        Serial.println(selectedTask->taskName);
        selectedTask->lastExecutedAt = currentTime;
        selectedTask->dynamicPriority = 0;

        const uint32_t currentTimeBeforeTaskRun = micros();
        selectedTask->taskFunc(currentTimeBeforeTaskRun);
    }
}
