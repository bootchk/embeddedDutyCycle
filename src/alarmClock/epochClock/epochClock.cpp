

#include "epochClock.h"

#include "../RTC/realTimeClock.h"



EpochTime EpochClock::timeNow() {
    return RTC::timeNowOrReset();
}



EpochTime EpochClock::timeDurationFromNow(Duration duration) {
    return timeDurationFromTime(timeNow(), duration);
}

EpochTime EpochClock::timeDurationFromTime(EpochTime time, Duration duration) {
    // When Duration type
    // return time + static_cast<unsigned int> (duration);
    return time + duration;
}

