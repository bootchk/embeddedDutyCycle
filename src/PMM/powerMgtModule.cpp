
#include "powerMgtModule.h"


// mcu hal layer e.g. MSPWare DriverLib
// depends on msp430.h
#include <pmm.h>

// some mcu family members have watchdog a and b
// #include <wdt_a.h>

#include <cassert>



/*
 * Hack: use bare metal instead of DriverLib.  Should be equivalent.
 */
void PMM::stopWatchdog() {
	// bare metal: password & disable bit
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

void PMM::clearAllResetInterruptFlags() {
    PMM_clearInterrupt(PMM_ALL);
}




void PMM::triggerSoftwareBORReset() {
    // TODO
    /*
     * For debugging, uncomment this assert and catch an attempted reset
     * in an infinite loop (see definition of assert.)
     */
    assert(false);

	PMM_trigBOR();
}
