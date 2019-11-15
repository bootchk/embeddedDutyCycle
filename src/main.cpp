
// TI
#include <msp430.h>

// MSP430Drivers
#include <PMM/powerMgtModule.h>
#include <SoC/SoC.h>
#include <assert/myAssert.h>

// For assertions on drivers in msp43Drivers
#include <i2c/i2c.h>


// embeddedDutyCycle framework
#include "dutyCycle/solarPower.h"
#include "dutyCycle/duty.h"
#include "dutyCycle/centralSystem.h"
#include "pinFunction/pinFunction.h"
#include "app/app.h"


// Assertions
#include "pinFunction/allPins.h"

// Testing
// #include <src/debug/test.h>
// #include "delay.h"



/*
 * Basic framework for an app that sleeps in LPM4.5 between external interrupts.
 *
 * Derived from TI example code.
 *
 * Ideally, none of the framework should be changed; only implement the hooks.
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

    CentralSystem::configureAfterReset();

    ///delayForStartup();

    // Dispatch on reset reason: reset is wake out of an LPMx.5 OR any other (typically cold start.)
    if ( SoC::isResetWakeFromSleep() and didColdstart ) {

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

        SoC::clearIFGForResetWakeFromSleep();
        SoC::unlockGPIOFromSleep();
        // Interrupt would be serviced now, but the configuration at this point has NOT enabled alarm interrupt

        /*
         * The order of the next two calls is important.
         * If App expects to use EpochClock, which depends on RTC, then Alarm must be configured first.
         */

        /*
         * Clear the alarm IFG and ready Alarm (and its dependency: RTC, and therefore also ready EpochClock)
         * RTC chip should not have POR reset, and remains configured.
         */
        Duty::prepareForAlarmingAfterWake();

        /*
         * App may use GPIO and modules not reserved for framework.
         * App must leave any such GPIO and modules in sleeping state.
         */
        App::onWakeForAlarm();
#ifdef TEMP
        /*
         * Clear the alarm IFG and ready Alarm (and its dependency: RTC, and therefore also ready EpochClock)
         * RTC chip should not have been reset, and remains configured.
         */
        Duty::prepareForAlarmingAfterWake();
#endif
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

        // When solar powered, must sleep until power is adequate.
        SolarPower::sleepUntilPowerReserve();

        /*
         * LPM5 should not be locked if is a true cold restart, but debugger may do funny things.
         */
        if (PMM::isLockedLPM5()) PMM::unlockLPM5();

        /*
         * App go to initial state
         */
        App::onPowerOnReset();

        /*
         * RTC chip also might have been power on reset.
         * If so, RTC needs configuring.
         */
        Duty::prepareForAlarmingAfterColdReset();
    }

    myAssert(not PMM::isLockedLPM5());
    // assert we configured GPIO and modules needed for Alarm.

    /*
     * App usually is using task scheduler.
     * Some task must be scheduled. Scheduled task determines duration of sleep.
     * !!! Duration may be zero, but Alarm handles that by bumping to 2.
     */
    Duty::setDurationAlarmOrReset(App::durationOfSleep());

    App::hookBeforeSleep();

    // Undo GPIO and modules for Alarm
    Duty::lowerMCUToPresleepConfiguration();

    /*
     * Require GPIO configured for sleeping, soon to be locked.
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
    // TODO assert ADC off

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
