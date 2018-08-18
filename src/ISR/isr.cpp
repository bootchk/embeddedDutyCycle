
#include <msp430.h>

#include "../duty.h"

#include <cassert>


// Apparently this is C++ compatible


#if defined(__TI_COMPILER_VERSION__)
  #warning "TI compiler"
#elif defined(__GNUC__)
  #warning "GNUC compiler"
#else
  #error Compiler not supported!
#endif


// For GNUC:  void __attribute__ ((interrupt(PORT2_VECTOR))) Port2_ISR (void)



/*
 * ISR for RTC alarm interrupt.
 *
 * If alarm pin is Px, need ISR for Port x
 *
 * It is possible to eliminate this if you don't enable interrupt after sleep before unlocking.
 * Then the ISR is not called, even though interrupt occurred.
 *
 * Here, the ISR just clears interrupt flag, so no infinite interrupt loop.
 */

#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    // assert(false);  // TEMP
    Duty::clearAlarmOnMCU();
}






