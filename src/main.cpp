
// TI
#include <msp430.h>

// MSP430Drivers
#include <PMM/powerMgtModule.h>
#include <SoC/SoC.h>
#include <assert/myAssert.h>
#include <bridge/serialBus/i2c/i2c.h>

#include "dutyCycle/dutyMain.h"
#include "pinFunction/pinFunction.h"

// Assertions
#include "pinFunction/allPins.h"

// Testing
//#include <src/debug/test.h>
#include <timer/timer.h>



/*
 * Basic framework for an app that sleeps in LPM4.5 between external interrupts.
 *
 * Derived from TI example code.
 *
 * Ideally, none of the framework shouldbe changed; only implement the hooks.
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
 * !!! May need to reload program while debugging to reset this.
 */
#pragma PERSISTENT
bool didColdstart = false;


namespace {

/*
 * Configure system (after reset)
 * - faults we want to catch (NMI for vacant memory access)
 * - capability we want (write to FRAM)
 */
void configureSystem() {
    SoC::enableBSLOffAndVacantMemoryNMI();

    // Prevent NMI on FRAM writes
    SoC::disableFRAMWriteProtect();

    /*
     *  XT1 defaults to autooff.
     *  Framework does not use XT1, so it should be off and this is not required.
     */
    /// SoC::disableXT1();

    // This doesn't seem to have any effect.
    // Docs show that SMCLK defaults to on, but since it is driven by MCLK and divisor is 1,
    // turning it off does not save power.  But we drive eUSCI_B with it?
    //SoC::turnOffSMCLK();
}

/*
 * Delay is needed for some unknown reason.
 * When I remove this delay, fails assert RTC::isReadable in alarmConfigure
 * !!! This delay, determined experimentally, seems necessary.
 * Originally it was on every reset.
 * It also seems to work only for coldstart.
 */


void delayForStartup()
{
    LowPowerTimer::delayHalfSecond();
}


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
    SoC::stopWatchDog();

    configureSystem();

    ///delayForStartup();

    // Dispatch on reset reason: reset is wake out of an LPMx.5 OR any other (typically cold start.)
    if ( DutyMain::isResetAwakeFromSleep() and didColdstart ) {

        //delayForStartup();
        PinFunction::configureToSleepState();
        AllPins::assertAreConfiguredForSleep();

#ifdef TRAP_WAKE
        // Trap to allow debugger to synch when using "Free Run" ?
        // You can set variable isTrapped using debugger to proceed.
        bool isTrapped;
        isTrapped = true;
        while (isTrapped)
            ;
#endif

        DutyMain::onWakeFromLPMReset();
    }
    else {
        /*
         * Reset from power up (cold start) or reset reason other than wake from LPM4.5
         * LPM5 might be locked if reset reason is not a cold start
         */
        didColdstart = true;

        /*
         * Need to configure for low power (because pins are floating input using power)
         * and set output values (because they are indeterminate, possibly LED on)
         * BEFORE delay for Startup
         */
        PinFunction::configureToSleepState();
        AllPins::assertAreConfiguredForSleep();

        //delayForStartup();

        DutyMain::onColdReset();
    }

    myAssert(not PMM::isLockedLPM5());

    //LowPowerTimer::delayHalfSecond();

    // Configure one or more wakeup sources
    DutyMain::onResetPostlude();

    // require a wakeup source else never wake

    /*
     * Ensure GPIO configured for sleeping, to soon be locked.
     * (FUTURE: assert only alarm pin is an input.)
     *
     * This assertion holds:
     * - just before we sleep
     * - after we wake, just before we unlock.
     * If we attempt to sleep when assertion if false, power consumption could be excess.
     * If we unlock while assertion is false, power consumption could be excess.
     */
    AllPins::assertAreConfiguredForSleep();

    /*
     * Assert no modules in use.
     * These are assertions about their state, not about their pins.
     * There exists some overlap in these assertions.
     * These only cover the modules used by the framework, not by the app.
     * The app must no use any modules during sleep (else sleep won't be LPM4.5)
     */
    myAssert(I2C::isInSleepState());
    // TODO assert internal Timer RTC off

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
