
#include "dutyMain.h"

#include "PMM/powerMgtModule.h"
#include "duty.h"
#include "app.h"
#include "mcuSleep.h"

#include "testMain.h"

#include <cassert>


void DutyMain::onColdReset() {
        // assert unused GPIO configured

        // require external RTC connected, because this configures it

        Duty::onPowerOnReset();
        PMM::unlockLPM5();
        assert(not PMM::isLockedLPM5());
        // assert Duty is ready for setAlarm

        App::onPowerOnReset();
        // assert app in initial state
}


void DutyMain::onWakeFromLPM() {
    PMM::unlockLPM5();

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
    Duty::setAlarmOrReset(App::durationOfSleep());

    TestMain::blinkGreenLED(5);

    Duty::lowerMCUToPresleepConfiguration();
    // GPIO configuration: sleep:all
}
