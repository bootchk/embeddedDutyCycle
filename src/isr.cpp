
#include "duty.h"

// TODO does this need to be C




/*
 * ISR for RTC alarm interrupt.
 *
 * If alarm pin is P1.3, need ISR for Port 1
 *
 * It is possible to eliminate this if you don't enable interrupt after sleep before unlocking.
 * Then the ISR is not called, even though interrupt occurred.
 *
 * Here, the ISR just clears interrupt flag, so no infinite interrupt loop.
 */
#if defined(__TI_COMPILER_VERSION__)
#pragma vector=PORT1_VECTOR
__interrupt void Port2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    // assert(false);  // TEMP
    Duty::clearAlarmOnMCU();
}
