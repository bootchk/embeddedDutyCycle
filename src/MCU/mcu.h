
/*
 * Facade for controlling SoC components:
 * system core
 * bus faults
 * watchdog
 *
 * TODO misnamed.
 * These deal with the SoC, not just the mcu.
 * The watchdog, PMM, etc are not part of the mcu.
 */

class MCU {
public:
    static void stopWatchDog();

	static void enterLPM4orLPM4_5();

	static void enableGlobalInterrupts();

	/*
	 * Make BSL behave like vacant memory
	 * and enable vacant memory non maskable interrupt
	 */
	static void enableBSLOffAndVacantMemoryNMI();


	/*
	 * Disable write protect for all FRAM.
	 */
	static void disableFRAMWriteProtect();

	// TODO belongs elsewhere
	static void configureUnusedPinsLowPower();
};
