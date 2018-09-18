

/*
 * EpochTime implemented on RTC.
 */


class EpochClock {
public:
    static EpochTime timeNow();

    static EpochTime timeDurationFromNow(Duration);
};
