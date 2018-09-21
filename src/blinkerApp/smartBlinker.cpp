
#include "smartBlinker.h"

// modules of app
#include "blinkPeriod.h"
#include "day.h"

#include "../OS/taskScheduler.h"

// implementation
#include "../peripheral/ADC/adc.h"
#include "../peripheral/LED/led.h"

#include "../blinkerApp/blinker.h"




void SmartBlinker::configureGPIO() {
    LED::configureGPIO();
}


void SmartBlinker::scheduleInitialTask() {
#ifdef BLINK_ONLY
    /*
     * For integration testing, only blink
     */
    scheduleBlinkTask();
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
    // blink LED
    Blinker::onAlarm();

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





