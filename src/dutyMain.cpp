
#include <src/debug/test.h>
#include "dutyMain.h"

#include "PMM/powerMgtModule.h"
#include "duty.h"
#include "mcuSleep.h"

#include "app/app.h"

#include "debug/myAssert.h"




void DutyMain::onColdReset() {
    // assert unused GPIO configured

    // require external RTC connected, because this configures it
    Duty::onPowerOnReset();

    // assert not isLockedLPM5
    // Must unlock, since we will configure SPI pins and use them
    ///PMM::unlockLPM5();

    // assert Duty is ready for setAlarm

    // Comment out this define to test only the DutyCycle framework, without App
#define APPPOR
#ifdef APPPOR // test 1c
    App::onPowerOnReset();
    // assert app in initial state
#endif
}


void DutyMain::onWakeFromLPM() {
    // LOCKLPM5 is set unless we are using forceBlink functions
    /// myAssert(PMM::isLockedLPM5());

    MCUSleep::clearIsResetAWakeFromSleep();

    /*
     * GPIO config registers were reset (but GPIO state is held.)
     * Preconfigure as it was before sleep.
     * Effective upon unlockMCUFromSleep()
     */
    Duty::restoreMCUToPresleepConfiguration();
    App::configureSleepingGPIO();

    MCUSleep::unlockMCUFromSleep();

    // Interrupt is serviced now, if presleep configuration enables interrupts

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

#define SET_ALARM_BY_TIME
#ifdef SET_ALARM_BY_TIME
    Duty::setTimeAlarmOrReset(App::timeToWake());
#else
    Duty::setDurationAlarmOrReset(App::constantDurationOfSleep());
#endif


    Duty::lowerMCUToPresleepConfiguration();
    // GPIO configuration: sleep:all
}
