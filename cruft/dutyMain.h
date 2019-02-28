
#pragma once


/*
 * Actions for duty cycling using external RTC.
 *
 * This class separates actions needed for the DutyCycle framework from App actions.
 *
 * Both the framework and the app get a slice at the same phases of main.
 * This slices: calls both the framework and the app.
 */

class DutyMain2 {
public:
    /*
     * Determine why we reset
     */
    static bool isResetAwakeFromSleep();

    /*
     * Actions for two kinds of reset.
     */
    /*
     * Cold reset: System power was cycled.
     * mcu is reset but also all other chips in system have been lost power and thus reset.
     */
    static void onColdReset();

    /*
     * Wake from LPM4.5 reset: mcu power was cycled.
     * All other chips in system (specifically the external RTC which provides alarm that woke us)
     * are still powered.
     */
    static void onWakeFromLPMReset();

    /*
     * Postlude: actions after every reset,
     * before going back into LPM4.5 i.e. power off mcu
     *
     * Note there is no preamble method per se,
     * but there are similar actions early in each reset.
     */
    static void onResetPostlude();
};
