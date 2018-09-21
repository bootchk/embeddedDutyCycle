
#include "../alarm/alarmTypes.h"    // Duration
#include "../RTC/timeTypes.h"   // EpochTime

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
 */

class App {
public:
	/*
	 * app should initialize state (FRAM is initialized at load time, not in resetHandler)
	 */
	static void onPowerOnReset();

	/*
	 * act using persistent state (in FRAM)
	 */
	static void onWakeForAlarm();




	/*
	 * Called from postlude.
	 * About to low power sleep in duty cycle.
	 * Result depends on state of app.
	 */
	/*
	 * Alternatives, depend on simple vs task scheduled
	 */
	// Duration we should sleep
	static Duration durationOfSleep();

	// Time we should wake.
	static EpochTime timeToWake();


	// Configure GPIO that persists through sleep
	static void configureSleepingGPIO();
};
