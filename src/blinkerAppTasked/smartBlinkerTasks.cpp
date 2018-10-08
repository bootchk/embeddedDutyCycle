
#include "smartBlinker.h"

#include "../peripheral/LED/led.h"
#include <src/blinkerAppTasked/blinkPeriod.h>

#include "../debug/myAssert.h"


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

