
#include "duty.h"

#include "alarm/alarm.h"
#include "PMM/powerMgtModule.h"	// Software reset





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

    Alarm::configureAfterWake();

    Alarm::clearAlarm();

	/*
	 * Assert alarm is cleared and no more will come, yet.
	 * Assert ready for setAlarm()
	 *
	 * Continuation typically is to act, then sleep mcu.
	 */
}


void Duty::setAlarmOrReset(Duration duration) {
	/*
	 * Fail means system might sleep forever, so only adequate response is reset mcu
	 */
	if (!Alarm::setAlarmInSeconds(duration)) {
		PMM::triggerSoftwareBORReset();
	}
}

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
	Alarm::unconfigureMcuSPIInterface();
}

void Duty::clearAlarmOnMCU() {
	Alarm::clearAlarmOnMCU();
}
