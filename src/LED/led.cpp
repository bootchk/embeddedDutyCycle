
#include "led.h"


// DriverLib
#include <gpio.h>


/*
 * Definitions for MSP-EXP430FR2433: two LED's
 */
// P1.0 red
#define APP_LED1_PORT     GPIO_PORT_P1
#define APP_LED1_PIN      GPIO_PIN0

// P1.1 green
#define APP_LED2_PORT     GPIO_PORT_P1
#define APP_LED2_PIN      GPIO_PIN1


void LED::configure() {
	GPIO_setAsOutputPin(APP_LED1_PORT,    APP_LED1_PIN);
	GPIO_setAsOutputPin(APP_LED2_PORT,    APP_LED2_PIN);

	// OUT register resets to "undefined".
	// Ensure known state
	GPIO_setOutputLowOnPin(APP_LED1_PORT,    APP_LED1_PIN);
	GPIO_setOutputLowOnPin(APP_LED2_PORT,    APP_LED2_PIN);
}


/*
 * mcu sources current to LED: high is on
 */
void LED::turnOff(){
	GPIO_setOutputLowOnPin(APP_LED1_PORT,    APP_LED1_PIN);
}

void LED::turnOn(){
	GPIO_setOutputHighOnPin(APP_LED1_PORT,    APP_LED1_PIN);
}

void LED::toggle() {
    GPIO_toggleOutputOnPin(APP_LED1_PORT, APP_LED1_PIN);
}

