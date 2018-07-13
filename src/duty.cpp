
#include "duty.h"

#include "alarm.h"
#include "MCU/powerMgtModule.h"	// Software reset





/*
 * The mcu has seen a power on reset event (Vcc rising).
 * The mcu was not in LPM sleep previously (a hard reset.)
 * The design intends that the rtc also suffered a reset.
 */
void Duty::onPowerOnReset() {
    // Must precede waitSPIReadyOrReset
    Alarm::configureMcuAlarmInterface();

	/*
	 * Spin finite time waiting for rtc ready for SPI, i.e. out of reset.
	 */
	Alarm::waitSPIReadyOrReset();

	// assert alarm interrupt signal is high
	// mcu pin resets to an input, but without interrupt enabled

	// Assume rtc was reset also

	// Must precede use of SPI to configure rtc
	Alarm::configureMcuSPIInterface();

	Alarm::configureRTC();

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

	// Fail reset if RTC not alive.
	Alarm::resetIfSPINotReady();

	Alarm::configureMcuSPIInterface();

	/*
	 May fail reset since RTC is remote device.
	 */
	Alarm::clearAlarmOnRTCOrReset();

	Alarm::clearAlarmOnMCU();

	/*
	 * Assert alarm is cleared and no more will come, yet.
	 * Assert ready for setAlarm()
	 *
	 * Continuation typically is to act, then sleep mcu.
	 */
}


void Duty::setAlarmOrReset(unsigned int duration) {
	/*
	 * Fail means system might sleep forever, so only adequate response is reset mcu
	 */
	if (!Alarm::setAlarm(duration)) {
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
