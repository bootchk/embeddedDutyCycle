
#pragma once

#include "parameters.h"




/*
 * BlinkPeriod is an interval of time during which we blink (not necessarily continuously).
 * Typically at night.
 * BlinkPeriod comprises two subperiods, evening and morning, separated by period of non-blinking.

 * Knows count of blinks in blink periods.
 *
 * We schedule the blinking task, not an end of period task.
 *
 * BlinkPeriod comprises two subperiods, evening and morning, separated by time.
 *
 * Sequence of calls is:
 * initForEveningBlinking,
 * advance, isOver, ..., advance, isOver=>true,
 * isEvening()=>false
 * initForMorningBlinking
 * advance, isOver, ..., advance, isOver=>true,
 */





class BlinkPeriod {

public:
    static void initForEveningBlinking();

    static void initForMorningBlinking();

    static bool isActive();

    /*
     * Side effect: deactivate when over
     */
    static bool isOver();

    static bool isEvening();

    static void advance();
};
