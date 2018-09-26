
#include <src/blinkerAppFixed/blinker.h>
#include "../peripheral/LED/led.h"
#include "../epochClock/epochClock.h"


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
    LED::configureGPIO();
}


EpochTime Blinker::timeToWake() {
    return EpochClock::timeDurationFromNow(Duration::TenSeconds);
}
