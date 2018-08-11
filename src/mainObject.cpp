

#include <src/mainObject.h>


#include "testMain.h"

#include "PMM/powerMgtModule.h"
#include "duty.h"
#include "app.h"
#include "mcuSleep.h"

#include <cassert>

// Two versions, working and non-working development


#define WORKING_VERSION



bool Main::isResetAwakeFromSleep() {
#ifndef WORKING_VERSION
    // checks only one reason
    return TestMain::isResetAwakeFromSleep();
#else
    // decodes all reasons
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
#ifdef WORKING_VERSION
    // No interrupts configured, ISR not called
    TestMain::blinkRedLED();
#else
    assert(PMM::isLockedLPM5());
    PMM::unlockLPM5();
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
    // MCUSleep::configureUnusedPinsLowPower();

    // require external RTC connected, because this configures it

    Duty::onPowerOnReset();
    PMM::unlockLPM5();
    assert(not PMM::isLockedLPM5());

    // assert Duty is ready for setAlarm

    //App::onPowerOnReset();
    // assert app in initial state
    // assert GPIO in sleeping configuration
#endif
}

void Main::configureWakeupSource() {
    TestMain::configureButtonWakeupSource();
}
