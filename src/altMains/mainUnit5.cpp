
/*
 * Unit test of sun detecting.
 * Requires proto hw: solar cell Vcc to pin 1.4/A4
 */

#include "../MCU/mcu.h"
#include "../blinkerAppTasked/powerMgr.h"
#include "../debug/test.h"



int main() {

    MCU::stopWatchDog();

    /*
     * All routines below configure their own pins.
     */

    // No sleeping
    while (true) {
        if (PowerMgr::isSolarCellDark()) {
            Test::blinkForcedGreenLED(1);
        }
    }
}
