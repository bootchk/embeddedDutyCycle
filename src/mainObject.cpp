

#include <src/mainObject.h>


#include "testMain.h"

#include "PMM/powerMgtModule.h"
#include "duty.h"
#include "app.h"
#include "mcuSleep.h"

#include <cassert>

/*
 * Two versions, working and non-working development.
 *
 * The working version sleeps until a button is pushed.
 * Then it blinks the red LED.
 *
 * The dev version uses an external RTC clock to periodically wake.
 */


/*
 * Categories of GPIO pins:
 * - unused
 * - sleeping
 * - - alarm pin
 * - - test pins (LED's)
 * - - app outputs (LED's)
 * - only awake (SPI pins)
 */


#define WORKING_VERSION



bool Main::isResetAwakeFromSleep() {
#ifndef WORKING_VERSION
    // checks only one reason
    return TestMain::isResetAwakeFromSleep();
#else
    // decodes all reasons and may assert
    return MCUSleep::isResetAWakeFromSleep();
#endif
}




void Main::onResetPreamble() {
    // !!! not require PMM::isLockedLPM5()

#ifndef WORKING_VERSION
    TestMain::initAllGpioOutLow();
    PMM::unlockLPM5();
#else
    MCUSleep::configureUnusedPinsLowPower();
    PMM::unlockLPM5();
#endif
}



void Main::onWakeFromLPM() {
#ifndef WORKING_VERSION
    // No interrupts configured, ISR not called
    TestMain::blinkRedLED();
#else
    assert(PMM::isLockedLPM5());
    PMM::unlockLPM5();
    // Here app would do something useful.
#endif
}



void Main::onColdReset() {
    // !!! not require PMM::isLockedLPM5()
    // since actually called for any reset not LPM5, which may include non BOR resets.

#ifndef WORKING_VERSION
    /*
     * LED already configured as output.
     * Later in postlude, we configure button input
     */
#else
    // assert unused pins already configured

    // require external RTC connected, because this configures it

    Duty::onPowerOnReset();
    PMM::unlockLPM5();
    assert(not PMM::isLockedLPM5());

    // assert Duty is ready for setAlarm

    App::onPowerOnReset();
    // assert app in initial state

    // TODO this is not right, SPI is configured but should be off for sleeping.
    // assert GPIO in sleeping configuration
#endif
}

void Main::onResetPostlude() {
#ifndef WORKING_VERSION
    TestMain::configureButtonWakeupSource();
#else
    // For testing, an alternate source of wake
    TestMain::configureButtonWakeupSource();


    // Assert app is done with its useful work, or is in initial state

    // Assert app has unconfigured any devices used temporarily in its useful work.
    // Some GPIO pins that app persists during sleep may still be in use (e.g. an LED)
    // Some GPIO pins are still configured for Duty i.e. SPI pins

        // Resets if fail to set alarm
        Duty::setAlarmOrReset(App::durationOfSleep());

        // TestMain::blinkGreenLED();

        Duty::lowerMCUToPresleepConfiguration();
#endif
}
