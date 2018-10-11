
/*
 * Delay with cpu idle but RAM retained.
 *
 * AKA standby
 * AKA LPM3 (TI nomenclature)
 */

class LowPowerTimer {
public:
    static void delay(unsigned int);
};
