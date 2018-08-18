#include <msp430.h>

#include <cassert>

#include "mainObject.h"
#include "mcuSleep.h"
#include "PMM/powerMgtModule.h"
#include "MCU/mcu.h"

#include "testMain.h"
#include "debug.h"



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



int main6(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    Debug::leaveCrumb(1);

    MCU::enableBSLOffAndVacantMemoryNMI();

    // Prevent NMI on FRAM writes
    MCU::disableFRAMWriteProtect();

    // LPM5 might be locked
    // Cannot blink LEDs when LPM5 locked
    Main::onResetPreamble();
    Debug::leaveCrumb(2);
    /*
     * !!! If no interrupt is enabled when unlocked,
     * no ISR is called, even though interrupt occurred in wakeup case.
     */

    // Dispatch on reset reason: reset is wake out of an LPMx.5 OR any other (typically cold start.)
    if ( Main::isResetAwakeFromSleep()) {

#ifdef TRAP_WAKE
        // Trap to allow debugger to synch when using "Free Run" ?
        // You can set variable isTrapped using debugger to proceed.
        bool isTrapped;
        isTrapped = true;
        while (isTrapped)
            ;
#endif
        Main::onWakeFromLPM();
        TestMain::blinkRedLED(5);
    }
    else {
        // Device powered up from a cold start or other reset reason
        // assert(not PMM::isLockedLPM5());
        // LPM5 might be locked if reset reason is not a cold start
        Main::onColdReset();
        TestMain::blinkRedLED(2);
    }

    assert(not PMM::isLockedLPM5());

    // Configure one or more wakeup sources
    Main::onResetPostlude();

    TestMain::blinkRedLED(10);

    // require a wakeup source else never wake

    // assert GPIO configured for sleeping, to soon be locked

    /*
     * Some TI sources say there is a race here,
     * that this cannot be wrapped in a function call.
     * That is, powering down the core regulator
     * leaves little time to enter sleep.
     */
    // Core regulator off gives LPMx.5 (cpu unpowered)
    // Disable SVSH saves more current
    PMMCTL0_H = PMMPW_H;                // Unlock PMM Registers
    PMMCTL0_L &= ~(SVSHE);              // Disable SVS high side
    PMMCTL0_L |= PMMREGOFF;             // Core regulator off
    //PMMCTL0_H = 0;                    // Lock PMM Registers

    /*
     * Enter LPM4 or LPM4.5
     * If LPM4.5, this does not return.
     * LPM4.5 sleep will exit via a RESET event, resulting in re-start of code.
     */
    __bis_SR_register(LPM4_bits | GIE);
    __no_operation();
    assert(false);
}
