
/*
 * Unit test of sun detecting.
 * Requires proto hw: solar cell Vcc to pin 1.4/A4
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
        LowPowerTimer::delay(5000);
        Test::blinkForcedGreenLED(1);
    }
}
