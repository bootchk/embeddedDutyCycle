
/*
 * Test whether LED on launchpad generates voltage
 *
 * in board.h, temporarily configure ADC pin to A0 instead of A4
 */

#include "../MCU/mcu.h"
#include "../debug/test.h"
#include "../timer/timer.h"
#include "../peripheral/ADC/adc.h"
#include "../peripheral/LED/led.h"



void delayOneSecond() {
    LowPowerTimer::delayTicksOf100uSec(10000);  // 1 second
}


unsigned int measureLight() {
    unsigned int result;

    // Measure light using LED on P1.0
   ADC::configureForExternalPinVoltageProportionToVcc();

   result = ADC::readExternalPinVoltageProportionToVcc();
   ADC::releaseExternalPin();

   return result;
}



unsigned long long sum=0;
unsigned int sampleCount=0;
float average;
unsigned int min = 1000;
unsigned int max = 0;
int breakPointsHit;

void keepStatistics(unsigned int sample) {
    sum += sample;
    sampleCount +=1;
    average = sum / sampleCount;
    if (sample < min) min = sample;
    if (sample > max) max = sample;
    if (sampleCount > 20)
        breakPointsHit +=1;
}


void blinkAmber() {
    // Same pin as used to measure light, now used to generate light
    LED::configureLED1();

    LED::blink(); // spins, using power

    //LED::turnOn();
    //LowPowerTimer::delayTicksOf100uSec(5000);
    //LED::turnOff();
}


void sampleAndBlink() {
    unsigned int sample;
    unsigned int sample2;
    sample = measureLight();
    sample2 = measureLight();

    keepStatistics(sample);
    keepStatistics(sample2);

    // blink green led when dark
    if (sample < 90 and sample2 < 90)
    {
        Test::blinkForcedGreenLED(1);
    }
}

void spin10mSec() {
    __delay_cycles(100000);
}

int main777() {

    MCU::stopWatchDog();

    Test::blinkForcedGreenLED(1);

    // No sleeping
    while (true) {
        delayOneSecond();

        //spin10mSec();

        sampleAndBlink();

        //blinkAmber();

        // Delay to allow charge on pin to dissipate?
    }
}
