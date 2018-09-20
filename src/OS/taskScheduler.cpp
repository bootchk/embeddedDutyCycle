
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
static ScheduledTaskSlot tasks[2];



/*
 * Some task must be scheduled.
 * Some task must always be ready, except temporarily while the ready task is executing.
 *
 * Must be persistent.
 * We decide this when we set the alarm.
 * Then we sleep, the decision must persist through sleep.
 */
#pragma PERSISTENT
static unsigned int readyTaskIndex = 666;

}


void TaskScheduler::init() {
    // No tasks scheduled
    tasks[0].isEmpty = true;
    tasks[1].isEmpty = true;

    // readyTaskIndex will not be accessed until scheduling alarm makes it valid
}


void TaskScheduler::onAlarm() {
    /*
     * Ready task in schedule container might be overwritten after this,
     * since a task can schedule other tasks.
     */
    tasks[readyTaskIndex].execute();
    /*
     * Executing it frees slot for reuse.
     */

    readyTaskIndex = 666;   // invalidate
}


EpochTime TaskScheduler::timeOfNextTask() {
    EpochTime result;

    // At least one task slot is not empty
    // At most two task slots are not empty

    if (tasks[0].isEmpty) {
        // only task 1
        readyTaskIndex = 1;
        result = tasks[1].scheduledTime;
    }
    else if (tasks[1].isEmpty) {
        // only task 0
        readyTaskIndex = 0;
        result = tasks[0].scheduledTime;
    }
    else {
        // choose task with soonest time

        if (tasks[0].scheduledTime < tasks[1].scheduledTime) {
            readyTaskIndex = 0;
        }
        else {
            readyTaskIndex = 1;
        }
        result = tasks[readyTaskIndex].scheduledTime;
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
    tasks[kind].isEmpty = false;
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


