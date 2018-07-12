#include "pinFunction.h"


// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h


// rename from hal namespace to alarmLib namespace

// P1.3
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN3

// P1.2
#define RTCSelectPort GPIO_PORT_P1
#define RTCSelectPin  GPIO_PIN2





void PinFunction::configureAlarmPinPullupLoToHiInterrupt() {
	GPIO_setAsInputPinWithPullUpResistor(AlarmSignalPort, AlarmSignalPin);
    GPIO_enableInterrupt(AlarmSignalPort, AlarmSignalPin);
	GPIO_selectInterruptEdge(AlarmSignalPort, AlarmSignalPin, GPIO_LOW_TO_HIGH_TRANSITION);
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







void PinFunction::configureSelectSPIPin() {
	GPIO_setAsOutputPin(RTCSelectPort, RTCSelectPin);
}


void PinFunction::selectSPISlave(){
	GPIO_setOutputLowOnPin(RTCSelectPort, RTCSelectPin);
}
void PinFunction::deselectSPISlave(){
	GPIO_setOutputHighOnPin(RTCSelectPort, RTCSelectPin);
}



void PinFunction::configureAllGPIOPinsLowPower() {
	/*
	 * Specific to msp430fr2433
	 *
	 * DriverLib has no blanket function.  Optimize: set entire register for port.
	 *
	 * Output is low power (input, not driven would float and use power.)
	 */
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);
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
