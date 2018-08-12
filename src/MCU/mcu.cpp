
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



void MCU::enableBSLOffAndVacantMemoryNMI() {

    // vacant memory generate interrupt as well as read and execute funny
    // bit set
    SFRIE1 |= VMAIE;

    // BSL memory behave as vacant memory
    // bit set
    SYSBSLC |= SYSBSLOFF;

}
