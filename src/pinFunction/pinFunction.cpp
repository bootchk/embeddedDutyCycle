#include "pinFunction.h"

// Delegates to App
#include "../app/app.h"

// msp430Drivers
#include <bridge/bridge.h>
#include <alarmClock/alarm/alarm.h>
#include <pinFunction/allPins.h>






void PinFunction::configureToSleepState() {

    // Only the app knows all unused pins
    // OLD App::configureUnusedPinsLowPower();

    // msp430Drivers knows all pins and how to set to a base state.
    // Configure all to a base state, to be soon changed.
    AllPins::setHighOutput();


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


