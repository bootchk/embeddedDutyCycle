
/*
 * Test whether LED on launchpad generates voltage
 *
 * in board.h, temporarily configure ADC pin to A0 instead of A4
 */

#include "../MCU/mcu.h"
#include "../debug/test.h"
#include "../timer/timer.h"
#include "../peripheral/ADC/adc.h"
#include "../peripheral/ADC/ADCConfigure.h"
#include "../peripheral/LED/led.h"


void measureLight() {
    // Measure light using LED on P1.0
   ADC::configureForExternalPinVoltageProportionToVcc();

   // Delay to allow charge on pin to dissipate?
   LowPowerTimer::delayTicksOf100uSec(10000);  // 1 second

   unsigned int foo = ADC::readExternalPinVoltageProportionToVcc();
   ADC::releaseExternalPin();

    if (foo < 90)
    {
        Test::blinkForcedGreenLED(1);
    }

}

int main() {

    MCU::stopWatchDog();

    // No sleeping
    while (true) {
        // 5 seconds
        LowPowerTimer::delayTicksOf100uSec(50000);

        measureLight();

        // Same pin as used to measure light, now used to generate light
        LED::configureLED1();
        LED::blink();
    }
}
