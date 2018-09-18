
/*
 * Simple app: blink every ten seconds
 *
 * Formerly used without TaskScheduler (just call onAlarm when alarm goes off)
 *
 * Can be tested with TaskScheduler, where there is only one task ever scheduled.
 */

class Blinker {
public:
    static void init();
    static void onAlarm(void);
    static void configureGPIO();
};
