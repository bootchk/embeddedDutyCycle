

#include "scheduledTask.h"
#include "../alarm/alarmTypes.h"



/*
 * Simple scheduler.
 *
 * Contains scheduled tasks.
 * Knows time to earliest task.
 * Knows task for which alarm has expired.
 *
 * Only two kinds of task to be scheduled at any one time.
 * Container holds only two tasks.
 */

class TaskScheduler {
public:
    /*
     * Alarm has fired for next task.
     */
    static void onAlarm();

    /*
     * Activate next task and return duration until it should execute.
     */
    static unsigned int durationTilNextTask();

    /*
     *
     */
    static void scheduleTask(unsigned int kind,
        TaskMethodPtr method,
        EpochTime epochTime);

#ifdef OLD
    static void scheduleBlinkTask();
    static void scheduleCheckSunriseTask();
#endif
};
