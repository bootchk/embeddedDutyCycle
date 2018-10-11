
#include "mcu.h"

#include <msp430.h>

#include <driverlib.h>

#include "../PMM/powerMgtModule.h"
#include "../resetReason.h"


#ifdef DISABLE_BSL
/*
 * Disable BSL
 * BSL might already have executed, this only affects subsequent resets.
 */
#pragma location=0xFF84
unsigned int bslSignature1=0x5555;
#pragma location=0xFF86
unsigned int bslSignature2=0x5555;
#endif




void MCU::stopWatchDog() {
    WDTCTL = WDTPW | WDTHOLD;
}


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

    // Clear NMI and VMA flags
    // so we don't get an immediate interrupt if one has already occurred.
    SFRIFG1 &= ~(NMIIFG | VMAIFG);

    // vacant memory generate interrupt as well as read and execute funny
    // bit set
    SFRIE1 |= VMAIE;

    // BSL memory behave as vacant memory
    // bit set
    SYSBSLC |= SYSBSLOFF;

}


void MCU::disableFRAMWriteProtect() {
    // By default, writes cause NMI.  To disable, enable writing.
    SysCtl_enableFRAMWrite(SYSCTL_FRAMWRITEPROTECTION_DATA | SYSCTL_FRAMWRITEPROTECTION_PROGRAM);
}


/*
 * Delegate to PMM
 */
void MCU::clearIsResetAWakeFromSleep() { PMM::clearIsResetAWakeFromSleep(); }
void MCU::unlockMCUFromSleep(){ PMM::unlockLPM5(); }

/*
 * Delegate to ResetReason
 * (Another version delegates to PMM)
 */
bool MCU::isResetAWakeFromSleep() { return ResetReason::isResetAWakeFromSleep(); }
