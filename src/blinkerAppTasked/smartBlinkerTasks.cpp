
#include "smartBlinker.h"

#include "blinker.h"
#include "blinkPeriod.h"

#include <assert/myAssert.h>


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
    Blinker::blink();

    ///TestMain::blinkForcedGreenLED(5);

    myAssert(BlinkPeriod::isActive());

    BlinkPeriod::advance();

    // check for blink period over
    if (BlinkPeriod::isOver()) {
        if (BlinkPeriod::isEvening()) {
            onEveningBlinkPeriodOver();
            // first blink task of morning is scheduled
        }
        else {
            // else morning blink subperiod over,  blink period over, no blinkTask scheduled
            onMorningBlinkPeriodOver();
        }
    }
    else {
        scheduleBlinkTask();
        // next blink task of period is scheduled
    }
    // assert (blinkTask and not blinkperiod::isOver())
    // or (blinkPerio::isOver and no blinking related task is scheduled)
}

