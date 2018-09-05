
#include "alarm.h"

#include "RTC/realTimeClock.h"  // Avoid clash with rtc.h"
#include "AB08xx/bridge.h"	    //  hides SPI
#include "pinFunction/pinFunction.h"    // hides GPIO functions
#include "PMM/powerMgtModule.h"

#include <cassert>

/*
 * State variable that is NOT persistent across sleeps.
 */
bool _isConfiguredForAlarming = false;


bool Alarm::isConfiguredForAlarming() {
    return _isConfiguredForAlarming;
}



void Alarm::configureMcuSide() {
    // Must precede waitSPIReadyOrReset
    Alarm::configureMcuAlarmInterface();

    // Must precede use of SPI to configure rtc
    Alarm::configureMcuSPIInterface();

    PMM::unlockLPM5();
}

#ifdef OLD
            /*
             * Spin finite time waiting for rtc ready for SPI, i.e. out of reset.
             */
            // TODO TEMP
            //Alarm::waitSPIReadyOrReset();

            // assert alarm interrupt signal is high
            // mcu pin resets to an input, but without interrupt enabled

            // Assume rtc was reset also
#endif


/*
 * Not assume RTC was reset also.
 * RTC might not have powered down and up.
 * RTC could be in some bizarre state.
 */
void Alarm::configureForAlarming() {
    Alarm::configureMcuSide();

    assert(RTC::isReadable());

    Alarm::configureRTC();

    _isConfiguredForAlarming = true;
    // Ensure MCU SPI interface and RTC are configured for alarming
}


void Alarm::configureAfterWake() {
    // Fail reset if RTC not alive.
    // TODO Alarm::resetIfSPINotReady();

    Alarm::configureMcuSPIInterface();

    // RTC is still configured
    _isConfiguredForAlarming = true;
}


void Alarm::clearAlarm() {
        // May fail reset since RTC is remote device.
        Alarm::clearAlarmOnRTCOrReset();

        Alarm::clearAlarmOnMCU();
}


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
    /*
     * Require RTC in reset condition:  OUT bit is 1 and FOUT/nIRQ configured to show OUT bit.
     * Otherwise, this code will reset.
     */
    /*
     * Does NOT require SPI interface is ready, only requires alarm pin configured on MCU side.
     */
    // WRONG assert(RTC::readOUTBit() == true);

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
	// RTC should raise nIRQ signal to high if not already so.
	// mcu interrupt occurred on rising edge.
	// TODO delay needed?
	RTC::clearIRQInterrupt();

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
	PinFunction::configureAlarmPinPullupLoToHiInterrupt();
}

bool Alarm::isConfiguredMcuAlarmInterface() {
    return PinFunction::isConfiguredAlarmPin();
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


/*
 * Must be bulletproof since if alarm is failed to set, may sleep forever.
 */
bool Alarm::setAlarm(Duration duration) {
	bool result;

	assert(isConfiguredForAlarming());

	// delegate to RTC
	result = RTC::setAlarm(duration);

	// ensure alarm is set or result is false
	return result;
}
