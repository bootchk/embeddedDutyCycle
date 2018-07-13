
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
	static void configureSelectSPIPin();
	static void selectSPISlave();
	static void deselectSPISlave();

	static void configureAlarmPinPullupLoToHiInterrupt();
	static bool isConfiguredAlarmPin();
	static void clearAlarmInterruptOnPin();
	static bool isAlarmPinHigh();

	static void configureAllGPIOPinsLowPower();
};
