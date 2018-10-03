

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
     *
     */
    static void scheduleTask(unsigned int kind,
        TaskMethodPtr method,
        EpochTime epochTime);

    /*
     * Is some task scheduled?
     * Task may not be ready yet (until timeOfNextTask() is called.)
     */
    static bool isTaskScheduled();

    /*
     * Is some task ready, i.e. after alarm was set and until after alarm fires and ready task is executed.
     */
    static bool isTaskReady();
};
