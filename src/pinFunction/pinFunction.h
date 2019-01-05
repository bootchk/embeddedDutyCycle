
/*
 * Abstracts configuring module's GPIO pins.
 * Hides that the framework drives configuration,
 * but the app defines it.
 *
 * Application and board specific.
 * Knows which devices (that use pins) are used by application.
 *
 * board.h knows which devices use which pins.
 *
 * Framework pin functions:
 * During sleep:
 * - alarm interrupt from external RTC
 * - chip (slave) select for SPI to external RTC
 * During wake:
 * - three other SPI pins, as needed (MOSI, SIMO, CLK)
 *
 * App pin functions (for example.)
 * - LED / light sensor
 */

class PinFunction {
public:
    static void configure();

};
