
#include "blinker.h"

#include "../peripheral/LED/led.h"


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
