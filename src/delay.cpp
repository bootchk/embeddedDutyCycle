
#include "delay.h"

#include <timer/timer.h>



void delayForStartup()
{
    LowPowerTimer::delayHalfSecond();
}
