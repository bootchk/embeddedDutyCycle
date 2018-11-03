#include "pinFunction.h"


// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h


#include "spiPins.h"
#include "ledPins.h"

#include "../debug/myAssert.h"




void PinFunction::configure() {
    configureUnusedPinsLowPower();
    configureUsedPins();

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
     *
     * Alternatives for low power:  input with pullup/pulldown or output.
     * Do not leave as input without pull: floats and wastes power.
     */

    // TODO optimize: configure entire register at once

    // P1.0 red LED or LEDP
    // P1.1 not used or green LED or LEDPWM
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
    // P1.2 SPI Slave select (out)
    // P1.3 Alarm (in)
    // P1.4 LEDN
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    // SPI data pins
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



void PinFunction::configureUsedPins() {

    SPIPins::configureSelectSPIPin();
    // Other SPI pins configured as needed

    LEDPins::configure();

    // TODO Alarm pin configured later???
}


