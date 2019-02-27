

#include "dutyMain.h"

#include "duty.h"
#include "solarPower.h"

#include "../app/app.h"

// MSP430Drivers
#include <SoC/SoC.h>
#include <assert/myAssert.h>


// TEMP
//#include <src/debug/test.h>
#include <timer/timer.h>



bool DutyMain::isResetAwakeFromSleep() {
    /*
     * decodes all reasons.
     * May assert if reset reason is not an expected one,
     * e.g. some fault and not a wake from sleep reset or a coldstart reset.
     */
    return SoC::isResetAWakeFromSleep();
}


void DutyMain::onColdReset() {
    // assert unused GPIO configured

    // When solar powered, must sleep until power is adequate.
    SolarPower::sleepUntilPowerReserve();

    // Give framework a slice
    // require external RTC connected, because this configures it
    Duty::onPowerOnReset();

    // assert not isLockedLPM5
    // Must unlock, since we will configure SPI pins and use them
    ///PMM::unlockLPM5();

    // assert Duty is ready for setAlarm

    // Give application a slice
    // Comment out this define to test only the DutyCycle framework, without App
#define APPPOR
#ifdef APPPOR // test 1c
    App::onPowerOnReset();
    // assert app in initial state
#endif
}


void DutyMain::onWakeFromLPMReset() {
    // LOCKLPM5 is set unless we are using forceBlink functions
    /// myAssert(PMM::isLockedLPM5());

    SoC::clearIsResetAWakeFromSleep();

    // TEMP testing
    //LowPowerTimer::delayHalfSecond();

    /*
     * GPIO config registers were reset (but GPIO state is held.)
     * Preconfigure as it was before sleep.
     * Effective upon unlockMCUFromSleep()
     */
    Duty::restoreMCUToPresleepConfiguration();
    App::configureSleepingGPIO();

    SoC::unlockMCUFromSleep();

    // Interrupt would be serviced now, but the configuration at this point has NOT enabled alarm interrupt

    // TEMP debugging
    //LowPowerTimer::delayFiveSeconds();

    Duty::onWakeForAlarm();
    // assert alarm interrupt is cleared and Duty is ready for setAlarm


    App::onWakeForAlarm();
    // assert app done with useful work

    // GPIO configuration: awake:SPI, sleep:all
}


void DutyMain::onResetPostlude() {
    // Assert app is done with its useful work, or is in initial state

    // Assert app has unconfigured any devices used temporarily in its useful work.

    /*
     * GPIO configuration:
     * awake:SPI
     * sleep:all
     */

    // Resets if fail to set alarm

    /*
     * Alternatives, depending on whether app schedules in terms of type Duration or EpochTime
     */


#ifdef OLD
    Formerly, scheduling was by time
    Duty::setTimeAlarmOrReset(App::timeToWake());
#endif
    Duty::setDurationAlarmOrReset(App::durationOfSleep());

    /*
     * Also, for testing you can:
     * Duty::setDurationAlarmOrReset(App::constantDurationOfSleep());
     */




    Duty::lowerMCUToPresleepConfiguration();
    // GPIO configuration: sleep:all
}
