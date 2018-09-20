

#include "epochClock.h"

#include "../RTC/realTimeClock.h"



EpochTime EpochClock::timeNow() {
    return RTC::timeNow();
}



EpochTime EpochClock::timeDurationFromNow(Duration duration) {
    // TODO hack, should implement operator+
    return timeNow() + static_cast<unsigned int> (duration);
}
