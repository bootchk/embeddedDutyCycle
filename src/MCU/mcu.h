
/*
 * Facade for controlling SoC components:
 * system core
 * bus faults
 * watchdog
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
};
