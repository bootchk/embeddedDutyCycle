
#include <src/blinkerAppTasked/day.h>
#include "../alarmClock/epochClock/epochClock.h"

#include "../debug/myAssert.h"





namespace {
/*
 * In persistent memory, to survive low power mode.
 * They must be initialized on cold restart by a call to init.
 */
#pragma PERSISTENT
static EpochTime previousSunrise = 0;
#pragma PERSISTENT
static bool _isSunriseTimeValid = false;

}




void Day::init() {
    _isSunriseTimeValid = false;
}



void Day::setSunriseTime() {
    previousSunrise = EpochClock::timeNow();
    _isSunriseTimeValid = true;
}


bool Day::isSunriseTimeValid() { return _isSunriseTimeValid; }




EpochTime Day::timeTwoHoursBeforeSunriseTime() {
    /*
     * Next sunrise is previous + 24 hours.
     */
    myAssert(isSunriseTimeValid());
    return EpochClock::timeDurationFromTime(previousSunrise, Duration::TwentyTwoHours);
}
