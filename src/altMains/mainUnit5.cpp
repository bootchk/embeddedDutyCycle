
/*
 * Unit test of sun detecting.
 * Requires proto hw: solar cell Vcc to pin 1.4/A4
 */

#include <SoC/SoC.h>
#include <lightSensor/lightSensor.h>

#include "../test/test.h"



int mainccc() {

    SoC::stopWatchDog();

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
