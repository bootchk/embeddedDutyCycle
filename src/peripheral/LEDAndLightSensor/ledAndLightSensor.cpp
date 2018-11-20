
#include "ledAndLightSensor.h"

// DriverLib
#include <gpio.h>

#include "../../board.h"

#include "../../blinkerAppTasked/parameters.h"


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
    unsigned int result;
    unsigned char value;

    for ( result = 0; result < 30000; result++) {
        value = GPIO_getInputPinValue(NSideLEDPort, NSideLEDPin);
        // assert value is 0 or 1
        if (  value == GPIO_INPUT_PIN_LOW )
            break;
    }
    return result;
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



bool LEDAndLightSensor::isNighttimeDark() {
    unsigned int sample;

    sample = measureLight();

    /*
     * Greater value is dark.
     * Discharge is through the LED as solar cell generated current.
     * That current is greater in illumination, and discharges quickly, in fewer cycles of loop.
     */
    return (sample > Parameters::MinCyclesInLightToDischargeLEDCapacitance );
}
