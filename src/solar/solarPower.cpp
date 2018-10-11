
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
    // 50k * 10uSec tick is 500kuSec is .5 seconds
    LowPowerTimer::delay(50000);

    while (not isPowerForStarting())
        LowPowerTimer::delay(50000);
}
