
#include "mcuSleep.h"

/*
 * Implementation is MSP430 specific.
 */
#include "MCU/mcu.h"
#include "MCU/pinFunction.h"

#include "PMM/powerMgtModule.h"
#include "resetReason.h"




#ifdef USE_WRAPPERS
void MCUSleep::enterLowestPowerSleep(){
	 /*
	    Require XT1 xtal oscillator off.
	     See family guide, section 3.2.4 "XT1 Oscillator"
	     The HW enables it for several conditions such as:
	     - the sw has configured it as a source for a clock and the clock is active
	     - a peripheral requests it (typically RTC)
	     - sw cleared AUTOFF and the power mode is in range [active, LPM4]

        Require Watchdog disabled.
     */

	  /*
	      In this example, none of conditions exist for XT1 enabled by hw
	      assert SCG0 == 1 SCG1 == 1, OSCOFF == 1
	      but those bit field names may be for another family member?

	      This is also necessary if XT1 is on
          UCSCTL6 |= XT1OFF | XT2OFF;
	  */

	  // For lowest power, PMM must be off
	  PMM::configureOff();

	  /*
	   * I don't think there is a race here.
	   * Turning regulator off does not power down until enter LPM4.5
	   * We can execute unlimited instructions here.
	   */

	  MCU::enterLPM4orLPM4_5();

	  // ensure Icc is least for family member e.g. Ilpm4.5 == 12nA for MSP430fr2433.
	  // ensure CPUOFF == 1
}
#else

/*
 * Use exact sequence prescribed by user's guide, to avoid any possibility of race conditions
 * or subsequent alteration by unfamiliar programmers.
 * Without any function wrappers.
 */

#include <msp430.h>

void MCUSleep::enterLowestPowerSleep(){

    // Disable interrupts
    __bic_SR_register(GIE);
    // Unlock PMM
    PMMCTL0_H = PMMPW_H;
    // Disable SVS high side
    PMMCTL0_L &= ~(SVSHE);
    // Core regulator off
    PMMCTL0_L |= PMMREGOFF;
    // Lock PPM
    PMMCTL0_H = 0;

    // Atomically enable global interrupts and sleep.
    __bis_SR_register(LPM4_bits & GIE);
    // __bis_SR_register(LPM4_bits);
    // Never get here, does not return
}

#endif


void MCUSleep::spinWithInterruptsEnabled() {
    __bis_SR_register(GIE);
    while (true) {
        __no_operation();
    }
}


void MCUSleep::unlockMCUFromSleep(){
	PMM::unlockLPM5();
}

/*
 The user's guide says alternatives for determinig are:
 - flag PMMLPM5IFG
 - decode SYSRSTIV

 The PPMLPM5IFG didn't seem to work reliably???
 */
#ifdef USE_PPMLPM5IFG

bool MCUSleep::isResetAWakeFromSleep() { return PMM::isResetAWakeFromSleep(); }
void MCUSleep::clearIsResetAWakeFromSleep() { PMM::clearIsResetAWakeFromSleep(); }

#else

bool MCUSleep::isResetAWakeFromSleep() { return ResetReason::isResetAWakeFromSleep(); }

// ??? Need to clear the IFG
void MCUSleep::clearIsResetAWakeFromSleep() { PMM::clearIsResetAWakeFromSleep(); }

#endif



void MCUSleep::configureAllPinsLowPower() {
	PinFunction::configureAllGPIOPinsLowPower();
}
