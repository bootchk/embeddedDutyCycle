
#include "alarm.h"

#include "RTC/realTimeClock.h"  // Avoid clash with rtc.h"
#include "AB08xx/bridge.h"	    //  hides SPI
#include "MCU/pinFunction.h"    // hides GPIO functions
#include "MCU/powerMgtModule.h"

#include <cassert>

/*
 * RTC signals SPI not ready (during reset) by asserting rtc:Fout/nIRQ pin low.
 * !!! Same pin as for the Alarm signal.
 *
 * Since SPI ready uses the alarm pin, it depends on interrupt is a pulse on rising edge.
 * Alternatively, if signal is another RTC pin (nRST), the interrupt could be configured falling edge.
 */
bool Alarm::isSPIReady() {
    assert(isConfiguredMcuAlarmInterface());    // requires
	return (Alarm::isAlarmInterruptSignalHigh());
}


void Alarm::resetIfSPINotReady() {
	if (!Alarm::isSPIReady()) {
		 // System is in invalid state (mcu not POR, but rtc is POR)
		 PMM::triggerSoftwareBORReset();
	 }
}



void Alarm::waitSPIReadyOrReset() {
	int i = 0;
	while ( ! Alarm::isSPIReady() ) {
		i++;
		if (i > 1000) {
			PMM::triggerSoftwareBORReset();
		}
	}
}



bool Alarm::clearAlarmOnRTC() {
	// Tell RTC to end interrupt pulse (signal to high) if not already so
	bool result;

	// Simple write to a register of RTC
	RTC::clearIRQInterrupt();

	// RTC should lower signal
	// TODO delay needed?

	result = isAlarmInterruptSignalHigh();

    // assert alarm signal high or result is false
	return result;
}



void Alarm::clearAlarmOnRTCOrReset() {
	if (!Alarm::clearAlarmOnRTC())  {
		// RTC may be continuing to generate interrupt signal on Fout/nIRQ
		PMM::triggerSoftwareBORReset();
	}
	// ensure RTC interrupt flag is clear
	// ensure interrupt signal net is high
}


void Alarm::clearAlarmOnMCU() {
	PinFunction::clearAlarmInterruptOnPin();
}


bool Alarm::isAlarmInterruptSignalHigh() {
	// requires pin configured as input

	return PinFunction::isAlarmPinHigh();
}



void Alarm::configureMcuSPIInterface(){ Bridge::configureMcuSide(); }
void Alarm::unconfigureMcuSPIInterface() { Bridge::unconfigureMcuSide(); }


void Alarm::configureMcuAlarmInterface() {
	/*
	 * Pin is high when no interrupt.
	 * RTC pulses low on interrupt.
	 * Pulse width is relatively long (1/4 second)
	 * Use trailing edge, low-to-high
	 */
	PinFunction::configureAlarmPinPullupLoToHiInterrupt();
}

bool Alarm::isConfiguredMcuAlarmInterface() {
    return PinFunction::isConfiguredAlarmPin();
}


void Alarm::configureRTC() {
	// require configureMcuSPIInterface

	// Order of configuration not important.
	RTC::configure24HourMode();

	RTC::configureRCCalibratedOscillatorMode();

	RTC::configureAlarmInterruptToFoutnIRQPin();
}


/*
 * Must be bulletproof since is alarm is failed to set, may sleep forever.
 */
bool Alarm::setAlarm(Duration duration) {
	bool result;

	// delegate to RTC
	result = RTC::setAlarm(duration);

	// ensure alarm is set
	return result;
}
