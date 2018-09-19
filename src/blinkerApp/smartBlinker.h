
/*
 * App:
 * - blink in two periods, after sunset and before sunset
 * - detect sunrise and sunset
 *
 * A set of tasks and events
 */

class SmartBlinker {
public:
    static void init();
    static void onAlarm(void);
    static void configureGPIO();

    /*
     * Scheduled tasks
     */
    static void checkSunriseTask();
    static void checkSunsetTask();
    static void blinkTask();

    /*
     * Events
     * Determined by tasks, from external events.
     */
    static void onSunriseDetected();
    static void onSunsetDetected();
    static void onPowerLevelGood();
    static void onEveningBlinkPeriodOver();


    /*
     * Scheduling
     */
    static void scheduleBlinkTask();
    static void scheduleCheckSunriseTask();
    static void scheduleCheckSunsetTask();
    ///static void scheduleLaggedCheckSunsetTask();
    static void scheduleFirstEveningBlinkTask();
    static void scheduleFirstMorningBlinkTask();
};
