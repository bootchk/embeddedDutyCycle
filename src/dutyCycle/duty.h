

#include <alarmClock/time/timeTypes.h>  // EpochTime

/*
 * Duty cycle the mcu (sleep, wake on alarm from rtc.)
 *
 * Layer below main.c
 * Hides implementation of alarms.
 *
 * Specialized for MSP430 LPM4.5.
 *
 * The library sw resets the mcu when it detects improper state.
 * In the implementation assert(false) software resets mcu.
 * asserts are NOT optional (should not be disabled with NDEBUG).
 *
 * Improper state could happen during falling Vcc, if the RTC is in reset before the mcu.
 * The design does not depend on any particular timing of mcu and rtc entering reset on falling Vcc.
 */





class Duty {
public:
	/*
	 * Tear down the RTC SPI interface
	 * (but the alarm pin remains configured.)
	 */
	static void lowerMCUToPresleepConfiguration();

	/*
	 * Same configuration as ensured by lowerMCUToPresleepConfiguration().
	 */
	static void restoreMCUToPresleepConfiguration();



	/*
	 * Caller should follow immediately with:
	 * - restoreMCUToPresleepConfiguration()
	 * - sleep
	 *
	 * Duration must be long enough for the above.
	 * As currently coded, will not work if the duration has expired before sleep.
	 */
	static void setDurationAlarmOrReset(Duration);

	/*
	 * Time must be beyond now time else sleep forever.
	 */
	static void setTimeAlarmOrReset(EpochTime);

	/*
	 * Clear alarm interrupt flag.
	 * Does not clear alarm on RTC chip.
	 */
	static void clearAlarmOnMCU();


	/*
	 * Configure GPIO and modules so we can talk to RTC chip.
	 * Does not configure RTC chip.
	 */
	static void prepareForAlarmingAfterWake();

	/*
     * Configure GPIO and modules so we can talk to RTC chip.
     * AND configure RTC chip.
     * Does not configure GPIO for alarm signal (it is an input, configured earlier.)
     */
    static void prepareForAlarmingAfterColdReset();

};


#ifdef OLD
/*
     * Configures RTC interface and RTC for duty cycling Alarm
     *
     * Called infrequently.
     * The system is intended to avoid power on reset caused by exhausting power.
     *
     * Exceptions:
     * sw resets the mcu if rtc is non-communicative (times out or other error).
     * Since this is called on reset, and might sw reset the mcu,
     * the mcu could continually reset while rtc is not functioning.
     */
    static void onPowerOnReset();


    /*
     * Handle duty cycling aspect of waking:
     * Restore interface to RTC and clear the alarm.
     *
     * Called on wake for alarm interrupt,
     * which is a reset (for TI LPM4.5, called BOR reset)
     * Called frequently as part of duty cycling.
     * RTC has not been reset.
     */
    static void onWakeForAlarm();

#endif
