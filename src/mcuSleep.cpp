
#include "mcuSleep.h"

/*
 * Implementation is MSP430 specific.
 */
#include "MCU/mcu.h"
#include "MCU/powerMgtModule.h"
#include "MCU/pinFunction.h"





void MCUSleep::enterLowestPowerSleep(){
	 /*
	     XT1 xtal oscillator must be off.
	     See family guide, section 3.2.4 "XT1 Oscillator"
	     The HW enables it for several conditions such as:
	     - the sw has configured it as a source for a clock and the clock is active
	     - a peripheral requests it (typically RTC)
	     - sw cleared AUTOFF and the power mode is in range [active, LPM4]
	  */
	  /*
	      In this example, none of conditions exist for XT1 enabled by hw
	      assert SCG0 == 1 SCG1 == 1, OSCOFF == 1
	      but those bit field names may be for another family member?
	  */

	  // This is also necessary if XT1 is on
      // UCSCTL6 |= XT1OFF | XT2OFF;

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



void MCUSleep::unlockMCUFromSleep(){
	PMM::unlockLPM5();
}


bool MCUSleep::isResetAWakeFromSleep() {
	/*
	 * The user's guide says alternatives are:
	 * - flag PMMLPM5IFG
	 * - decode SYSRSTIV
	 */
	return PMM::isResetAWakeFromSleep();
}

void MCUSleep::clearIsResetAWakeFromSleep() {
	PMM::clearIsResetAWakeFromSleep();
}




void MCUSleep::configureAllPinsLowPower() {
	PinFunction::configureAllGPIOPinsLowPower();
}
