/*
 * Derived from TI example code same name as file.
 *
 * Altered so that it blinks when button pushed.
 * Sleeps LMP4.5 otherwise.
 */


#include <msp430.h>

#include <assert.h>

// msp430Drivers
#include <SoC/SoC.h>



void configureAllGpioOut()
{
    P1DIR = 0xFF; P2DIR = 0xFF; P3DIR = 0xFF;
    P1REN = 0xFF; P2REN = 0xFF; P3REN = 0xFF;

    /*
     * !!! PxOUT is undefined now (after a reset) although the shadow value on the pin is defined.
     * !!! The PxOUT register does not now reflect the shadow value.
     *
     * If we want the shadow value (what we set before sleeping) we must set that now.
     */
    // Just set all low
    P1OUT = 0x00;
    P2OUT = 0x00; P3OUT = 0x00;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}


void toggleLED() {
    // P1.0 toggle.  XOR bit 0 with 0b1.
    P1OUT ^= BIT0;
}

void blinkLEDOnce() {
    // is low
    toggleLED();
    __delay_cycles(100000);
    toggleLED();
    // is low
}

void blinkLEDForever() {
    while (1)
        {
             toggleLED();
            __delay_cycles(100000);
        }
}



void trap() {
            // After LPM4.5 and button push, loop here until debugger reattached
            int trapped = 1;
            while(trapped);
}


void configureButtonInterrupt() {
            // Configure button P2.3 as wakeup source
            P2DIR &= ~(BIT3);                   // input direction
            P2OUT |= BIT3;                      // pulled-up
            P2REN |= BIT3;                      // pull-up register enable
            P2IES |= BIT3;                      // Hi/Low edge
            P2IFG = 0;                          // Clear all interrupt flags
            P2IE |= BIT3;                       // interrupt enabled
}


// Does not return.  Continues as a reset.
void sleepLPM4_5() {
           // Explicitly clear RTC control registers to disable it
           // just incase if the RTC was previously enabled
           RTCCTL = 0;

           PMMCTL0_H = PMMPW_H;                // Open PMM Registers for write
           PMMCTL0_L |= PMMREGOFF;             // and set PMMREGOFF
           //PMMCTL0_H = 0;                      // Lock PMM Registers

           // Enter LPM4 Note that this operation does not return. The LPM4.5
           // will exit through a RESET event, resulting in a re-start
           // of the code.
           __bis_SR_register(LPM4_bits | GIE);
           __no_operation();
           assert(0);
}



void initXtal() {
    // Initialize XT1 32kHz crystal
#ifdef NOT_USED
        P2SEL0 |= BIT0 | BIT1;              // set XT1 pin as second function
        do
        {
            CSCTL7 &= ~(XT1OFFG | DCOFFG);  // Clear XT1 and DCO fault flag
            SFRIFG1 &= ~OFIFG;
        } while (SFRIFG1 & OFIFG);          // Test oscillator fault flag
#endif
}






class B {
private:
    int value;
public:
    void init() {
        value = 0;
        assert(value == 0);
    }
    void increment() {value++;}
    int get() {return value;}
};

class A {
private:
     B b;
public:
     // just delegate to owned instance of B
     void init() {b.init(); }
     void increment() {b.increment();}
     int get() {return b.get();}
};


// Not initialized, and linker.cmd puts TI.noinit segment in FRAM
#pragma NOINIT
A a;
#pragma NOINIT
A another;

#pragma PERSISTENT
static int myCounter = 0;




int mainfoo(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // When testing persistence
    SoC::disableFRAMWriteProtect();

    configureAllGpioOut();
    // This is not the same configuration as when we went to sleep
    // Namely, button is not configured.
    // Thus we did wake from a button interrupt from LPM45, but no ISR will be called.

    // Determine whether we are coming out of an LPMx.5 or a regular RESET.
    if (SYSRSTIV == SYSRSTIV_LPM5WU)        // MSP430 just woke up from LPMx.5
    {
        // blinkLEDForever();
        // trap();
        // toggleLED();
        blinkLEDOnce();
        configureButtonInterrupt();

        // compare class instance and counter, both should be persistent, same value
        if (a.get() != myCounter) blinkLEDForever();

        a.increment();
        myCounter++;

        sleepLPM4_5();
    }
    else
    {
        // Device powered up from a cold start.
        configureButtonInterrupt();

        // myCounter was set to zero at load time
        assert(myCounter == 0);

        // We only init a on coldstart
        a.init();
        assert(a.get() == 0);


        sleepLPM4_5();
    }

    return 0;
}





