
#include "mcu.h"

#include "msp430.h"


void MCU::enterLPM4orLPM4_5(){

	/*
	 * Set certain bits in the mcu status register (SR)
	 *
	 * Alternatives:
	 * - LPM4 macro (doesn't atomic set GIE)
	 * - __bis_SR_register(LPM4_bits);
	 */
	/*
	 * Atomically enable global interrupts and sleep.
	 */
	__bis_SR_register(LPM4_bits & GIE);
}



void MCU::enableGlobalInterrupts() {
    __bis_SR_register(GIE);
}
