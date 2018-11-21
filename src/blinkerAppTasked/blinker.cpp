
#include "blinker.h"

// MSP430Drivers

// Use two-sided, dual purpose LED
#include <LEDAndLightSensor/ledAndLightSensor.h>
// Use low power timer
#include <timer/timer.h>


void Blinker::blink()
{
    // Same LED as used to measure light, now used to generate light
    LEDAndLightSensor::toOnFromOff();

    LowPowerTimer::delayTwentyMilliSeconds();

    LEDAndLightSensor::toOffFromOn();
}
