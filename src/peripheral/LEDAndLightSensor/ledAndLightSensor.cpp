
#include "ledAndLightSensor.h"

// DriverLib
#include <gpio.h>

#include "../../board.h"


/*
 * private
 */
void LEDAndLightSensor::toReversedFromOff() {
    // assert p side low

    GPIO_setOutputHighOnPin(NSideLEDPort,    NSideLEDPin);
}


void LEDAndLightSensor::toMeasuringFromReversed() {
    // assert reversed: p side low, n side hign

    GPIO_setAsInputPin(NSideLEDPort,    NSideLEDPin);
}


unsigned int LEDAndLightSensor::measureByBleeding() {
    unsigned int j;
    for ( j = 0; j < 30000; j++) {
        if ( GPIO_getInputPinValue(NSideLEDPort, NSideLEDPin) == GPIO_INPUT_PIN_LOW )
            break;
    }
    return j;
    // assert state still measuring
}


void LEDAndLightSensor::toOffFromMeasuring() {
    // assert measuring:
    // n side out is high but dir is input
    // p side low
    GPIO_setOutputLowOnPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setAsOutputPin(NSideLEDPort,    NSideLEDPin);
    // assert n and p sides low, out
}



/*
 * public
 */

void LEDAndLightSensor::toOffFromUnconfigured() {
    // Set both sides low before configuring direction out
    GPIO_setOutputLowOnPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setOutputLowOnPin(PSideLEDPort,    PSideLEDPin);

    GPIO_setAsOutputPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setAsOutputPin(PSideLEDPort,    PSideLEDPin);
    // state = Off
}


void LEDAndLightSensor::toOnFromOff() {
    // assert N side low, out
    GPIO_setOutputHighOnPin(PSideLEDPort,    PSideLEDPin);
    // assert N side low, P side high
}


void LEDAndLightSensor::toOffFromOn() {
    GPIO_setOutputLowOnPin(PSideLEDPort,    PSideLEDPin);
    // assert both sides low
}


unsigned int LEDAndLightSensor::measureLight() {
    // assert state Off

    unsigned int result;

    // Charge capacitance of LED
    toReversedFromOff();

    // Configure p side as input
    toMeasuringFromReversed();

    result = measureByBleeding();

    toOffFromMeasuring();

    return result;
    // assert state Off
}


// TODO
bool LEDAndLightSensor::isNighttimeDark() {

    return false;
}
