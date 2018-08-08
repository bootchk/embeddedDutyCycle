
/*
 * For testing on msp-exp430fr2433 LaunchPad
 */

#include "testMain.h"

#include <msp430.h>


void TestMain::initGpio()
{
    P1DIR = 0xFF; P2DIR = 0xFF; P3DIR = 0xFF;
    P1REN = 0xFF; P2REN = 0xFF; P3REN = 0xFF;
    P1OUT = 0x00; P2OUT = 0x00; P3OUT = 0x00;

    // !!! GPIO config not effected until unlock LOCKLPM5
}


void TestMain::blinkLED() {
    for (unsigned int i = 30; i > 0; i-- ) {
            P1OUT ^= BIT0;                      // P1.0 = toggle
            __delay_cycles(100000);
    }
}


void TestMain::configureWakeupSource() {
    P2DIR &= ~(BIT3);                   // Configure P1.3 as input direction pin
    P2OUT |= BIT3;                      // Configure P1.3 as pulled-up
    P2REN |= BIT3;                      // P1.3 pull-up register enable
    P2IES |= BIT3;                      // P1.3 Hi/Low edge
    P2IFG = 0;                          // Clear all P1 interrupt flags
    P2IE |= BIT3;                       // P1.3 interrupt enabled
}
