
#include <msp430.h>

#include "../dutyCycle/duty.h"

// msp430Drivers
#include <board.h>  // board config

#include <assert/myAssert.h>




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


/*
 * This implementation is somewhat fragile, it only covers 3 GPIO ports
 *
 * !!! Also, it does not share the interrupt vector for other interrupts on the same port.
 */



#if  GPIO_PORT_P1 == AlarmSignalPort

#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    /*
         * This ISR never executes since we don't enable the interrupt after waking from LPM4.5.
         * The interrupt generator does wake from LPM4.5 and the IFG is set,
         * but the ISR is not called.
         */
    Duty::clearAlarmOnMCU();
}
#elif GPIO_PORT_P2 == AlarmSignalPort

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{

    myAssert(false);

    Duty::clearAlarmOnMCU();
}

#elif GPIO_PORT_P3 == AlarmSignalPort

#ifndef PORT3_VECTOR
// !!! Some family members have no interrupts on port 3
#error PORT3_VECTOR not defined!
#endif

#pragma vector = PORT3_VECTOR
__interrupt void Port3_ISR(void)
{
    Duty::clearAlarmOnMCU();
}

#else

#error AlarmSignalPort not match any GPIO Port!

#endif






