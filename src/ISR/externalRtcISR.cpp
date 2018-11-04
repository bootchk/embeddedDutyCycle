
#include <msp430.h>

#include "../duty.h"

#include "../board.h"  // board config



// Apparently this is C++ compatible

#ifdef NOT_USED
// Not used when warnings are errors
#if defined(__TI_COMPILER_VERSION__)
  // #warning "TI compiler"
  // unknown preprocessor directive: #message "TI compiler"
  // unrecognized pragma: #pragma message "foo"
#elif defined(__GNUC__)
  #warning "GNUC compiler"
#else
  #error Compiler not supported!
#endif
#endif


// For GNUC:  void __attribute__ ((interrupt(PORT2_VECTOR))) Port2_ISR (void)



/*
 * ISR for external RTC alarm interrupt.
 *
 * If alarm pin is Px, need ISR for Port x
 *
 * It is possible to eliminate this if you don't enable interrupt after sleep before unlocking.
 * Then the ISR is not called, even though interrupt occurred.
 *
 * Here, the ISR just clears interrupt flag, so no infinite interrupt loop.
 */

#if  GPIO_PORT_P2 == AlarmSignalPort
#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    Duty::clearAlarmOnMCU();
}
#else
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    Duty::clearAlarmOnMCU();
}
#endif






