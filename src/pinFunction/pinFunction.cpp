#include "pinFunction.h"

// Delegates to App
#include "../app/app.h"

// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h

// MSP430Drivers
#include <bridge/bridge.h>






void PinFunction::configureToSleepState() {

    // Only the app knows all unused pins
    App::configureUnusedPinsLowPower();


    // Framework knows pins it uses

    /*
     * Framework requires bus access to RTC
     */
    Bridge::configureToSleepState();

    // Other bus pins configured as needed


    // App knows pins it uses
    App::configureUsedPins();

    // TODO Alarm pin configured later???
}


