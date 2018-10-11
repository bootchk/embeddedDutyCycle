
#pragma once

#include "../alarmClock/RTC/timeTypes.h"

// include governing definitions
#include "../board.h"

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


#ifdef TEST_PARAMETERS
    // Most durations very short so time compressed
    static const Duration BetweenBlinks = 3;
    static const Duration BetweenSunsetAndBlinking = 10;
    // Evening blinking lasts 30 seconds (10 blinks every 3 seconds)

    // Start morning two minutes after fake sunrise,sunset
    // Because of other built-in delays, allow much greater than 40 seconds
    static const Duration BetweenMorningBlinkStartAndSunrise = 86400 - 60;

    // Check sun every 15-30 seconds (in middle of blinking period)
    ///static const Duration BetweenSunChecks = 15;
    static const Duration BetweenSunChecks = 10;


    // counts for periods
    static const unsigned int BlinksEvening = 5;
    static const unsigned int BlinksMorning = 5;


#else
    static const Duration BetweenBlinks = 10;
    static const Duration BetweenSunsetAndBlinking = 1800;  // Thirty minutes

    // Check sun every 3 minutes
    ///static const Duration BetweenSunChecks = 120;
    static const Duration BetweenSunChecks = 900;  // Fifteen minutes

    static const Duration BetweenMorningBlinkStartAndSunrise = 7200;    // Two hours

    // Evening blink for 4 hours every 10 seconds
    // 6 ticks/minute * 60 minutes/hour * 4 hours
    static const unsigned int BlinksEvening = 6 * 60 * 4;

    // Morning blink for 2 hours every 10 seconds
    static const unsigned int BlinksMorning = 6 * 60 * 2;
#endif


/*
 * Voltages.
 * Depends on:
 * - power supply Vccmax (3.3V on Launchpad, 3.6V on PCB)
 * - sunlight detector
 */
static const unsigned int MinVccForBlinking = 300;  // centiVolts, 3V

#ifdef SOLAR_CELL4_2V
// solar cell KXOB22-01x8
// 2V light, .6V dark
static const unsigned int MaxVccForDark = 80;  // centiVolts, 0.8V

// (128/256)*1.5 => .75V
static const unsigned int MaxVscProportionTo1_5ForDark = 128;  // centiVolts, 0.3V128
#elif defined SOLAR_CELL2V
// solar cell KXOB22-04x3
// .5V light, .2V dark

// NOT USED static const unsigned int MaxVccForDark = 30;  // centiVolts, 0.3V

// (50/256)*1.5 => .3V
static const unsigned int MaxVscProportionTo1_5ForDark = 30;  // centiVolts, 0.3V

#endif


/*
 * Voltage that indicates a reserve of power above Vmin
 */
static const unsigned int MinVccForStarting = 190; // centiVolts 1.9V
};
