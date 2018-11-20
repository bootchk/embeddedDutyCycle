
/*
 * Test whether LED on launchpad generates voltage
 *
 * in board.h, temporarily configure ADC pin to A0 instead of A4
 */

#include <src/SoC/SoC.h>
#include "../peripheral/LEDAndLightSensor/ledAndLightSensor.h"

#include "../timer/timer.h"
#include "../PMM/powerMgtModule.h"

//#include "../peripheral/ADC/adc.h"
//#include "../peripheral/LED/led.h"




void delayOneSecond2() {
    LowPowerTimer::delayTicksOf100uSec(10000);  // 1 second
}

unsigned int measureLight2() {
   unsigned int result;

   result = LEDAndLightSensor::measureLight();

   return result;
}



void blinkAmber2() {
    // Same LED as used to measure light, now used to generate light
    LEDAndLightSensor::toOnFromOff();
    LowPowerTimer::delayTwentyMilliSeconds();
    LEDAndLightSensor::toOffFromOn();
}



//void spin10mSec() {
//    __delay_cycles(100000);
//}



static unsigned long long sum=0;
static unsigned int sampleCount=0;
static float average;
static unsigned int min = 1000;
static unsigned int max = 0;
static int breakPointsHit;

void keepStatistics2(unsigned int sample) {
    sum += sample;
    sampleCount +=1;
    average = sum / sampleCount;
    if (sample < min) min = sample;
    if (sample > max) max = sample;
    if (sampleCount > 20)
        breakPointsHit +=1;
}


void sampleAndBlink2() {
    unsigned int sample;
    //unsigned int sample2;
    sample = measureLight2();
    //sample2 = measureLight2();

    keepStatistics2(sample);
    //keepStatistics2(sample2);

    // blink green led when dark
    // greater value is more light
    if (sample > 25) // and sample2 < 90)
    {
        ///Test::blinkForcedGreenLED(1);
        blinkAmber2();
    }
}




/*
 * Behaviour:
 *
 * Every cycle:
 * - red LED should blink.
 * - green LED should blink if LED illuminated by night dark
 */

int main() {

    SoC::stopWatchDog();

    ///Test::blinkForcedGreenLED(1);
    // assert LPM5 is unlocked because blinkForced unlocked it.

    LEDAndLightSensor::toOffFromUnconfigured();
    PMM::unlockLPM5();

    SoC::unlockMCUFromSleep();

    // No sleeping
    while (true) {
        delayOneSecond2();
        delayOneSecond2();

        blinkAmber2();

        sampleAndBlink2();
    }
}
