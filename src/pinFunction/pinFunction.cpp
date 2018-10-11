#include "pinFunction.h"


// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h

#include "../board.h"

#include "../debug/myAssert.h"




/*
 * Alarm pin
 */

void PinFunction::configureAlarmPinPullupLoToHiInterrupt() {
	GPIO_setAsInputPinWithPullUpResistor(AlarmSignalPort, AlarmSignalPin);
	GPIO_selectInterruptEdge(AlarmSignalPort, AlarmSignalPin, GPIO_LOW_TO_HIGH_TRANSITION);
}


void PinFunction::enableAlarmInterrupt() {
    clearAlarmInterruptOnPin();
    GPIO_enableInterrupt(AlarmSignalPort, AlarmSignalPin);
}



/*
 * Hack for asserts only.
 * Not implemented
 * Should either use a variable in FRAM, or access port dir bit directly
 */
bool PinFunction::isConfiguredAlarmPin() {
    return true;
}

bool PinFunction::isAlarmInterruptClear() {
    // hack not independent of port, fixed to port 1
    return not (P1IFG & AlarmSignalPin);
}

void PinFunction::clearAlarmInterruptOnPin() {
	GPIO_clearInterrupt(AlarmSignalPort, AlarmSignalPin);

	// If interrupt flag still set, the signaling device is still signaling
	myAssert(isAlarmInterruptClear());
}

bool PinFunction::isAlarmPinHigh() {
	/*
	 * GPIO_getInputPinValue returns a unsigned byte result for pin mask byte argument.
	 * Non-zero result means AlarmSignalPin is high.
	 */
	return (GPIO_getInputPinValue(AlarmSignalPort, AlarmSignalPin) != 0);
}



/*
 * OLD prototype
 * Used pins:
 * - SPI 1.4-1.6
 * - slave select 3.2
 * - alarm 2.7
 */


void PinFunction::configureUnusedPinsLowPower() {
    /*
     * Not require LPM5 locked.
     *
     * Only configure pins not connected,
     * since configuring connected pins as output can trigger connected devices.
     */


    // TODO these should be in another routine configureUsedPins
    // Launch pad LED pins 1.0 and 1.1 (out)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);

    // Slave select (out)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
    // P1.3 Alarm (in)
    // P1.4 Solar cell Vcc ADC pin (analog in)
    //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    // SPI pins
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

    // Only 5 pins on port 3
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);

    // Not ensure configuration is effective, since LPM5 might be locked
}



/*
 * Should be called before configured, so they don't blink.
 */
void PinFunction::setUsedOutPinValues() {
    // app led (also test led) low
    // TODO call driver functions for sink/source
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    // Test pin (green led) low
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
    // TODO chip select
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

}
#endif
