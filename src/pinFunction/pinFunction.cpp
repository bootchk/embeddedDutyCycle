#include "pinFunction.h"

// Delegates to App
#include "../app/app.h"

// msp430Drivers
#include <bridge/bridge.h>
#include <alarmClock/alarm/alarm.h>
#include <pinFunction/allPins.h>






void PinFunction::configureToSleepState() {
    /*
     * This has no effect on actual signals (only affects GPIO config regs), since GPIO pins are locked.
     */
    // require GPIO pins LPM45 locked

    // Only the app knows all unused pins
    // OLD App::configureUnusedPinsLowPower();

    // msp430Drivers knows all pins and how to set to a base state.
    /*
     * Configure all pins to a base, default state.
     * To be soon changed for pins used by framework and app.
     *
     * Default state is somewhat arbitrary.
     * Default state concerns pins which are not connected (or tied to a rail, see the board design.)
     * Direction must be out to prevent excess current from floating inputs.
     * Value can be high or low, here we choose High (since unused pins are not connected on the board.)
     */
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


