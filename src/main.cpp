#include <msp430.h>

#include <cassert>

#include "mainObject.h"
#include "mcuSleep.h"
#include "PMM/powerMgtModule.h"



/*
 * Basic framework for an app that sleeps in LPM4.5 between external interrupts.
 *
 * Derived from TI example code.
 * This portion should not be changed.
 *
 * See mainObject.cpp for prelude, cold reset, wake, and postlude implementations.
 *
 * It doesn't seem to work in the debugger.
 * Must disconnect from debug probe for it to work.
 * I have posted a bug report to TI forum.
 */

/*
 * Disable BSL
 * BSL might already have executed, this only affects subsequent resets.
 */
#pragma location=0xFF84
unsigned int bslSignature1=0x5555;
#pragma location=0xFF86
unsigned int bslSignature2=0x5555;


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // LPM5 might be locked
    Main::onResetPreamble();

    /*
     * !!! If no interrupt is enabled when unlocked,
     * no ISR is called, even though interrupt occurred in wakeup case.
     */

    // Dispatch on reset reason: reset is wake out of an LPMx.5 OR any other (typically cold start.)
    if ( Main::isResetAwakeFromSleep()) {
        PMM::unlockLPM5();
        Main::onWakeFromLPM();
    }
    else {
        // Device powered up from a cold start or other reset reason
        // assert(not PMM::isLockedLPM5());
        // LPM5 might be locked if reset reason is not a cold start
        Main::onColdReset();
    }

    assert(not PMM::isLockedLPM5());

    // Configure one or more wakeup sources
    Main::onResetPostlude();

    // require a wakeup source else never wake

    // assert GPIO configured for sleeping, to soon be locked

    /*
     * Some TI sources say there is a race here,
     * that this cannot be wrapped in a function call.
     * That is, powering down the core regulator
     * leaves little time to enter sleep.
     */
    // This gives LPMx.5 (cpu unpowered)
    PMMCTL0_H = PMMPW_H;                // Unlock PMM Registers
    PMMCTL0_L |= PMMREGOFF;             // Core regulator off
    //PMMCTL0_H = 0;                    // Lock PMM Registers

    // Enter LPM4 Note that this operation does not return. LPM4.5 sleep
    // will exit via a RESET event, resulting in re-start of code.
    __bis_SR_register(LPM4_bits | GIE);
    __no_operation();
    assert(false);
}
