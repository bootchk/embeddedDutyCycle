
#include "../alarm/alarmTypes.h"    // Duration
#include "../RTC/timeTypes.h"   // EpochTime


/*
 * EpochTime implemented on RTC.
 */


class EpochClock {
public:
    static EpochTime timeNow();

    static EpochTime timeDurationFromNow(Duration);
};
