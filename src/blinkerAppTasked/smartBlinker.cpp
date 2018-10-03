
#include <src/blinkerAppTasked/blinkPeriod.h>
#include <src/blinkerAppTasked/day.h>
#include <src/blinkerAppTasked/smartBlinker.h>
#include "parameters.h"

#include "../OS/taskScheduler.h"

// implementation
#include "../peripheral/ADC/adc.h"
#include "../peripheral/LED/led.h"


#include "../debug/myAssert.h"
#include "../debug/test.h"




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
#define NIGHT_ONLY
//#define NORMAL_PERIODS


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
    // Force sunrise, will schedule sunset detect for 15 minutes later
    // sunsetDetectTask will never run (test will be over first)
    onSunriseDetected();

    // Sunset detect will artificially detect sunset and schedule nightime blinking
    onSunsetDetected();

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

/*
 * Tasks
 */

void SmartBlinker::checkSunriseTask() {
    if ( not isNight() ) {
        onSunriseDetected();
    }
    else {
        // still dark, schedule self again short time later
        scheduleCheckSunriseTask();
    }
}


void SmartBlinker::checkSunsetTask() {
    if ( isNight() ) {
        onSunsetDetected();
    }
    else {
        // still dark, schedule self again short time later
        scheduleCheckSunsetTask();
    }
}


void SmartBlinker::blinkTask() {
    LED::blink();

    ///TestMain::blinkForcedGreenLED(5);

    myAssert(BlinkPeriod::isActive());

    BlinkPeriod::advance();

    // check for blink period over
    if (BlinkPeriod::isOver()) {
        if (BlinkPeriod::isEvening()) {
            onEveningBlinkPeriodOver();
            // first blink task of morning is scheduled
        }
        // else morning blink subperiod over,  blink period over, no blinkTask scheduled
    }
    else {
        scheduleBlinkTask();
        // next blink task of period is scheduled
    }
    // assert blinkTask or no blinking related task is scheduled
}





EpochTime SmartBlinker::timeOfMorningBlinkPeriodStart() {
    return Day::timeBeforeNextSunriseBySeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
}



bool SmartBlinker::isNight() {
    /// For integration testing
    return true;

    /// Normal code
    ///return ADC::isSolarCellDark();
}
