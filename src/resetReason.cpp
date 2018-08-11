
#include "resetReason.h"

#include <msp430.h>

#include "testMain.h"

#include <cassert>


/*
 * SYSRSTIV is a generator: reading clears one reason but hw generates new reasons immediately
 * Many reasons can exist.
 *

MSP430FR2433

SYSRSTIV_NONE          (0x0000)        No Interrupt pending
SYSRSTIV_BOR           (0x0002)        SYSRSTIV : BOR
SYSRSTIV_RSTNMI        (0x0004)        SYSRSTIV : RST/NMI
SYSRSTIV_DOBOR         (0x0006)        SYSRSTIV : Do BOR
SYSRSTIV_LPM5WU        (0x0008)        SYSRSTIV : Port LPM5 Wake Up
SYSRSTIV_SECYV         (0x000A)        SYSRSTIV : Security violation
SYSRSTIV_RES12         (0x000C)        SYSRSTIV : Reserved
SYSRSTIV_SVSHIFG       (0x000E)        SYSRSTIV : SVSHIFG
SYSRSTIV_RES16         (0x0010)        SYSRSTIV : Reserved
SYSRSTIV_RES18         (0x0012)        SYSRSTIV : Reserved
SYSRSTIV_DOPOR         (0x0014)        SYSRSTIV : Do POR
SYSRSTIV_WDTTO         (0x0016)        SYSRSTIV : WDT Time out
SYSRSTIV_WDTPW         (0x0018)        SYSRSTIV : WDT Key violation
SYSRSTIV_WDTKEY        (0x0018)        SYSRSTIV : WDT Key violation
SYSRSTIV_FRCTLPW       (0x001A)        SYSRSTIV : FRAM Key violation
SYSRSTIV_UBDIFG        (0x001C)        SYSRSTIV : FRAM Uncorrectable bit Error
SYSRSTIV_PERF          (0x001E)        SYSRSTIV : peripheral/config area fetch
SYSRSTIV_PMMPW         (0x0020)        SYSRSTIV : PMM Password violation
SYSRSTIV_FLLUL         (0x0024)        SYSRSTIV : FLL unlock
 */

bool ResetReason::isResetAWakeFromSleep() {
  bool done = false;
  bool result = false;

  while (! done)
  {
    // not using even_in_range() which is just an optimization, and dissappeared from compiler?
    unsigned int resetReason = SYSRSTIV;
    switch (resetReason)
    {
    case SYSRSTIV_NONE:
      done = true;  // stop loop - all reset reasons are cleared
      break;

    case SYSRSTIV_LPM5WU:
      result = true;
      break;

    // Expected, normal
    case SYSRSTIV_BOR:     // power up
    case SYSRSTIV_RSTNMI:  // RST/NMI pin reset e.g. from debug prove
      //toggleGreenLED();
      break;

     // WDT Time out
     case SYSRSTIV_WDTTO:
         assert(false);
         break;

    // Software initiated
    case SYSRSTIV_DOBOR:
    case SYSRSTIV_DOPOR:
        assert(false);
        break;



    // Faults, abnormal e.g. "bus error"
    case SYSRSTIV_UBDIFG:    // FRAM Uncorrectable bit Error
        assert(false);
        break;

    // Security
        /*
         * Accessing BSL that is protected.
         * Probably errant
         */
    case SYSRSTIV_SECYV:     // Security violation
        //assert(false);
        TestMain::lightGreenLED();
        break;

    // Keys
    case SYSRSTIV_WDTKEY:    // WDT Key violation
    case SYSRSTIV_FRCTLPW:   // FRAM Key violation
        assert(false);
        break;
    case SYSRSTIV_PERF:      // peripheral/config area fetch
    case SYSRSTIV_PMMPW:     // PMM Password violation
    case SYSRSTIV_FLLUL:     // FLL unlock
      assert(false);
      break;

    default:
      assert(false);
      break;
    }
  }
  return result;
}