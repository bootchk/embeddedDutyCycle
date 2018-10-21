
/*
 * Unit test of sun detecting.
 * Requires proto hw: solar cell Vcc to pin 1.4/A4
 */

#include "../MCU/mcu.h"
#include "../peripheral/lightSensor/lightSensor.h"
#include "../debug/test.h"



int mainccc() {

    MCU::stopWatchDog();

    /*
     * All routines below configure their own pins.
     */

    // No sleeping
    while (true) {
        if (LightSensor::isDark()) {
            Test::blinkForcedGreenLED(1);
        }
    }
}
