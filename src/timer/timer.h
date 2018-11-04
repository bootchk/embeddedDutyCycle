
/*
 * Delay with cpu idle but RAM retained.
 *
 * cpu idle:
 * AKA standby
 * AKA LPM3 (TI nomenclature)
 */

class LowPowerTimer {
public:

    static void delayTicksOf100uSec(unsigned int);
    static void delayFiveSeconds();
};
