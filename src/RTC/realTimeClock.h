
#include "../alarmTypes.h"	// Duration


/*
 * Abstracts RTC methods.
 *
 * Implementation is via writes and reads to registers of external device rtc.
 * This hides:
 *  - registers and values
 *  - combinations of values required for certain actions
 *
 * Collaborates with Bridge, which hides SPI versus I2C
 */

class RTC {
public:
	/*
	 * Clear interrupt flag that generated alarm interrupt.
	 * The flag must be cleared before another alarm can be set.
	 * Since the interrupt is downward pulse (1/64 second)  from rtc,
	 * and interrupt on the mcu is rising edge,
	 * Fout/nIRQ should already be high.
	 *
	 * No error return.
	 * Caller must check that Fout/nIRQ is high.
	 */
	static void clearIRQInterrupt();


	/*
	 * Return true if alarm is set.
	 * Return false if:
	 * - duration too short
	 * - SPI bus error (what was written did not compare to what was read)
	 */
	static bool setAlarm(Duration);



	/*
	 * Configuration methods.
	 *
	 * At POR reset, the RTC should be:
	 * - running (CTRL1.stop is cleared)
	 * - in 12 hour mode
	 * - using XTAL oscillator mode
	 */

	/*
	 * Configure hour counter to count to 24.
	 * !!! If you omit, upper nibble of hour counter is NOT hours tens digit, but include bit for AM/PM
	 */
	static void configure24HourMode();

	/*
	 * Configure RTC to use an oscillator that is both precise and low power.
	 * See data sheet for details and other modes.
	 * If you don't call this, RTC defaults at reset to XTAL oscillator mode.
	 */
	static void configureRCCalibratedOscillatorMode();

	/*
	 * Configure RTC so only alarm interrupts are on pin.
	 */
	static void configureAlarmInterruptToFoutnIRQPin();



private:
	static void selectOscModeRCCalibratedWithAutocalibrationPeriod();
	static void enableAutocalibrationFilter();
	static void enablePulseInterruptForAlarm();
	static void connectFoutnIRQPinToAlarmSignal();

	static void unlockMiscRegisters();
	static void unlockOscControlRegister();
};
