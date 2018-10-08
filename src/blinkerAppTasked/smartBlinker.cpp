
//#include <src/blinkerAppTasked/blinkPeriod.h>
#include <src/blinkerAppTasked/day.h>
#include <src/blinkerAppTasked/smartBlinker.h>
#include <src/blinkerAppTasked/powerMgr.h>

#include "parameters.h"

#include "../OS/taskScheduler.h"

// implementation
#include "../peripheral/ADC/adc.h"
#include "../peripheral/LED/led.h"


#include "../debug/myAssert.h"
#include "../debug/test.h"

// Configures code by ifdef
#include "../board.h"




void SmartBlinker::configureGPIO() {
    LED::configureGPIO();
}



/*
 * Hacks for integration testing.
 *
 * BLINK_ONLY:  exercise task scheduling but only one task, the blinkTask.
 *
 * NIGHT_ONLY: blink tasks, and detect sun tasks, but always dark.
 * Starts with a forced sunrise and sunset.
 *
 * NORMAL_PERIODS: full test as designed, blink and sun detect tasks, aligned with day
 */
//#define BLINK_ONLY
//#define NIGHT_ONLY
#define NORMAL_PERIODS


void SmartBlinker::scheduleInitialTask() {

#ifdef BLINK_ONLY
    // Hack: start a blink period without waiting for sunset
    BlinkPeriod::initForEveningBlinking();

    /*
     * test blink task by calling it to schedule self
     * Alternatively, could: scheduleBlinkTask();
     */
    blinkTask();
#endif
#if defined(NIGHT_ONLY)
    // Force sunrise.
    // !!! Not onSunriseDetected(), it will schedule sunset detect for 15 minutes later
    // but onSunsetDetected would attempt to schedule task of same kind, and fail
    Day::setSunriseTime();

    // Sunset detect will artificially detect sunset and schedule nightime blinking
    onSunsetDetected();
    myAssert(TaskScheduler::isTaskScheduled());

#endif
#if defined(NORMAL_PERIODS)
    /*
     * Schedule sun detection
     */
    /*
     * Assume it is daylight.
     * Start detecting sunset.
     *
     * Case 1: is already night, the first check sunset will immediately start checking for sunrise.
     * and possibly start blinking.
     *
     * Case 2: is daylight.  Check and continue checking for sunset.
     */
    scheduleCheckSunsetTask();
#endif

    // Some task is scheduled
    myAssert(TaskScheduler::isTaskScheduled());
}


void SmartBlinker::init() {
    /*
     * Power on reset.
     */

    // Show ignorance of actual day.
    Day::init();

    TaskScheduler::init();
    // Assert no tasks schedule, ready to schedule

    scheduleInitialTask();
}


void SmartBlinker::onAlarm() {
    // Schedule ready task
    TaskScheduler::onAlarm();
}

EpochTime SmartBlinker::timeToWake() {
    return TaskScheduler::timeOfNextTask();
}



void SmartBlinker::testTasks() {
    checkSunriseTask();
    checkSunsetTask();
    blinkTask();
}


EpochTime SmartBlinker::timeOfMorningBlinkPeriodStart() {
    return Day::timeBeforeNextSunriseBySeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
}



bool SmartBlinker::isNight() {
#ifdef SOLAR_CELL_PRESENT
    /// Normal code
        return PowerMgr::isSolarCellDark();
#else
    /// For integration testing
    return true;
#endif


}
