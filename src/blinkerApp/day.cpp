
#include "day.h"

#include "../epochClock/epochClock.h"

namespace {
/*
 * In persistent memory, to survive low power mode.
 * They must be initialized on cold restart by a call to init.
 */
#pragma PERSISTENT
static EpochTime previousSunrise;
#pragma PERSISTENT
static bool _isSunriseTimeValid ;

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
    return previousSunrise + static_cast<unsigned int>(Duration::TwentyTwoHours);
}
