
/*
 * Demonstrates flaky Launchpad behaviour
 */

#include <msp430.h>
#include <cassert>

// Driverlib
#include <pmm.h>
#include <sysctl.h>
#include <gpio.h>
#include "eusci_a_spi.h"



#define SPIInstanceAddress      EUSCI_A0_BASE
// P1.4
#define MOSI_PORT     GPIO_PORT_P1
#define MOSI_PIN      GPIO_PIN4
// P1.5
#define MISO_PORT     GPIO_PORT_P1
#define MISO_PIN      GPIO_PIN5
// P1.6
#define SPI_CLK_PORT  GPIO_PORT_P1
#define SPI_CLK_PIN   GPIO_PIN6




void warbleGreenLEDForever() {
    // ensure GPIO will be effective
    PMM_unlockLPM5();

    P1DIR |= BIT1;  // Dir: out

    while(true) {
        P1OUT ^= BIT1;                      // P1.1 = toggle
        __delay_cycles(50000);
    }
}


void blinkRedLED(unsigned int count) {
    // require configure DIR==out

    // start with off
    P1OUT &= ~BIT0;

    for (unsigned int i = count * 2; i > 0; i-- ) {
            P1OUT ^= BIT0;                      // P1.0 = toggle
            __delay_cycles(200000);
    }

    // delay extra to distinguish consecutive calls
    __delay_cycles(500000);
}


// S1 P2.3
void configureButtonWakeupSource() {
    P2DIR &= ~(BIT3);                   // Configure P1.3 as input direction pin
    P2OUT |= BIT3;                      // Configure P1.3 as pulled-up
    P2REN |= BIT3;                      // P1.3 pull-up register enable
    P2IES |= BIT3;                      // P1.3 Hi/Low edge
    P2IFG = 0;                          // Clear all P1 interrupt flags
    P2IE |= BIT3;                       // P1.3 interrupt enabled
}


#pragma vector = SYSNMI_VECTOR
__interrupt void mySYSNMI_ISR(void)
{
  while (true) {
    // NMI source
    unsigned int reason = SYSSNIV;
    // see data sheet under SYS module for values.  0x12 is VMA

    switch(reason) {
    case SYSSNIV_NONE:         /* No Interrupt pending */
        break;

    case SYSSNIV_VMAIFG:       /* SYSSNIV : VMAIFG */
        warbleGreenLEDForever();


    case SYSSNIV_SVSLIFG:      /* SYSSNIV : SVS low-power reset entry */

    case SYSSNIV_UBDIFG:       /* SYSSNIV : FRAM Uncorrectable bit Error */
    case SYSSNIV_CBDIFG:      /* SYSSNIV : FRAM Correctable Bit error */
    case SYSSNIV_ACCTEIFG:     /* SYSSNIV : ACCTEIFG */

    case SYSSNIV_JMBINIFG :    /* SYSSNIV : JMBINIFG */
    case SYSSNIV_JMBOUTIFG:    /* SYSSNIV : JMBOUTIFG */
    default:
        // while (true) TestMain::blinkGreenLED(2);
        assert(false);
    }
  }
}


void configureGPIO() {
    // Configure all pins out except for a set to be used by app
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);
    //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);
    //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

    // Only 5 pins on port 3
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
    //GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);

    // Test pins (leds) low
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
}



void configureSPI() {
    EUSCI_A_SPI_initMasterParam param = {0};

        // Other API's just setClockDivider()
        param.selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK;
        param.clockSourceFrequency = 8000000;
        param.desiredSpiClock = 1000000;

        // setBitOrder()
        param.msbFirst = EUSCI_A_SPI_MSB_FIRST;

        // SPI clock mode
        // SPI_MODE == 0
        // setDataMode()
        param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
        param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;

        /*
         * TI's SPI mode
         * 3-pin mode (SCLK, SMOSI, and SMISO) w separate SS not part of eUSCI module
         */
        param.spiMode = EUSCI_A_SPI_3PIN;

        EUSCI_A_SPI_initMaster(SPIInstanceAddress, &param);
}

