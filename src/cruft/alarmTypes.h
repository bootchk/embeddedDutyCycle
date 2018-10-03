
#pragma once


/*
 * Cruft:  moved to timeTypes.h
 */

/*
 * Primitive types for AlarmLib
 */

///typedef unsigned int Duration ;

/*
 * Durations in seconds.
 * Specific to blinker app
 */
enum class Duration : unsigned long {
    FiveSeconds = 5,
    TenSeconds = 10,
    FifteenMinutes = 900,
    ThirtyMinutes = 1800,
    TwentyTwoHours = 79200 // 3600 * 22,
};
