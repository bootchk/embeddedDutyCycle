
#include "serial.h"

#include "spi.h"

#ifdef USE_ENERGIA_SPI

Not used. This constrains choice of pins.
This requires a dev environment with Energia.

/* Choose mcu family and chip.
 * Must precede SPI.h
 * This configures pins for SPI interface to the default for the Launchpad i.e. the dev kit board.
 * I.E. configures a board.h file.
 *
 * A hack to get a clean compile in Eclipse IDE.
 * !!! Not necessarily correct, they just eliminate compiler errors.
 * Not needed if compiling in Energia IDE (where the Board Manager does magic.)
 * Proper paths also necessary.
 */
#define __MSP430__
#define __MSP430FR4133__ 1
// #define MSPEXP430FR2433LP 1

#include <SPI.h>
#endif



void Serial::begin() {
	/*
	 * Only configures 3 of the 4 pins (not the Slave Select pin):
	 * MOSI, MISO, SCLK
	 *
	 * TI Energia document doesn't say this configure MISO ???
	 */
	/*
	 * Configure the mcu SPI peripheral with parameters of rtc chip's SPI
	 */
	SPI::disable();	// Can only configure when disabled.
	SPI::configureMaster();
	SPI::enable();

	// ensure ready for transfer()
}


void Serial::end() {
	// ??? disabling saves power
	SPI::disable();

	SPI::unconfigureMaster();
}


unsigned char Serial::transfer(unsigned char value) {
	return SPI::transfer(value);
}


#ifdef NOT_USED

Cruft from Energia SPI implementation

Serial::setBitOrder(MSBFIRST);
Serial::setDataMode(SPI_MODE0);
Serial::setClockDivider(SPI_CLOCK_DIV128);

#endif
