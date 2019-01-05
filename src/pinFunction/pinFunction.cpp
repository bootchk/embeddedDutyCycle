#include "pinFunction.h"

// Delegates to App
#include "../app/app.h"

// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h

// MSP430Drivers
#include <pinFunction/spiPins.h>






void PinFunction::configure() {

    // Only the app knows all unused pins
    App::configureUnusedPinsLowPower();


    // Pins used by framework

    // requires SPI access to RTC
    SPIPins::configureSelectSPIPin();
    // Other SPI pins configured as needed


    // Pins used by app
    App::configureUsedPins();

    // TODO Alarm pin configured later???
}


