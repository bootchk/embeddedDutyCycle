
/*
 * Understands how to sleep mcu.
 *
 * For a design where:
 * - GPIO is locked during sleep but responsive to waking interrupts
 * - wake from sleep is a reset
 *
 * Implementation is for TI's LPM4.5 on MSP430
 *
 * Hides that implementation is two different modules:  cpu and pmm.
 */

class MCUSleep {
public:

	/*
	 * Sleep mcu.
	 * Locks state of GPIO.
	 * Only wake will be a reset of some sort, usually from an interrupt.
	 */
	static void enterLowestPowerSleep();


	/*
	 * Not low power.
	 * For testing.
	 */
	static void spinWithInterruptsEnabled();

	/*
	 * Unlocks GPIO state and makes effective any prior configuration.
	 * GPIO can be configured prior, but said configuration is not effective until this call.
	 *
	 * Must follow isResetAWakeFromSleep
	 * Once unlocked, you can't tell whether reset was from sleep.
	 */
	static void unlockMCUFromSleep();

	/*
	 * Called when mcu has been reset.
	 * True if sleeping before this reset.
	 * False if not sleeping, and therefore a power on or software reset or other reset.
	 */
	static bool isResetAWakeFromSleep();
	static void clearIsResetAWakeFromSleep();

	/*
	 * Called on reset.
	 * Reset configures pins that might not be low power ( e.g. MSP430: input, possibly floating.)
	 * Ensure they are low power.
	 * Typically, this configuration is overridden subsequently.
	 */
	static void configureAllPinsLowPower();
};
