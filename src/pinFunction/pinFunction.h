
/*
 * Abstracts functions of GPIO pins.
 * Hides implementation: TI MSPWare Driverlib
 *
 * Two pins:
 * - alarm interrupt from RTC
 * - chip (slave) select for SPI to RTC
 *
 * Three other pins are used by SPI.
 */
class PinFunction {
public:

	static void configureAlarmPinPullupLoToHiInterrupt();
	static bool isConfiguredAlarmPin();
	static void enableAlarmInterrupt();
	static void clearAlarmInterruptOnPin();
	static bool isAlarmInterruptClear();
	static bool isAlarmPinHigh();

	/*
	 * Configure unused and unconnected pins as low power.
	 *
	 * On most platforms, outputs.
	 * On reset, default to inputs.
	 * If left as inputs and unconnected, they float and draw excess power.
	 *
	 * Does not ensure determinate output values.
	 */
	static void configureUnusedPinsLowPower();

	/*
	 * So they don't blink while starting
	 */
	static void setUsedOutPinValues();
};
