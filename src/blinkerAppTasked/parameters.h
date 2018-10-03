
#pragma once

#include "../alarmClock/RTC/timeTypes.h"



/*
 * Parameters of application (smart blinking)
 *
 * Two sets:
 * - normal
 * - for testing in compressed time
 */



class Parameters {
public:

    static const Duration TwentyFourHours = 86400;

#define TEST_PARAMETERS
#ifdef TEST_PARAMETERS
    // Most durations very short so time compressed
    static const Duration BetweenBlinks = 5;
    static const Duration BetweenSunsetAndBlinking = 30;
    // Evening blinking lasts 60 seconds (12 blinks every 5 seconds)

    // Start morning one minute after last evening blink
    static const Duration BetweenMorningBlinkStartAndSunrise = 86400 - 150;

    static const Duration BetweenSunChecks = 60;

    // counts for periods
    static const unsigned int BlinksEvening = 12;
    static const unsigned int BlinksMorning = 12;


#else
    static const Duration BetweenBlinks = 10;
    static const Duration BetweenSunsetAndBlinking = 1800;  // Thirty minutes

    static const Duration BetweenSunChecks = 900;  // Fifteen minutes

    static const Duration BetweenMorningBlinkStartAndSunrise = 7200;    // Two hours

    // Evening blink for 4 hours every 10 seconds
    // 6 ticks/minute * 60 minutes/hour * 4 hours
    static const unsigned int BlinksEvening = 6 * 60 * 4;

    // Morning blink for 2 hours every 10 seconds
    static const unsigned int BlinksMorning = 6 * 60 * 2;
#endif

};
