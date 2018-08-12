
#pragma once


/*
 * Actions for duty cycling usng external RTC.
 */

class DutyMain {
public:
    static void onColdReset();

    static void onWakeFromLPM();

    static void onResetPostlude();
};
