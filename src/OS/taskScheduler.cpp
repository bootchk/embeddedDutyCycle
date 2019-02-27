

#include "scheduledTaskSlot.h"
#include "taskScheduler.h"

// MSP430Drivers
#include <assert/myAssert.h>
#include <realTimeClock/epochClock/epochClock.h>




namespace {

/*
 * Owns container, here a simple array.
 * Two types of task can be scheduled.
 *
 * Must be persistent through low power sleep.
 *
 * !!! Must be initialized, else CCS compiler puts them in .bss RAM.  Compile time initial value is moot.
 */
#pragma PERSISTENT
ScheduledTaskSlot tasks[2] = {0,0};



/*
 * Some task must be scheduled.
 * Some task must always be ready, except temporarily while the ready task is executing.
 *
 * Must be persistent.
 * We decide this when we set the alarm.
 * Then we sleep, the decision must persist through sleep.
 */
#pragma PERSISTENT
unsigned int readyTaskIndex = 666;


unsigned int getReadyTaskIndex() {
    // At least one task slot is not empty
    // At most two task slots are not empty

    unsigned int result;

    if (tasks[0].isEmpty)
    {
        // only task 1
        myAssert(not tasks[1].isEmpty);
        result = 1;
    }
    else if (tasks[1].isEmpty)
    {
        // only task 0
        myAssert(not tasks[0].isEmpty);
        result = 0;
    }
    else
    {
        // both slots non-empty
        // choose task with soonest time

        if (tasks[0].durationUntilExecution.seconds < tasks[1].durationUntilExecution.seconds)
        {
            result = 0;
        }
        else
        {
            result = 1;
        }
    }
    return result;
}

}  // namespace




void TaskScheduler::init() {
    // No tasks scheduled
    tasks[0].isEmpty = true;
    tasks[1].isEmpty = true;

    // readyTaskIndex will not be accessed until scheduling alarm makes it valid
}


void TaskScheduler::onAlarm() {
    myAssert( isTaskReady() );

    /*
     * Ready task in schedule container might be overwritten after this,
     * since a task can schedule other tasks.
     */
    tasks[readyTaskIndex].execute();
    /*
     * Executing it frees slot for reuse.
     */

    /*
     * No task is ready until next call to timeOfNextTask()
     */
    readyTaskIndex = 666;   // invalidate
}


void TaskScheduler::readyATask(unsigned int taskIndex) {
    myAssert( not tasks[taskIndex].isEmpty );

    readyTaskIndex = taskIndex;

}


Duration TaskScheduler::getDurationUntilReadyTask() {
    return tasks[readyTaskIndex].durationUntilExecution;
}


#ifdef OLD
Now we schedule by duration


void TaskScheduler::makeTaskScheduledTimeInFuture(unsigned int taskIndex) {
    // Side effect on parameter
    EpochClock::setTimeAlarmableInFuture(tasks[taskIndex].scheduledTime );
}



EpochTime TaskScheduler::timeOfNextTask() {
    myAssert(isTaskScheduled());

    unsigned int theReadyTaskIndex;

    theReadyTaskIndex = getReadyTaskIndex();

    /*
     * Caller will set alarm and sleep.
     * Ensure that alarm time is in the future, else alarm would not go off.
     * If alarm does not go off, and it is the only scheduled task,
     * that violates "some task is alarmed to execute in future"
     * which should be true before we go to sleep.
     */
    makeTaskScheduledTimeInFuture(theReadyTaskIndex);

    return readyATask(theReadyTaskIndex);
}
#endif


Duration TaskScheduler::durationUntilNextTask() {
    myAssert(isTaskScheduled());

    unsigned int theReadyTaskIndex;

    theReadyTaskIndex = getReadyTaskIndex();
    readyATask(theReadyTaskIndex);
    return getDurationUntilReadyTask();
}


void TaskScheduler::scheduleTask(
        unsigned int kind,
        TaskMethodPtr method,
        Duration aDurationUntilExecution) {

    // slot must be empty, to reuse it
    myAssert( tasks[kind].isEmpty );

    tasks[kind].taskMethodPtr = method;
    tasks[kind].durationUntilExecution = aDurationUntilExecution;
    tasks[kind].isEmpty = false;
}



bool TaskScheduler::isTaskScheduled() {
    return (not (tasks[0].isEmpty and tasks[1].isEmpty ));
}


bool TaskScheduler::isTaskReady() {
    return (readyTaskIndex == 0 or readyTaskIndex == 1);
}
