/*
 * Knows how to configure MCU, clocks, etc.
 * Excludes common peripherals.
 */

class CentralSystem {
public:
    /*
     * After any reset (POR), but it can be cold restart or wake from LPMx.5.
     */
    static void configureAfterReset();
};
