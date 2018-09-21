
/*
 * For testing on msp-exp430fr2433 LaunchPad
 */

#include "testMain.h"

#include <msp430.h>

#include "../PMM/powerMgtModule.h"


/*
 * Various hacks: temporary for testing alternative implementations.
 */

bool TestMain::isResetAwakeFromSleep() {
    return (SYSRSTIV == SYSRSTIV_LPM5WU);
}


void TestMain::delayBriefly() {
    __delay_cycles(500);
    // 50000 works
    // 50 fails another way
    // 0 fails with NMI VMA
}



/*
 * These routines mock what a real app should do.
 */


void TestMain::initAllGpioOutLow()
{
    // direction: out
    P1DIR = 0xFF; P2DIR = 0xFF; P3DIR = 0xFF;
    // pull resistor: enabled (when input)
    P1REN = 0xFF; P2REN = 0xFF; P3REN = 0xFF;
    // value: low
    P1OUT = 0x00; P2OUT = 0x00; P3OUT = 0x00;

    // !!! GPIO config not effected until unlock LOCKLPM5
}


#ifdef OLD
void TestMain::blinkRedLED() {
    for (unsigned int i = 30; i > 0; i-- ) {
            P1OUT ^= BIT0;                      // P1.0 = toggle
            __delay_cycles(100000);
    }
}
#endif

/*
 * Blink on count number of times.
 * Leave off.
 */
void TestMain::blinkGreenLED(unsigned int count) {
    // start with off
    P1OUT &= ~BIT1;

    for (unsigned int i = count * 2; i > 0; i-- ) {
            P1OUT ^= BIT1;                      // P1.1 = toggle
            __delay_cycles(200000);
    }

    // delay extra to distinguish consecutive calls
    __delay_cycles(500000);
}

///#define TESTBLINKRED 1
#ifdef TESTBLINKRED
void TestMain::blinkRedLED(unsigned int count) {
    // start with off
    P1OUT &= ~BIT0;

    for (unsigned int i = count * 2; i > 0; i-- ) {
            P1OUT ^= BIT0;                      // P1.0 = toggle
            __delay_cycles(200000);
    }

    // delay extra to distinguish consecutive calls
    __delay_cycles(500000);
}
#else
// impotent
void TestMain::blinkRedLED(unsigned int count) {}
#endif


void TestMain::lightGreenLED() {
    // require direction OUT
    P1OUT |= BIT1;
}


void TestMain::configureButtonWakeupSource() {
    P2DIR &= ~(BIT3);                   // Configure P1.3 as input direction pin
    P2OUT |= BIT3;                      // Configure P1.3 as pulled-up
    P2REN |= BIT3;                      // P1.3 pull-up register enable
    P2IES |= BIT3;                      // P1.3 Hi/Low edge
    P2IFG = 0;                          // Clear all P1 interrupt flags
    P2IE |= BIT3;                       // P1.3 interrupt enabled
}


void TestMain::abortGreenLED() {
    ensureGreenLEDLightable();
    TestMain::lightGreenLED();
    // spin
    while(true) ;
}



void TestMain::blinkForcedGreenLED(unsigned int count) {
    ensureGreenLEDLightable();
    blinkGreenLED(count);
}





/*
 * Terminal routines: never return.  To indicate faults.
 */

void TestMain::warbleRedLEDForever() {
    ensureRedLEDLightable();

    // warble
    while(true) {
        P1OUT ^= BIT0;                      // toggle P1.0
        __delay_cycles(50000);
    }
}

void TestMain::warbleGreenLEDForever() {
    ensureGreenLEDLightable();

    // warble
    while(true) {
        P1OUT ^= BIT1;                      // toggle P1.1
        __delay_cycles(50000);
    }
}


void TestMain::ensureGreenLEDLightable() {
    // Ensure GPIO config will have effect
    PMM::unlockLPM5();

    // GPIO configured out
    P1DIR |= BIT1;
}

void TestMain::ensureRedLEDLightable() {
    // Ensure GPIO config will have effect
    PMM::unlockLPM5();

    // GPIO configured out
    P1DIR |= BIT0;
}







