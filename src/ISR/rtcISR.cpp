
#include <msp430.h>

#include <rtc.h>




#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    // Alternatively, read RTCIV to clear all flags

    // We expect overflow
    RTC_clearInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT_FLAG);

    /*
     * The flag is cleared automatically.
     * We don't do anything here except exit low power mode.
     * The continuation is after the call to _low_power_mode_x();
     */
    _low_power_mode_off_on_exit();
}
