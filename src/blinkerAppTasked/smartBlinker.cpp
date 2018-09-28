
#include <src/blinkerAppTasked/blinkPeriod.h>
#include <src/blinkerAppTasked/day.h>
#include <src/blinkerAppTasked/smartBlinker.h>
#include "../OS/taskScheduler.h"

// implementation
#include "../peripheral/ADC/adc.h"
#include "../peripheral/LED/led.h"


#include "../debug/myAssert.h"
#include "../debug/test.h"




void SmartBlinker::configureGPIO() {
    LED::configureGPIO();
}


void SmartBlinker::scheduleInitialTask() {
#define BLINK_ONLY
#ifdef BLINK_ONLY
    /*
     * For integration testing, only blink task, in a blink period
     */
    BlinkPeriod::initForEveningBlinking();

    ///  test blink task by calling it to schedule self
    blinkTask();
    ///scheduleBlinkTask();
#else
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
    if ( not ADC::isSolarCellDark()) {
        onSunriseDetected();
    }
    else {
        // still dark, schedule self again short time later
        scheduleCheckSunriseTask();
    }
}


void SmartBlinker::checkSunsetTask() {
    if ( ADC::isSolarCellDark()) {
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




