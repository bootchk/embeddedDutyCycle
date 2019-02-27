#include "pinFunction.h"

// Delegates to App
#include "../app/app.h"

// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h

// msp430Drivers
#include <bridge/bridge.h>
#include <alarm/alarm.h>






void PinFunction::configureToSleepState() {

    // Only the app knows all unused pins
    App::configureUnusedPinsLowPower();


    // Framework knows pins it uses

    /*
     * Framework reserves bus access to RTC.
     * App knows they are reserved but doesn't know how to configure them.
     */
    Bridge::configureToSleepState();

    /*
     * Framework reserves alarm pin.
     * App knows they are reserved but doesn't know how to configure them.
     */
    Alarm::configureMcuAlarmInterface();


    // App knows pins it uses during sleep (e.g. a pin that lights an LED during sleep.)
    App::configureUsedPins();

    // ensure all pins configured for sleep
}


