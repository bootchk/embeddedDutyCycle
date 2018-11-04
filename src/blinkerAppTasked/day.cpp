
#include <src/blinkerAppTasked/day.h>
#include "../alarmClock/epochClock/epochClock.h"
#include "parameters.h"

#include "../debug/myAssert.h"





namespace {
/*
 * In persistent memory, to survive low power mode.
 * They must be initialized on cold restart by a call to init.
 */
#pragma PERSISTENT
EpochTime previousSunrise = 0;

#pragma PERSISTENT
bool _isSunriseTimeValid = false;

}




void Day::init() {
    _isSunriseTimeValid = false;
}



void Day::setSunriseTime() {
    previousSunrise = EpochClock::timeNow();
    _isSunriseTimeValid = true;
}


bool Day::isSunriseTimeValid() { return _isSunriseTimeValid; }




EpochTime Day::timeBeforeNextSunriseBySeconds(Duration duration) {
    /*
     * Next sunrise is previous + 24 hours.
     */
    myAssert(isSunriseTimeValid());
    return EpochClock::timeDurationFromTime(previousSunrise, Parameters::TwentyFourHours - duration);
}
