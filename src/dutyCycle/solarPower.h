
/*
 * Knows how to boot with a solar power supply
 * with a voltage monitor with small hysteresis.
 * Vcc rises slowly.
 */

class SolarPower {
public:
    /*
     * When cold starting after Vcc just crossed Vmin threshold
     * sleep until Vcc is higher
     * indicating a reserve that will sustain more processing (the initial burst.)
     * Until now, a bare minimum of processing was done to boot the system.
     *
     * May not return for a long time if ever.
     * Solar power may not be enough to reach the reserve
     * or sustain Vcc above system Vmin through this routine.
     */
    static void sleepUntilPowerReserve();

    /*
     * Is Vcc a few tenths of a volt about Vmin?
     */
    static bool isPowerForStarting();
};
