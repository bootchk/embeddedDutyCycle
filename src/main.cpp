#include <msp430.h>

#include "mainObject.h"
#include "PMM/powerMgtModule.h"
#include "MCU/mcu.h"
#include "timer/timer.h"
#include "pinFunction/pinFunction.h"
#include "solar/solarPower.h"

#include "debug/myAssert.h"
#include <src/debug/test.h>



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
 * For debugging.
 * Without this, the debugging probe makes the app always think it is wake from sleep?
 */
#pragma PERSISTENT
bool didColdstart = false;


namespace {

void configureSystem() {
    MCU::enableBSLOffAndVacantMemoryNMI();

    // Prevent NMI on FRAM writes
    MCU::disableFRAMWriteProtect();
}

/*
 * Delay is needed for some unknown reason.
 * When I remove this delay, fails assert RTC::isReadable in alarmConfigure
 * !!! This delay, determined experimentally, seems necessary.
 * Originally it was on every reset.
 * It also seems to work only for coldstart.
 */

#ifdef UNUSED
void delayForStartup()
{
    ///Test::blinkForcedGreenLED(1);

    // 500k cycles at 8mHz SMCLK is .05 seconds
    //Test::delayHalfMillionCycles();

    // 5k * 10uSec tick is 50kuSec is .05 seconds
    LowPowerTimer::delay(5000);
}
#endif

}


/*
 * !!! If no interrupt is enabled when unlocked,
 * no ISR is called, even though interrupt occurred in wakeup case.
 */


int main(void)
{
    /*
     * Conditions on every reset:
     * - watchdog running
     * - VMA NMI disabled
     * Conditions on some resets (LPMx.5 exit)
     * - LPM5 locked
     */
    MCU::stopWatchDog();

    configureSystem();

    ///delayForStartup();
    ///Main::onResetPreamble();

    // Dispatch on reset reason: reset is wake out of an LPMx.5 OR any other (typically cold start.)
    if ( Main::isResetAwakeFromSleep() and didColdstart ) {

        ///delayForStartup();
        ///Main::onResetPreamble();
        PinFunction::setUsedOutPinValues();
        PinFunction::configureUnusedPinsLowPower();

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
        /*
         * Reset from power up (cold start) or reset reason other than wake from LPM4.5
         * LPM5 might be locked if reset reason is not a cold start
         */
        didColdstart = true;

        /*
         * Need to configure for low power (because pins are floating input using power)
         * and set output values (because they default to LED on)
         * BEFORE delay for Startup
         */
        PinFunction::setUsedOutPinValues();
        PinFunction::configureUnusedPinsLowPower();

        // When solar powered, must sleep until power is adequate.
        SolarPower::sleepUntilPowerReserve();
        //delayForStartup();
        ///Main::onResetPreamble();

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
     * Enter LPM4 or LPM4.5.
     * Since core regulator is off, this will be LPM4.5.
     * LPM4.5 does not return, sleep will end via a RESET event and continuation is at main().
     */
    __bis_SR_register(LPM4_bits | GIE);
    __no_operation();
    ///__delay_cycles(2);
    myAssert(false);
}
