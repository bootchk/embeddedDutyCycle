
#include "powerMgr.h"

#include "../peripheral/ADC/adc.h"




bool PowerMgr::isPowerForBlinking() {
    return ADC::isVccHigh();
}
