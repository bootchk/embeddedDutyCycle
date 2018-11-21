
#include "powerMgr.h"

#include <ADC/adc.h>

#include "parameters.h"




bool PowerMgr::isPowerForBlinking() {
    // Does not require GPIO configuration
    // Does not change GPIO configuration

    // Greater than 3 volts
    return ADC::measureVccCentiVolts() > Parameters::MinVccForBlinking ;
}
