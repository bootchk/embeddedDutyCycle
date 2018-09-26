#include "fatal.h"

#include <msp430.h>

#include "../PMM/powerMgtModule.h"  // unlock LPM4.5

/*
 * Terminal routines: never return.
 * To indicate faults.
 * In testing, asserts and NMI ISR's call these.
 * In production, asserts and NMI ISR's
 */

void Fatal::warbleRedLEDForever() {
    ensureRedLEDLightable();

    // warble
    while(true) {
        P1OUT ^= BIT0;                      // toggle P1.0
        __delay_cycles(50000);
    }
}


void Fatal::warbleGreenLEDForever() {
    ensureGreenLEDLightable();

    // warble
    while(true) {
        P1OUT ^= BIT1;                      // toggle P1.1
        __delay_cycles(50000);
    }
}


void Fatal::abortGreenLED() {
    ensureGreenLEDLightable();

    P1OUT != BIT1;      // P1.1 is high, LED is sourced
    ///TestMain::lightGreenLED();
    // spin
    while(true) ;
}


void Fatal::ensureGreenLEDLightable() {
    // Ensure GPIO config will have effect
    PMM::unlockLPM5();

    // GPIO configured out
    P1DIR |= BIT1;
}

void Fatal::ensureRedLEDLightable() {
    // Ensure GPIO config will have effect
    PMM::unlockLPM5();

    // GPIO configured out
    P1DIR |= BIT0;
}
