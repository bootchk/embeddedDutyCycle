#include <msp430.h>
#include <src/debug/test.h>

#include "mainObject.h"
#include "mcuSleep.h"
#include "PMM/powerMgtModule.h"
#include "MCU/mcu.h"

#include "debug/myAssert.h"
///#include "debug.h"



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


/*
 * For debugging.
 * Without this, the debugging probe makes the app always think it is wake from sleep?
 */
#pragma PERSISTENT
bool didColdstart = false;





int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    /// Debug::leaveCrumb(1);

    MCU::enableBSLOffAndVacantMemoryNMI();

    /*
     * When I remove the blinkForced, fails assert RTC::isReadable in alarmConfigure
     * !!! This delay, determined experimentally, seems necessary, for unknown reasons.
     */
    ///Test::blinkForcedGreenLED(1);
    Test::delayHalfMillionCycles();

    // Prevent NMI on FRAM writes
    MCU::disableFRAMWriteProtect();

    // LPM5 might be locked
    // Cannot blink LEDs when LPM5 locked
    Main::onResetPreamble();
    /// Debug::leaveCrumb(2);
    /*
     * !!! If no interrupt is enabled when unlocked,
     * no ISR is called, even though interrupt occurred in wakeup case.
     */

    // Dispatch on reset reason: reset is wake out of an LPMx.5 OR any other (typically cold start.)
    if ( Main::isResetAwakeFromSleep() and didColdstart ) {

#ifdef TRAP_WAKE
        // Trap to allow debugger to synch when using "Free Run" ?
        // You can set variable isTrapped using debugger to proceed.
        bool isTrapped;
        isTrapped = true;
        while (isTrapped)
            ;
#endif
        ///Test::blinkForcedGreenLED(3);
        ///Debug::leaveCrumb(20);

        Main::onWakeFromLPM();
        ///Test::blinkRedLED(5);
    }
    else {
        didColdstart = true;

        /*
         * Device reset from power up (cold start) or reset reason other than wake from LPM4.5
         * LPM5 might be locked if reset reason is not a cold start
         */

        /// Debug::leaveCrumb(10);
        ///Test::blinkForcedGreenLED(2);

        Main::onColdReset();
        ///Test::blinkRedLED(2);
    }

    myAssert(not PMM::isLockedLPM5());

    /// Debug::leaveCrumb(30);
    // Configure one or more wakeup sources
    Main::onResetPostlude();

    ///Test::blinkRedLED(10);
    ///Test::delayBriefly();

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
     * LPM4.5 does not return, sleep will end via a RESET event and re-start at main().
     */
    __bis_SR_register(LPM4_bits | GIE);
    __no_operation();
    myAssert(false);
}
