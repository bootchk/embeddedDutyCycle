
#include "blinker.h"

// Use two-sided, dual purpose LED
#include "../peripheral/LEDAndLightSensor/ledAndLightSensor.h"
// Use low power timer
#include "../timer/timer.h"


void Blinker::blink()
{
    // Same LED as used to measure light, now used to generate light
    LEDAndLightSensor::toOnFromOff();

    /*
     * Delay 10 mSec
     */
    // TODO Parameters::
    LowPowerTimer::delayTicksOf100uSec(100);

    LEDAndLightSensor::toOffFromOn();
}
