
#include "duty.h"

// MSP430Drivers
#include <alarm/alarm.h>
#include <softFault/softFault.h>





/*
 * The mcu has seen a power on reset event (Vcc rising).
 * The mcu was not in LPM sleep previously (a hard reset.)
 * The design intends that the rtc also suffered a reset.
 */
void Duty::onPowerOnReset() {
    Alarm::configureForAlarming();

	// ensure Alarm is ready for setAlarm()
	// caller typically calls setAlarm() and then sleeps.
}


void Duty::onWakeForAlarm() {
	/*
	 * Conditions:
	 *
	 * mcu was reset (LPM4.5 ends with reset, but not a POR).
	 * rtc has not been POR reset.
	 *
	 * If mcu was reset, caller configured GPIO for AlarmLib's alarm pin,
	 * but not for AlarmLib's SPI pins.
	 * The design is weird, see LPM4.5.
	 *
	 * rtc:Fout/nIRQ pin was pulsed low but now is high.
	 * The alarm interrupt is rising edge.
	 */

	// require alarm pin (also mean SPI ready) still configured as input

	/*
	 *
	 * Assert  no more alarms will come, until we set a new one.
	 *
	 * Continuation typically is to act, then sleep mcu.
	 */

    // We do nothing, until as late as possible.

    /*
     * Alternatively we could clear alarm IFG here, and ready Alarm now.
     * This is the old design.
     *
     * Alarm::clearAlarm();
     * Alarm::configureAfterWake();
     *
     * Ensure alarm is cleared.
     * Ensure ready for setAlarm()
     */
}


void Duty::setDurationAlarmOrReset(Duration duration) {
	/*
	 * Fail means system might sleep forever, so only adequate response is reset mcu
	 */
	if (!Alarm::setAlarmDurationSecondsFromNow(duration)) {
		SoftFault::failSetAlarm();
	}
}

#ifdef OLD

void Duty::setTimeAlarmOrReset(EpochTime time) {
    /*
     * Fail means system might sleep forever, so only adequate response is reset mcu
     */
    if (!Alarm::setAlarmToTime(time)) {
        SoftFault::failSetAlarm();
    }
}
#endif

/*
 * Three simple delegations.
 */

/*
 * Presleep configuration:
 * - alarm pin configured
 * - SPI pins low power
 */
void Duty::restoreMCUToPresleepConfiguration() {
	/*
	 * Require SPI pins already low power.
	 * I.E. the presleep configuration requires they are, but this does not make them so.
	 */
	Alarm::configureMcuAlarmInterface();
}

void Duty::lowerMCUToPresleepConfiguration() {
	Alarm::unconfigureMcuBusInterface();
}

void Duty::clearAlarmOnMCU() {
	Alarm::clearAlarmOnMCU();
}
