
/*
 * Abstracts functions of GPIO pins.
 * Hides implementation: TI MSPWare Driverlib
 *
 * Pin functions:
 * - alarm interrupt from RTC
 * - chip (slave) select for SPI to RTC
 * - LED / light sensor
 *
 * Three other pins are used by SPI, as needed
 */

class PinFunction {
public:
    static void configure();

private:
	static void configureUsedPins();

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
};
