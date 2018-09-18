
#include <src/OS/scheduledTask.h>
#include "taskScheduler.h"







/*
 * Owns container, here a simple array.
 * Two types of task can be scheduled.
 */
static ScheduledTask tasks[2];
static bool taskIsEmpty[2];




/*
 * Some task must be scheduled.
 * Some task must always be ready, except temporarily while the ready task is executing.
 */
static unsigned int readyTaskIndex;




void TaskScheduler::onAlarm() {
#ifdef OLD
    ScheduledTask currentTask = tasks[readyTaskIndex].task;

    // Remove executed task from schedule
    // tasks[readyTaskIndex] = nullptr;

    currentTask->execute();
#endif
    tasks[readyTaskIndex].execute();

    /*
     * Executed task may schedule new tasks.
     * readyTaskIndex is invalid.
     */
}


unsigned int TaskScheduler::durationTilNextTask() {
    if (taskIsEmpty[0]) {
        // only task 1
        readyTaskIndex = 1;
        return tasks[1].scheduledTime;
    }
    else if (taskIsEmpty[1]) {
        // only task 2
        // TODO
    }
    else {
        // choose task with soonest time
        // TODO
    }
    // assert readyTaskIndex indicates next task
    return 10;
}


void TaskScheduler::scheduleTask(
        unsigned int kind,
        TaskMethodPtr method,
        EpochTime epochTime) {
    tasks[kind].taskMethodPtr = method;
    tasks[kind].scheduledTime = epochTime;
}



#ifdef OLD
void TaskScheduler::scheduleBlinkTask() {
    tasks[0].task = &blinkTask;
    // Always schedules const seconds from now
    tasks[0].scheduledTime = EpochClock::timeDurationFromNow(10);
}

void TaskScheduler::scheduleCheckSunriseTask() {
    tasks[0].task = &blinkTask;
    // Always schedules const seconds from now
    tasks[0].scheduledTime = EpochClock::timeDurationFromNow(10);
}
#endif


