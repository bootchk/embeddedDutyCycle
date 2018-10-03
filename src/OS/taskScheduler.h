

#include "scheduledTaskSlot.h"
#include "../alarmClock/RTC/timeTypes.h"  // EpochTime



/*
 * Simple scheduler.
 *
 * Contains scheduled tasks.
 * Knows time to earliest task.
 * Knows task for which alarm has expired.
 *
 * Only two kinds of task to be scheduled at any one time.
 * At most one of each kind.
 * Container holds only two tasks.
 *
 *
 * States:
 * - task scheduled: app is still running and scheduling possibly other tasks
 * - task ready: app is done, getting ready to sleep, call to timeOfNextTask() makes next task ready
 * - alarm set: system is sleeping, alarm is set for the ready task
 * When the alarm fires, ready task will be executed, it becomes not ready, and the formerly ready task schedules more tasks.
 */

class TaskScheduler {
private:
    static EpochTime readyATask(unsigned int);

public:
    static void init();

    /*
     * Alarm has fired for next task.
     */
    static void onAlarm();

    /*
     * Activate next task and return time it should execute.
     * Caller will set alarm, alarm will call onAlarm()
     */
    static EpochTime timeOfNextTask();

    /*
     * Schedule a task of kind.
     * Asserts if there is already a task scheduled of that kind.
     * Only one task of a kind can be scheduled.
     */
    static void scheduleTask(unsigned int kind,
        TaskMethodPtr method,
        EpochTime epochTime);

    /*
     * Is some task scheduled? (Of either kind)
     * Task may not be ready yet (until timeOfNextTask() is called.)
     */
    static bool isTaskScheduled();

    /*
     * Is some task ready, i.e. after alarm was set and until after alarm fires and ready task is executed.
     */
    static bool isTaskReady();
};
