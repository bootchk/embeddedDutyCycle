
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

    /*
     * Events
     * Determined by tasks, from external events.
     */
    static void onSunriseDetected();


    static void scheduleBlinkTask();
    static void scheduleCheckSunriseTask();
};
