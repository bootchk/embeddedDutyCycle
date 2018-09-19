
#include "smartBlinker.h"


#include "blinkPeriod.h"

// implementation
#include "../peripheral/ADC/adc.h"
#include "../peripheral/LED/led.h"

#include "../blinkerApp/blinker.h"



void SmartBlinker::configureGPIO() {
    LED::configureGPIO();
}

void SmartBlinker::init() {
    /*
     * Power on reset.
     * Assume it is day.
     * Start detecting sunset.
     * If it is already night, the first check sunset will immediately start checking for sunrise.
     * and start blinking????
     */
    scheduleCheckSunsetTask();
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

    // check for blink period over
    BlinkPeriod::blink();
    if (BlinkPeriod::isOver()) {
        if (BlinkPeriod::isEvening()) {
            onEveningBlinkPeriodOver();
            // first blink task of morning is scheduled
        }
        // else morning blink period over, no blinkTask scheduled
    }
    else {
        scheduleBlinkTask();
        // next blink task of period is scheduled
    }
    // assert blinkTask or no blinking related task is scheduled
}





