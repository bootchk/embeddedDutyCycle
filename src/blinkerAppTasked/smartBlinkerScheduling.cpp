/*
 * Scheduling methods
 */

#include <src/blinkerAppTasked/smartBlinker.h>
#include "../OS/taskScheduler.h"
#include "../alarmClock/epochClock/epochClock.h"

#include "parameters.h"


/*
 * Task of kind 0 i.e. blink tasks
 */

void SmartBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            EpochClock::timeDurationFromNow(Parameters::BetweenBlinks));
}



void SmartBlinker::scheduleFirstEveningBlinkTask() {
    // 30 minutes after sunset
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            EpochClock::timeDurationFromNow(
                    Parameters::BetweenSunsetAndBlinking));
}

void SmartBlinker::scheduleFirstMorningBlinkTask() {
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            timeOfMorningBlinkPeriodStart());
}




/*
 * Tasks of kind 1 i.e. check sun tasks
 */


void SmartBlinker::scheduleCheckSunriseTask()
{
    TaskScheduler::scheduleTask(
            1,
            checkSunriseTask,
            EpochClock::timeDurationFromNow(Parameters::BetweenSunChecks));
}

void SmartBlinker::scheduleCheckSunsetTask()
{
    TaskScheduler::scheduleTask(
            1,
            checkSunsetTask,
            EpochClock::timeDurationFromNow(Parameters::BetweenSunChecks));
}



#ifdef FUTURE
void SmartBlinker::scheduleLaggedCheckSunsetTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunsetTask,
                                EpochClock::timeDurationFromNow(900));
}
#endif
