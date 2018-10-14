
/*
 * Unit test of LowPowerTimer
 */

#include "../MCU/mcu.h"
#include "../debug/test.h"
#include "../timer/timer.h"



int mainddd() {

    MCU::stopWatchDog();

    /*
     * All routines below configure their own pins.
     */

    // No sleeping
    while (true) {
        LowPowerTimer::delayTicksOf100uSec(5000);
        Test::blinkForcedGreenLED(1);
    }
}
