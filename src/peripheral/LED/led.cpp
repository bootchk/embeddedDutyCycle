
#include "led.h"


// DriverLib
#include <gpio.h>

#include "../../board.h"




void LED::configureGPIO() {
	GPIO_setAsOutputPin(APP_LED1_PORT,    APP_LED1_PIN);
	GPIO_setAsOutputPin(APP_LED2_PORT,    APP_LED2_PIN);

	// OUT register resets to "undefined".
	// Ensure known state
	turnOff();
	turnOffLED2();
}


/*
 * mcu sources current to LED: high is on
 */
void LED::turnOff(){
#if LED_SOURCED
	GPIO_setOutputLowOnPin(APP_LED1_PORT,    APP_LED1_PIN);
#else
	GPIO_setOutputHighOnPin(APP_LED1_PORT,    APP_LED1_PIN);
#endif
}

void LED::turnOffLED2() {
    // TODO is sourced LED, add sunk
    GPIO_setOutputLowOnPin(APP_LED2_PORT,    APP_LED2_PIN);
}



void LED::turnOn(){
#if LED_SOURCED
	GPIO_setOutputHighOnPin(APP_LED1_PORT,    APP_LED1_PIN);
#else
	GPIO_setOutputLowOnPin(APP_LED1_PORT,    APP_LED1_PIN);
#endif
}


void LED::toggle() {
    GPIO_toggleOutputOnPin(APP_LED1_PORT, APP_LED1_PIN);
}

void LED::blink() {
    turnOn();
    // 100k cycles is about 1/100 Sec or 10 mSec
    __delay_cycles(100000);
    turnOff();
}
