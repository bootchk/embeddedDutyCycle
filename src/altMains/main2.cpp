
#include "../duty.h"
#include "../mcuSleep.h"
#include "../app.h"

#include "../PMM/powerMgtModule.h"   // stopWatchdog

#include <msp430.h>   // PORT1_VECTOR

// assertions
#include "../SPI/spi.h"
#include "../MCU/mcu.h"

// test
#include <sysctl.h>

#include <cassert>


#include "../ADC/adc.h"



int main(void) {

	/*
	 * Reset has occurred:
	 * - from sleep
	 * - OR from power on or other reasons (Vcc faults, bus faults?)
	 */

	PMM::stopWatchdog();

	// Disable protection of BSL
	SysCtl_disableBSLProtect();

	// Test ADC
	ADC::configureSolarCellVoltagePin();
	ADC::isSolarCellDark();

	ADC::isVccHigh();

	// not effective until GPIO is unlocked
    MCUSleep::configureUnusedPinsLowPower();

	/*
	 * Dispatch on reason for wake.
	 */
	if (MCUSleep::isResetAWakeFromSleep()) {

		/*
		 * Trap to allow debugger to synch
		 */
		bool isTrapped;
		isTrapped = true;
		while(isTrapped);

		MCUSleep::clearIsResetAWakeFromSleep();

		/*
		 * GPIO config registers were reset (but GPIO state is held.)
		 * Preconfigure as it was before sleep.
		 * Effective upon unlockMCUFromSleep()
		 */
		Duty::restoreMCUToPresleepConfiguration();
		App::configureSleepingGPIO();

		MCUSleep::unlockMCUFromSleep();

		// Interrupt is serviced now, if presleep configuration enables interrupts

		Duty::onWakeForAlarm();
		// assert alarm interrupt is cleared and Duty is ready for setAlarm
		App::onWakeForAlarm();
		// assert app done with useful work
		// assert GPIO in sleeping configuration
	}
	else {	// power on reset or other reasons



		// POR reset clears lock bit but other reasons may not.  Requires unlocked, so safer to always unlock.
		MCUSleep::unlockMCUFromSleep();

		// Must clear flags else will not sleep
		PMM::clearAllResetInterruptFlags();

		/*
		 * GPIO configuration is reset.
		 * Configure Duty and App.
		 * Effective immediately, since GPIO not locked.
		 */
		Duty::onPowerOnReset();
		// assert Duty is ready for setAlarm
		App::onPowerOnReset();
		// assert app in initial state and GPIO in sleeping configuration
	}

	// Assert app is done with its useful work, or is in initial state

	// Assert app has unconfigured any devices used ephemerally in its useful work
	// Some GPIO pins that app persists during sleep may still be in use (e.g. an LED)

	// Resets if fail to set alarm
	Duty::setAlarmOrReset(App::durationOfSleep());

	Duty::lowerMCUToPresleepConfiguration();

	/*
	 * Service all interrupts, there should be none.
	 */
	MCU::enableGlobalInterrupts();

	/*
	 * Modules disabled, else will not sleep
	 */
	assert(not SPI::isEnabled());

	/*
	 * No interrupts should be flagged else will not sleep?
	 */
	// TODO not working, must enable eUSCI to clear interrupts?
	//SPI::clearInterrupt();
	//assert( not SPI::isInterrupt());

	/*
	 * Assert mcu is in presleep condition E.G. GPIO is not configured for Duty using SPI
	 *
	 * Assert some interrupt will come (E.G. a Duty Alarm) else we would sleep forever.
	 */

	/*
	 * Does not return.  Continuation is a reset.
	 * We don't need an infinite loop coded here, the loop is via interrupt and reset back to main()
	 */
	// MCUSleep::spinWithInterruptsEnabled();  // For debugging.
	// MCUSleep::enterLowestPowerSleep();
	//_low_power_mode_4();

	// Inlined
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

	// Should never get here...
	assert(false);
}
