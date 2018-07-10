
#include "powerMgtModule.h"


/*
 * Choose mcu family and chip.
 * Must precede SPI.h
 * This configures pins for SPI interface to the default for the Launchpad i.e. the dev kit board.
 * I.E. configures a board.h file.
 *
 * A hack to get a clean compile in Eclipse IDE.
 * !!! Not necessarily correct, they just eliminate compiler errors.
 * Not needed if compiling in Energia IDE (where the Board Manager does magic.)
 * Proper paths also necessary.
 */
#ifndef __MSP430__
#define __MSP430__
#define __MSP430FR4133__
#endif



// mcu hal layer e.g. MSPWare DriverLib
// depends on msp430.h
#include <pmm.h>

// some mcu family members have watchdog a and b
// #include <wdt_a.h>





void PMM::stopWatchdog() {
	// bare metal
	WDTCTL = WDTPW + WDTHOLD;

	// Using Driverlib
    //WDT_A_hold();
}



void PMM::configureOff() {

	// This saves a little more power.  The mode is not named.
	PMM_disableSVSH();

	// This gets x.5 power mode
	PMM_turnOffRegulator();
}


void PMM::unlockLPM5() {
	PMM_unlockLPM5();
}


bool PMM::isResetAWakeFromSleep() {
	return PMM_getInterruptStatus(PMM_LPM5_INTERRUPT);
}

void PMM::clearIsResetAWakeFromSleep() {
	PMM_clearInterrupt(PMM_LPM5_INTERRUPT);
}


void PMM::triggerSoftwareBORReset() {
	PMM_trigBOR();
}
