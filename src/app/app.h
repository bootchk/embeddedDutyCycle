

#include <time/timeTypes.h>   // EpochTime, Duration



/*
 * API between DutyCycle framework and an app that does useful stuff.
 * Called from main at apropos events of duty cycle.
 * An alternative is to register callbacks with the duty cycling framework?
 *
 * Groups of functions:
 *
 * - hooks into duty cycle phases: can be impotent, do not affect duty cycle function
 * -- onPowerOnReset
 * -- onWakeForAlarm
 *
 * - hooks called by duty cycle postlude
 * -- alarm duration functions
 * -- sleeping GPIO configuration
 *
 *
 *
 * Pin and Module configuration
 *
 * The app knows which modules and pins it uses directly.
 * The framework uses certain modules and pins.
 * Only the app knows which pins are not used.
 * The app, called by the framework, configures not used pins to low power.
 *
 * The framework calls configureUnsedPins() at the appropriate time.
 * The app implements.
 * The app must not declare unused the pins that the framework uses.
 *
 * The framework calls configuredAppUsedPins() at the appropriate time.
 *
 * TODO this is not clear.
 *
 * Used means: pin is active during sleep.
 * The app is free to configure and use other pins during wake,
 * as long as it reconfigures them to unused, low power state after use.
 *
 *
 * Sharing modules with the framework.
 *
 * The framework uses Alarm, which uses Serial (currently SPI.)
 * The app can only use certain functions of Alarm, namely nowTime().
 * The app should not setAlarm(), that is reserved for the framework.
 *
 * The app can also use the Serial bus, consecutively with the framework.
 * The framework leaves the Serial bus in an idle state.
 * The app must do the same.
 * Neither the app nor the framework can use the Serial bus across sleeps (a Serial session only while awake.)
 * This is not to say that the peripheral on the other end of the Serial bus cannot active,
 * in particular, the external RTC across the Serial bus IS active while sleeping,
 * but it's interaction comes in the form of an interrupt.
 */

class App {
public:
	/*
	 * Called when system is first powered on.
	 *
	 * app should initialize state.
	 * All RAM contents was lost and RAM variables set to initial values by resetHandler.
	 *
	 * Persistent state of application can be kept in FRAM.
	 * (FRAM is initialized at load time, not in resetHandler)
	 */
	static void onPowerOnReset();

	/*
	 * Called when system is waken from sleep.
	 *
	 * act using persistent state (in FRAM)
	 */
	static void onWakeForAlarm();

	/*
	 * Called from postlude, just before entering low power sleep in duty cycle.
	 * Result depends on state of app.
	 *
	 * Alternatives, depend on simple vs task scheduled.
	 * (ifdeffed in framework which one is used.)
	 */
	// Duty at constant intervals
	static Duration constantDurationOfSleep();

	// Duty at variable intervals, using task scheduler
	// OLD static EpochTime timeToWake();
	static Duration durationOfSleep();


	/*
	 * Configure GPIO that persists through sleep.
	 * Framework calls before GPIO unlocked.
	 */
	static void configureSleepingGPIO();

    /*
     * Configure unused and unconnected pins as low power.
     *
     * On most platforms, outputs.
     * On reset, default to inputs.
     * If left as inputs and unconnected, they float and draw excess power.
     *
     * Does not ensure determinate output values.
     */
    static void configureUnusedPinsLowPower();

    static void configureUsedPins();

    /*
     * Tell app that timeNow() is available.
     * Until after this call, app should not access timeNow()
     * In other words, this is a hook for the app called after EpochClock is available.
     */
    static void takeTimeNow();

};
