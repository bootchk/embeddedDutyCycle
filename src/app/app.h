
/*
 * API between DutyCycle framework and an app that does useful stuff.
 * Called from main at apropos events of duty cycle.
 * An alternative is to register callbacks with the duty cycling framework?
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
	 * About to low power sleep in duty cycle.
	 * Result depends on state of app.
	 */
	static unsigned int durationOfSleep();

	/*
	 * Configure GPIO that persists through sleep.
	 */
	static void configureSleepingGPIO();
};
