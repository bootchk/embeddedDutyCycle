
#include "../AB08xx/bridge.h"
#include "realTimeClock.h"



/*
 * Implementation knows:
 * - RTC is connected via SPI
 * - which register and bit, semantics of same
 */
void RTC::clearIRQInterrupt() {
	/*
	 * Since we are only using one interrupt from RTC,
	 * it is safe to clear them all by writing a zero to the flag byte.
	 * Per AB08x5 User Manual section 5.12.9
	 *
	 * Register name is Status
	 * Bit name is ALM, but we clear all bits.
	 *
	 * The chain is:
	 * alarm register matching time registers sets Status.ALM bit
	 * ALM controls the AIRQ signal
	 * AIRQ signal is configured to pin Fout/nIRQ.
	 */
	Bridge::write(Address::Status, 0);
	// Bridge::write(Status, 0);
}


void RTC::configureRCCalibratedOscillatorMode() {
	// Set two separate registers
	RTC::selectOscModeRCCalibratedWithAutocalibrationPeriod();
	RTC::enableAutocalibrationFilter();
}



void RTC::configureAlarmInterruptToFoutnIRQPin() {
	// Set two separate registers

	// Order not important
	RTC::enablePulseInterruptForAlarm();
	RTC::connectFoutnIRQPinToAlarmSignal();
}

void RTC::configure24HourMode() {
	/*
	 * BIT7 == 0 -> not stopped
	 * BIT6 == 1 -> 24 hour mode
	 * BIT0 == 0 -> counter registers locked
	 */
	Bridge::write(Address::Control1, (unsigned char) 0b01000000 ); // (BIT6) ); );
}


/*
 * Private
 */


void RTC::selectOscModeRCCalibratedWithAutocalibrationPeriod() {

	unlockOscControlRegister();

	// OSEL on => BIT7;
	// ACAL == 10 (17 minute autocalibration period) => BIT6

	Bridge::write(Address::OscillatorControl, (unsigned char) 0b11000000 ); // (BIT7 | BIT6) );
}


void RTC::enableAutocalibrationFilter() {
	unlockMiscRegisters();

	// Enable filter
	Bridge::write(Address::AutocalibrationFilter, (unsigned char) Key::AutocalibrationFilterEnable );
}




void RTC::enablePulseInterruptForAlarm() {
	/*
	 * Bit 2:  AIE: enable alarm interrupt
	 * Bit 5,6: IM: == 11 1/4 second pulse width, requires least power
	 */
	Bridge::write(Address::InterruptMask, 0b01100100 );

	// Polarity of interrupt is not configurable, is high-to-low
}

void RTC::connectFoutnIRQPinToAlarmSignal() {
	/*
	 * Connects signals to pin.
	 * Here, we connect only the rtc's internal nAIRQ signal (from alarm)
	 * bits 0,1: OUT1S: == 11, pin is signal nAIRQ if AIE is set, else OUT
	 */
	Bridge::write(Address::Control2, 0b11 );
}


void RTC::unlockMiscRegisters() {
	Bridge::write(Address::ConfigurationKey, (unsigned char) Key::UnlockMiscRegisters );
}

void RTC::unlockOscControlRegister() {
	Bridge::write(Address::ConfigurationKey, (unsigned char) Key::UnlockOscillatorControl );
}
