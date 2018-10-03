
#pragma once


/*
 * Actions for duty cycling usng external RTC.
 *
 * This class separates actions needed for the DutyCycle framework from App actions.
 *
 * Generally speaking, both the framework and the app get a slice at the same phases of main.
 * Thats all this does, call both the framework and the app.
 */

class DutyMain {
public:
    static void onColdReset();

    static void onWakeFromLPM();

    static void onResetPostlude();
};
