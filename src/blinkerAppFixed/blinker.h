
#include "../alarmClock/RTC/timeTypes.h"


/*
 * Simple app: blink every ten seconds
 *
 * Without TaskScheduler (just sets alarm to a constant duration from now)
 */

class Blinker {
public:
    static void init();
    static void onAlarm(void);
    static void configureGPIO();
    static EpochTime timeToWake();
};
