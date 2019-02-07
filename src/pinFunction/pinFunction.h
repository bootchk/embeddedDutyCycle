
/*
 * Framework understands that pins are held during LPM4.5.
 *
 * Abstracts configuring GPIO pins for the framework.
 *
 * The framework drives configuration.
 * Both the framework and the app defines configurations.
 * IOW, this configures pins owned by the framework, and gives app chance to configure pins owned by app.
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
    static void configureToSleepState();
};
