
#include "day.h"

#include "../epochClock/epochClock.h"



static EpochTime previousSunrise = 0;


void Day::setSunrise() {
    previousSunrise = EpochClock::timeNow();
}


EpochTime Day::timeTwoHoursBeforeSunrise() {
    /*
     * Next sunrise is previous + 24 hours.
     */
    return previousSunrise + static_cast<unsigned int>(Duration::TwentyTwoHours);
}
