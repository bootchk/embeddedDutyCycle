
#include "alarm.h"

#include "../PMM/powerMgtModule.h"
#include "../alarmClock/RTC/realTimeClock.h"  // Avoid clash with rtc.h"
#include "../alarmClock/AB08xx/bridge.h"       //  hides SPI
#include "../pinFunction/alarmPin.h"    // hides GPIO functions


#include <src/debug/myAssert.h>
#include <src/debug/test.h>


/*
 * Part of implementation of Alarm.
 * Configuration aspect.
 */


/*
 * State variable that is NOT persistent across sleeps.
 */
bool _isConfiguredForAlarming = false;


bool Alarm::isConfiguredForAlarming() {
    return _isConfiguredForAlarming;
}



/*
 * Configuration after coldstart.
 *
 * Not assume RTC was reset also.
 * RTC might not have powered down and up.
 * RTC could be in some bizarre state.
 */
void Alarm::configureForAlarming() {
    // GPIO must be unlocked because we configure SPI pins and use them
    // myAssert(not PMM::isLockedLPM5());

    // Configure interface (SPI) and driver (eUSCI)
    // Side effect is unlock
    Alarm::configureMcuSide();

    // assert not isLocked because we want to use SPI now

    // TODO needed?
    ///Test::delayBriefly();

    // assert can talk to RTC
    myAssert(RTC::isReadable());

    Alarm::configureRTC();

    _isConfiguredForAlarming = true;
    // Ensure MCU SPI interface and RTC are configured for alarming
}


void Alarm::configureAfterWake() {

    Alarm::configureMcuSPIInterface();

    // TODO this should be a reset
    myAssert(RTC::isReadable());
    // An older design checks the FOUT/nIRQ pin to test readiness of RTC
    // OLD Alarm::resetIfSPINotReady();

    // RTC is still configured
    _isConfiguredForAlarming = true;
}



void Alarm::configureMcuSide() {
    // Must precede waitSPIReadyOrReset
    Alarm::configureMcuAlarmInterface();

    // Must precede use of SPI to configure rtc
    Alarm::configureMcuSPIInterface();

    PMM::unlockLPM5();
}


void Alarm::configureMcuSPIInterface(){ Bridge::configureMcuSide(); }

void Alarm::unconfigureMcuSPIInterface() {
    Bridge::unconfigureMcuSide();
    _isConfiguredForAlarming = false;
}


void Alarm::configureMcuAlarmInterface() {
    /*
     * Pin is high when no interrupt.
     * RTC pulses low on interrupt.
     * Pulse width is relatively long (1/4 second)
     * Use trailing edge, low-to-high
     */
    AlarmPin::configurePullupLoToHiInterrupt();
}

bool Alarm::isConfiguredMcuAlarmInterface() {
    return AlarmPin::isConfigured();
}


void Alarm::configureRTC() {
    // require configureMcuSPIInterface
    /*
     * Require alarm is behind the counter.
     * Otherwise, by chance it could soon match,
     * resulting in an alarm coming before calling setAlarm().
     */

    // Order of configuration not important.
    RTC::configure24HourMode();

    RTC::configureRCCalibratedOscillatorMode();

    RTC::configureAlarmInterruptToFoutnIRQPin();

    RTC::enableAlarm();
}