void configureSPIPins() {
    GPIO_setAsPeripheralModuleFunctionOutputPin(MOSI_PORT, MOSI_PIN,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_CLK_PORT, SPI_CLK_PIN,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
    // One input pin
    GPIO_setAsPeripheralModuleFunctionInputPin(MISO_PORT, MISO_PIN,
                                               GPIO_PRIMARY_MODULE_FUNCTION);
}

void unconfigureSPIPins() {
    GPIO_setAsOutputPin(MOSI_PORT,    MOSI_PIN);
    GPIO_setAsOutputPin(SPI_CLK_PORT, SPI_CLK_PIN);
    GPIO_setAsOutputPin(MISO_PORT,    MISO_PIN);
}



unsigned char transfer(unsigned char value) {

    /*
     * Requires any previous transfer complete.
     * we wait afterwards, but for safety also check before.
     * Maybe configuration takes some time before SPI module is not busy?
     */
    while(EUSCI_A_SPI_isBusy(SPIInstanceAddress)) ;

    EUSCI_A_SPI_transmitData(SPIInstanceAddress, value);

    /*
     * Spin until transfer is complete.
     * SPI bus is slower than CPU.
     * Wait until result (of duplex communication).
     *
     * Finite.  If infinite duration, hw has failed.
     *
     * Other implementations use an interrupt flag???
     */
    while(EUSCI_A_SPI_isBusy(SPIInstanceAddress)) ;

    return EUSCI_A_SPI_receiveData(SPIInstanceAddress);
}






bool isResetAwakeFromSleep() {
  bool done = false;
  bool result = false;

  while (! done)
  {
    // not using even_in_range() which is just an optimization, and dissappeared from compiler?
    unsigned int resetReason = SYSRSTIV;
    switch (resetReason)
    {
    case SYSRSTIV_NONE:
        /// Won't work, LPM5 locked blinkGreenLED(1);
      done = true;  // stop loop - all reset reasons are cleared
      break;

    // Expected
    case SYSRSTIV_LPM5WU:
      /// Won't work, LPM5 locked blinkGreenLED(2);
      result = true;
      break;

    // Expected
    case SYSRSTIV_BOR:     // power up
    case SYSRSTIV_RSTNMI:  // RST/NMI pin reset e.g. from debug prove
        /// Won't work, LPM5 locked blinkGreenLED(3);
      break;

    // Security. Accessing BSL that is protected. Probably errant
    case SYSRSTIV_SECYV:     // Security violation
       //assert(false);
        /// Won't work, LPM5 locked blinkGreenLED(4);
       break;

     // WDT Time out
     case SYSRSTIV_WDTTO:

    // Software initiated
    case SYSRSTIV_DOBOR:
    case SYSRSTIV_DOPOR:

    // Faults, abnormal e.g. "bus error"
    case SYSRSTIV_UBDIFG:    // FRAM Uncorrectable bit Error

    // Keys
    case SYSRSTIV_WDTKEY:    // WDT Key violation
    case SYSRSTIV_FRCTLPW:   // FRAM Key violation

    case SYSRSTIV_PERF:      // peripheral/config area fetch
    case SYSRSTIV_PMMPW:     // PMM Password violation
    case SYSRSTIV_FLLUL:     // FLL unlock

    default:
        /// Won't work, LPM5 locked blinkGreenLED(5);
      assert(false);
      break;
    }
  }
  return result;
}


/*
 * Use SPI to transfer.
 * Doesn't need to be an actual slave.
 */
void futzWithSPI() {
    // configure requires disabled
    configureSPI();
    configureSPIPins();
    EUSCI_A_SPI_enable(SPIInstanceAddress);
    while (EUSCI_A_SPI_isBusy(SPIInstanceAddress))
        ;
    transfer(0xbe);
    EUSCI_A_SPI_disable(SPIInstanceAddress);
    unconfigureSPIPins();
}



int main777(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // assert GPIO in reset state

    PMM_unlockLPM5();


    /*
     * Enable NMI for VMA
     */


    // Clear NMI flag so no immediate interrupt if flag already set
    SFRIFG1 &= ~NMIIFG;

    // vacant memory generate NMI
    SFRIE1 |= VMAIE;

    // BSL memory behave as vacant memory
    SYSBSLC |= SYSBSLOFF;


    // unprotect FRAM
    SysCtl_enableFRAMWrite(SYSCTL_FRAMWRITEPROTECTION_DATA | SYSCTL_FRAMWRITEPROTECTION_PROGRAM);


    configureGPIO();
    // configuration might not be effective when LMP5 is locked


    if (isResetAwakeFromSleep())
    {
        PMM_unlockLPM5();
        blinkRedLED(5);
    }
    else
    {
        PMM_unlockLPM5();
        // Device powered up from a cold start or other reset reason
        blinkRedLED(2);
    }

    configureButtonWakeupSource();

    futzWithSPI();

    // sleep forever (no interrupt sources)

    PMMCTL0_H = PMMPW_H;                // Unlock PMM Registers
    PMMCTL0_L &= ~(SVSHE);              // Disable SVS high side
    PMMCTL0_L |= PMMREGOFF;             // Core regulator off
    //PMMCTL0_H = 0;                    // Lock PMM Registers

    /*
     * Enter LPM4 or LPM4.5
     * If LPM4.5, this does not return.
     * LPM4.5 sleep will exit via a RESET event, resulting in re-start of code.
     */
    __bis_SR_register(LPM4_bits | GIE);
    __no_operation();
    assert(false);
}
