
#include "duty.h"
#include "testMain.h"

#include <msp430.h>

#include <cassert>


// TODO does this need to be C?


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
 * If alarm pin is P1.3, need ISR for Port 1
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



/*
 * Indicate serious faults
 */

#pragma vector = SYSNMI_VECTOR
__interrupt void SYSNMI_ISR(void)
{
    // Decode NMI source
    // Not enabled by JMBINIE and JMPOUTIE
    //if (SFRIFG1 & JMBOUTIFG) assert(false);
    //if (SFRIFG1 & JMBINIFG) assert(false);
    //if (SFRIFG1 & VMAIFG) assert(false);
    // Not enabled CBDIFG, UBDIFG
    unsigned int reason = SYSSNIV;
    // see data sheet under SYS module for values.  0x12 is VMA

    while (true) {
        TestMain::blinkGreenLED(10);
    }
}
