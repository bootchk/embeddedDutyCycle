
#include <src/blinkerAppTasked/powerMgr.h>
#include "../peripheral/ADC/adc.h"



// TODO parameters

bool PowerMgr::isPowerForBlinking() {
    // Greater than 3 volts
    return ADC::measureVccCentiVolts() > 300 ;
}


bool PowerMgr::isSolarCellDark() {
    // Less the 0.8 volts
    return ADC::measureSolarCellCentiVolts() < 80  ;
}

