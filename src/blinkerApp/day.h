
#include "../RTC/timeTypes.h"   // EpochTime



/*
 * Knows certain times of day transitions i.e. sunset, sunrise
 */

class Day {


public:
    static void setSunrise();

    static EpochTime timeTwoHoursBeforeSunrise();
};
