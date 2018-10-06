
#include <src/blinkerAppTasked/powerMgr.h>
#include "../peripheral/ADC/adc.h"

#include "parameters.h"




bool PowerMgr::isPowerForBlinking() {
    // Does not require GPIO configuration
    // Does not change GPIO configuration

    // Greater than 3 volts
    return ADC::measureVccCentiVolts() > Parameters::MinVccForBlinking ;
}

/* Vcc varies.
 * Min Vcc is 1.9V else would not be booted.
 * Max Vcc is 3.6V.
 * Vcc in range [1.9, 3.6]
 *
 * adcResult in range [0, 255 (0xFF)]
 *
 * Return if solar cell voltage is 1/2 (0x80, 128 base 10) of Vcc i.e. about 0.9V to 1.8V
 * Return if solar cell voltage is 1/3 (0x50, 80 base 10) of Vcc i.e. about 0.6V to 1.2V
 */
bool PowerMgr::isSolarCellDark() {
    // Does not require GPIO configuration
    // This configures a GPIO as side effect.

    // Less than 0.8 volts
    /// Wrong return ADC::measureSolarCellCentiVolts() < Parameters::MaxVccForDark  ;
    bool result = ADC::measureSolarCellProportionToVcc() < 80;
    return result;
}

