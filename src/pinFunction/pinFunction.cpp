#include "pinFunction.h"


// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h

#include "../board.h"




/*
 * Alarm pin
 */

void PinFunction::configureAlarmPinPullupLoToHiInterrupt() {
	GPIO_setAsInputPinWithPullUpResistor(AlarmSignalPort, AlarmSignalPin);
    GPIO_enableInterrupt(AlarmSignalPort, AlarmSignalPin);
	GPIO_selectInterruptEdge(AlarmSignalPort, AlarmSignalPin, GPIO_LOW_TO_HIGH_TRANSITION);
}

/*
 * Hack for asserts only.
 * Not implemented
 * Should either use a variable in FRAM, or access port dir bit directly
 */
bool PinFunction::isConfiguredAlarmPin() {
    return true;
}

void PinFunction::clearAlarmInterruptOnPin() {
	GPIO_clearInterrupt(AlarmSignalPort, AlarmSignalPin);
}

bool PinFunction::isAlarmPinHigh() {
	/*
	 * GPIO_getInputPinValue returns a unsigned byte result for pin mask byte argument.
	 * Non-zero result means AlarmSignalPin is high.
	 */
	return (GPIO_getInputPinValue(AlarmSignalPort, AlarmSignalPin) != 0);
}






void PinFunction::configureUnusedPinsLowPower() {
    /*
     * Not require LPM5 locked.
     * Only configure pins not connected,
     * since configuring connected pins as output can trigger connected devices.
     */

    /*
     * TODO This should depend on board.h
     * For now, hardcoded:
     * Used pins:
     * - SPI 1.4-1.6
     * - slave select 3.2
     * - alarm 2.7
     *
     * Test pins:
     * - leds 1.1 and 1.2
     */
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);
        // SPI data pins
        //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);
        //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
        //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

        GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
        GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
        GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
        GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
        GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
        GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
        GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
        // RTC alarm pin
        //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

        // Only 5 pins on port 3
        GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
        GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);

        // SPI slave select pin
        //GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
        GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
        GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);

        // Test pins (leds) low
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
}

#ifdef OLD
Flawed because hardcoded to pins that have changed.
void PinFunction::configureUnusedPinsLowPower() {
	/*
	 * Specific to msp430fr2433
	 *
	 * DriverLib has no blanket function.  Optimize: set entire register for port.
	 *
	 * Output is low power (input, not driven would float and use power.)
	 */
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);

	/*
	 * Used.  Configure high so SPI Ready is also high?
	 */
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
	deselectSPISlave();

	// TODO
	// This will be an input always
	// GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);

	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

	// Only 5 pins on port 3
	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);
}
#endif
