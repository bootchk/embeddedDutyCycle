
/*
 * Facade to Power Management Module
 *
 * Here mostly about low power mode.
 * See User's Guide: 1.4.3 "Low-Power Modes...LPMx.5"
 */
class PMM {
public:
	/*
	 * Watchdog is enabled on reset.
	 * Should be separate module.
	 */
	static void stopWatchdog();

	/*
	 * Configure PMM for least power in sleep.
	 */
	static void configureOff();

	/*
	 * BOR is a named event in mcu state diagram.
	 */
	static void triggerSoftwareBORReset();

	/*
	 * LPM5 is a state of mcu.
	 * It actually is two states: sleepingLPM5 and awakeLPM5.
	 * sleepingLPM5 is cpu off but GPIO pins locked.
	 * mcu transitions to awakeLPM5 on interrupt, but GPIO pins are still locked.
	 * This takes the mcu from awakeLPM5 to operational state, with GPIO pins locked.
	 */
	static void unlockLPM5();

	/*
	 * A flag set by HW, checkable, and must be cleared.
	 */
	static bool isResetAWakeFromSleep();
	static void clearIsResetAWakeFromSleep();
	/*
	 * Clear all flags for any reset reasons.
	 */
	static void clearAllResetInterruptFlags();
};
