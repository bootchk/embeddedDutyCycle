
#include <src/mainObject.h>

#include "dutyMain.h"


#include "PMM/powerMgtModule.h"
#include "MCU/mcu.h"
//#include "mcuSleep.h"

#include <src/debug/test.h>
#include <src/debug/myAssert.h>

//#include "pinFunction/pinFunction.h"

/// #include "myAssert.h"

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
 * - sleep
 * - - alarm
 * - - test (LED's)
 * - - app out (LED's)
 * - only awake
 * - - app temp (used temporarily in its useful work)
 * - - SPI
 */


/*
 * Uncomment to use last working version,
 * but also the logice of some ifdefs can be reversed to ifndef
 */
// #define WORKING_VERSION



bool Main::isResetAwakeFromSleep() {
#ifdef WORKING_VERSION
    // checks only one reason
    return Test::isResetAwakeFromSleep();
#else
    // decodes all reasons and may assert
    return MCU::isResetAWakeFromSleep();
#endif
}



#ifdef OBSOLETE
void Main::onResetPreamble() {
    // !!! not require PMM::isLockedLPM5()

#ifdef WORKING_VERSION
    Test::initAllGpioOutLow();
    PMM::unlockLPM5();
#else
    PinFunction::configureUnusedPinsLowPower();
    ///MCU::configureUnusedPinsLowPower();
    //PMM::unlockLPM5();
#endif
}
#endif


void Main::onWakeFromLPM() {
#ifdef WORKING_VERSION
    // No interrupts configured, ISR not called
#else
    // Interrupts configured: ISR called when unlock

    /// This assertion is true only when not using forceBlink
    /// myAssert(PMM::isLockedLPM5());

    DutyMain::onWakeFromLPM();
#endif
}



void Main::onColdReset() {
    // !!! not require PMM::isLockedLPM5()
    // since actually called for any reset not LPM5, which may include non BOR resets.

#ifdef WORKING_VERSION
    /*
     * LED already configured as output.
     * Later in postlude, we configure button input
     */
#else
    DutyMain::onColdReset();

    // assert GPIO configuration: awake:SPI and sleep
#endif
}



void Main::onResetPostlude() {
#ifdef WORKING_VERSION
    Test::configureButtonWakeupSource();
#else
    // For testing, an alternate source of wake
    Test::configureButtonWakeupSource();

    // For duty cycling, set alarm source of wake
    DutyMain::onResetPostlude();
#endif
}
