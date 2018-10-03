
#include "../RTC/timeTypes.h"   // EpochTime, Duration


/*
 * EpochTime implemented on RTC.
 */


class EpochClock {
public:
    static EpochTime timeNow();
    static EpochTime timeDurationFromNow(Duration);
    static EpochTime timeDurationFromTime(EpochTime, Duration);
};
