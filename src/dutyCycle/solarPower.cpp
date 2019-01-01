
#include "solarPower.h"

// msp430Drivers
#include <timer/timer.h>
#include <ADC/adc.h>
#include <board.h>





bool SolarPower::isPowerForStarting() {
    // Does not require GPIO configuration
    // Does not change GPIO configuration

    /*
     * Greater than mcu's Vmin typically 1.9 volts or 190 centivolts
     *
     * Formerly a parameter of the app (Parameters::MinVccForStarting), now a parameter of the board.
     */
    return ADC::measureVccCentiVolts() >  MinVccForStarting;
}




// TODO magic number constants

void SolarPower::sleepUntilPowerReserve() {

    /*
     * Start by sleeping long enough to build a reserve
     * that will sustain the first ADC use to read Vcc.
     */
    LowPowerTimer::delayFiveSeconds();

    while (not isPowerForStarting())
        LowPowerTimer::delayFiveSeconds();
}
