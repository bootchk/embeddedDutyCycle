
/*
 * Unit test of LowPowerTimer
 */

// MSP430Drivers
#include <SoC/SoC.h>
#include <timer/timer.h>

#include "../test/test.h"




int mainddd() {

    SoC::stopWatchDog();

    /*
     * All routines below configure their own pins.
     */

    // No sleeping
    while (true) {
        LowPowerTimer::delayTicksOf100uSec(5000);
        Test::blinkForcedGreenLED(1);
    }
}
