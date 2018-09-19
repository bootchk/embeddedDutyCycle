
#include <src/OS/scheduledTask.h>
#include "taskScheduler.h"






namespace {

/*
 * Owns container, here a simple array.
 * Two types of task can be scheduled.
 *
 * Must be persistent through low power sleep.
 */
#pragma PERSISTENT
static ScheduledTask tasks[2];
#pragma PERSISTENT
static bool taskIsEmpty[2];

}



/*
 * Some task must be scheduled.
 * Some task must always be ready, except temporarily while the ready task is executing.
 */
static unsigned int readyTaskIndex;




void TaskScheduler::onAlarm() {
    /*
     * Ready task in schedule container might be overwritten after this,
     * since a task can schedule other tasks.
     */
    tasks[readyTaskIndex].execute();

    readyTaskIndex = 666;   // invalidate
}


EpochTime TaskScheduler::timeOfNextTask() {
    EpochTime result;

    if (taskIsEmpty[0]) {
        // only task 1
        readyTaskIndex = 1;
        result = tasks[1].scheduledTime;
    }
    else if (taskIsEmpty[1]) {
        // only task 0
        readyTaskIndex = 0;
        result = tasks[0].scheduledTime;
    }
    else {
        // choose task with soonest time
        // TODO
        result = 10;
    }
    // assert readyTaskIndex indicates next task
    return result;
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


