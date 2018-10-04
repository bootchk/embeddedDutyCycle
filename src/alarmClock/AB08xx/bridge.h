
/*
 * Abstracts access to the RTC.
 *
 * A bridge across a serial channel.
 *
 * Hides choice of two different serial channels: SPI or I2C
 * Change channel by substituting a different implementation.
 *
 * Hides any quirks of the remote device protocol:
 * - the upper bit has special meaning.
 * - send address, then values
 *
 * Hides handshaking: which is not necessary when slave is always ready for serial communication.
 * More handshaking is necessary when there are many masters.
 *
 * Hides slave select.
 *
 * Hides multi-byte transfers.
 */

#include "AB08xxRegisters.h"

#include "rtcTime.h"



class Bridge {
public:
	/*
	 * Configure mcu side of interface.
	 */
	static void configureMcuSide();

	/*
	 * Configure SPI pins to low power state.
	 *
	 * Energia docs for SPI lib say this does not change the GPIO configuration,
	 * only disable the SPI function on them (disables the driving device.)
	 * Since MOSI and SClk are already outputs, they are low power.
	 * MISO is an input, but driven by the RTC so it should be low power.
	 * The documents do not say this changes MISO from input to output.
	 *
	 * This does not affect the SlaveSelect pin.
	 * But it is an output, also low power.
	 */
	static void unconfigureMcuSide();


	static unsigned char read(Address);

	/*
	 * Write one byte to remote register at address.
	 *
	 * Assertions ensure register contains value.
	 * Said assertions read back the register.
	 * This is not appropriate when a read will clear a register
	 * (But there are no such registers on this RTC chip.)
	 */
	static void write(Address, unsigned char value);

	/*
	 * Sets bits given by mask to remote register at address.
	 * Ensures that masked bits of register are set.
	 * Ensures all other bits (not masked) retain their values.
	 *
	 * Not general purpose: more generally, you would pass a mask and a value for the masked bits.
	 */
	static void setBits(Address, unsigned char mask);

	static void clearBits(Address, unsigned char mask);


	/*
	 * Read Time register.
	 * Write Alarm register.
	 *
	 * For both, RTCTime is raw, i.e. still BCD encoded for AB08xx
	 */
	static void writeAlarm(const RTCTime*);
	static void readAlarm(RTCTime*);

	static void readTime(RTCTime*);
};