

#include "scheduledTaskSlot.h"
#include "taskScheduler.h"

// MSP430Drivers
#include <assert/myAssert.h>
#include <alarmClock/epochClock/epochClock.h>




namespace {

/*
 * MULTIPLE_TASK_SLOTS
 * Slot for two types of task can be scheduled concurrently.
 *
 * Without this, at most one task is ever scheduled.
 */

/*
 * Owns container, here a simple array.
 *
 *
 * Must be persistent through low power sleep.
 *
 * !!! Must be initialized, else CCS compiler puts them in .bss RAM.  Compile time initial value is moot.
 *
 *
 */
#ifdef MULTIPLE_TASK_SLOTS
#pragma PERSISTENT
ScheduledTaskSlot tasks[2] = {0,0};
#else
#pragma PERSISTENT
ScheduledTaskSlot tasks[1] = {0};
#endif




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

#ifdef MULTIPLE_TASK_SLOTS
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
#else
unsigned int getReadyTaskIndex() {
    // At least one task slot is not empty

    myAssert(not tasks[0].isEmpty);
    return 0;
}
#endif

}  // namespace




void TaskScheduler::init() {
    // No tasks scheduled
    tasks[0].isEmpty = true;
#ifdef MULTIPLE_TASK_SLOTS
    tasks[1].isEmpty = true;
#endif

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
    // Invoke MomentMethod to get Duration
    return tasks[readyTaskIndex].momentMethodPtr();
    // OLD return tasks[readyTaskIndex].durationUntilExecution;
}





Duration TaskScheduler::durationUntilNextTask() {
    myAssert(isTaskScheduled());

    unsigned int theReadyTaskIndex;

    theReadyTaskIndex = getReadyTaskIndex();
    readyATask(theReadyTaskIndex);
    return getDurationUntilReadyTask();
}

#ifdef MULTIPLE_TASK_SLOTS
void TaskScheduler::scheduleTask(
        unsigned int kind,
        TaskMethodPtr taskMethod,
        //OLD Duration aDurationUntilExecution
        MomentMethodPtr momentMethod
        ) {

    // slot must be empty, to reuse it
    myAssert( tasks[kind].isEmpty );

    tasks[kind].taskMethodPtr = taskMethod;
    // OLD tasks[kind].durationUntilExecution = aDurationUntilExecution;
    tasks[kind].momentMethodPtr = momentMethod;
    tasks[kind].isEmpty = false;
}
#else

void TaskScheduler::scheduleTask(
        TaskMethodPtr taskMethod,
        MomentMethodPtr momentMethod
        ) {

    // slot must be empty, to reuse it
    myAssert( tasks[0].isEmpty );

    tasks[0].taskMethodPtr = taskMethod;
    // OLD tasks[0].durationUntilExecution = aDurationUntilExecution;
    tasks[0].momentMethodPtr = momentMethod;
    tasks[0].isEmpty = false;
}
#endif



bool TaskScheduler::isTaskScheduled() {
#ifdef MULTIPLE_TASK_SLOTS
    return (not (tasks[0].isEmpty and tasks[1].isEmpty ));
#else
    return (not (tasks[0].isEmpty));
#endif
}


bool TaskScheduler::isTaskReady() {
    // readyTaskIndex is the index of some slot, not the NULL value
#ifdef MULTIPLE_TASK_SLOTS
    return (readyTaskIndex == 0 or readyTaskIndex == 1);
#else
    return (readyTaskIndex == 0 );
#endif
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
