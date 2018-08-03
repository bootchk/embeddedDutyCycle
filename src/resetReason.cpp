
#include "resetReason.h"

#include <msp430.h>



/*
 * SYSRSTIV is a generator: reading clears one reason but hw generates new reasons immediately
 * Many reasons can exist.
 */
bool ResetReason::isResetAWakeFromSleep() {
  bool done = false;
  bool result = false;

  while (! done)
  {
    // not using even_in_range() which is just an optimization, and dissappeared from compiler?
    switch (SYSRSTIV)
    {
    case SYSRSTIV_NONE:
      done = true;  // stop loop - all reset reasons are cleared
      break;

    case SYSRSTIV_LPM5WU:
      result = true;
      break;

    /*
    Not germane to fr2433
    case SYSRSTIV_SVSL:
    case SYSRSTIV_SVSH:
    case SYSRSTIV_SVML_OVP:
    case SYSRSTIV_SVMH_OVP:
    case SYSRSTIV_KEYV:
    case SYSRSTIV_PMMKEY:
    */

    // Expected, normal
    case SYSRSTIV_BOR:  // power up
    case SYSRSTIV_RSTNMI:  // RST/NMI pin reset e.g. from debug prove
      //toggleGreenLED();
      break;


    // Faults, abnormal e.g. "bus error"
    // not exhaustive
    case SYSRSTIV_SECYV:
    case SYSRSTIV_DOBOR:
    case SYSRSTIV_DOPOR:
    case SYSRSTIV_WDTTO:
    case SYSRSTIV_WDTKEY:
    case SYSRSTIV_FLLUL:
    case SYSRSTIV_PERF:
    default:
      break;
    }
  }
  return result;
}
