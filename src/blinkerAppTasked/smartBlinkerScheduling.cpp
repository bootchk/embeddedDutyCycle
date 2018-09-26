/*
 * Scheduling methods
 */
#include <src/blinkerAppTasked/day.h>
#include <src/blinkerAppTasked/smartBlinker.h>
#include "../OS/taskScheduler.h"
#include "../epochClock/epochClock.h"





void SmartBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(0,
                                blinkTask,
                                EpochClock::timeDurationFromNow(Duration::TenSeconds) );
}



void SmartBlinker::scheduleFirstEveningBlinkTask() {
    // 30 minutes after sunset
    TaskScheduler::scheduleTask(0,
                                blinkTask,
                                EpochClock::timeDurationFromNow(Duration::ThirtyMinutes) );
}

void SmartBlinker::scheduleFirstMorningBlinkTask() {
    TaskScheduler::scheduleTask(
            0, blinkTask, Day::timeTwoHoursBeforeSunriseTime());
}







void SmartBlinker::scheduleCheckSunriseTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunriseTask,
                                EpochClock::timeDurationFromNow(Duration::FifteenMinutes));
}

void SmartBlinker::scheduleCheckSunsetTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunsetTask,
                                EpochClock::timeDurationFromNow(Duration::FifteenMinutes));
}



#ifdef FUTURE
void SmartBlinker::scheduleLaggedCheckSunsetTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunsetTask,
                                EpochClock::timeDurationFromNow(900));
}
#endif
