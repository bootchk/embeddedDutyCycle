
#include "blinker.h"

// MSP430Drivers
#include <realTimeClock/epochClock/epochClock.h>
#include <LED/led.h>


/*
 * Simple blinker app, just toggle LED at each duty cycle.
 */
void Blinker::init() {
    LED::turnOn();
}

void Blinker::onAlarm() {
    ///LED::toggle();
    LED::blink();
}

void Blinker::configureGPIO() {
    LED::configureLED1();
}


EpochTime Blinker::timeToWake() {
    return EpochClock::timeDurationFromNow(10);
}
