
#include "solarPower.h"

#include "../timer/timer.h"
#include "../blinkerAppTasked/parameters.h"
#include "../peripheral/ADC/adc.h"



bool SolarPower::isPowerForStarting() {
    // Does not require GPIO configuration
    // Does not change GPIO configuration

    // Greater than 1.9 volts
    return ADC::measureVccCentiVolts() > Parameters::MinVccForStarting ;
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
