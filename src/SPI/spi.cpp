
#include "spi.h"

/*
 * Derived from
 * Maksim Gorev spi_driverlib.c http://www.breakmyboard.com/blog/ti-mcu-ti-driverlib-based-simple-spi-library/
 * https://github.com/machinaut/msp432-driverlib.git/spi.c
 * MSP430F5529_driverlib_examples/13A_USCI_B_SPI_MCP41010_digiPot
 * TI-RTOS for SimpleLink Wireless MCUs ... SPI.h
 *
 */
/*
 * Much specialization here, see TI docs
 * Serial
 *    USCI	(older device)
 *    eUSCI (newer device)
 *       eUSCI_A variant support UART, iRDA, SPI
 *          one or more instances
 *       eUSCI_B variant support I2C or SPI   <====== this
 *          one or more instances per family member
 */
/*
 * Too change to another instance substitute in many places.
 */

/*
 * Specialize for one family member.
 * Must precede eusci_b_spi.h
 */
// For compiling in some IDE's where these are not defined earlier
#ifndef __MSP430__
#define __MSP430__
#define __MSP430FR2033__
// #define MSPEXP430FR2433LP
#endif



// TI DriverLib
#include "eusci_a_spi.h"
//#include "eusci_b_spi.h"

#include "gpio.h"




/*
 * Local alias to definitions specific to:
 * - choice of family member (here MSP430FR2433)
 * - choice of instance of serial device (here UCA1)
 * Right hand sides from gpio.h
 */

/*
 * Address of chosen device instance on chosen family member.
 * MSP430FR2433 choices are A0, A1, B0
 * !!! if family member does not contain the choice of instance, you get compile errors.
 */
#define SPIInstanceAddress		EUSCI_A1_BASE

// msp430fr2433 specific, using instance UCB0: 1.2, 1.3, 1.1
// msp430fr2433 specific, using instance UCA1: 2.6, 2.5, 2.4

// P2.6
#define MOSI_PORT     GPIO_PORT_P2
#define MOSI_PIN      GPIO_PIN6
// P2.5
#define MISO_PORT     GPIO_PORT_P2
#define MISO_PIN      GPIO_PIN5
// P2.4
#define SPI_CLK_PORT  GPIO_PORT_P2
#define SPI_CLK_PIN   GPIO_PIN4





void SPI::enable() {
	EUSCI_A_SPI_enable(SPIInstanceAddress);
}


void SPI::disable() {
	EUSCI_A_SPI_disable(SPIInstanceAddress);
}


/*
 * A transfer is the master clocking the bus.
 * The slave may put data on MISO
 * and master put data on MOSI on each clock cycle.
 * I.E. duplex communication.
 */
unsigned char SPI::transfer(unsigned char value) {
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


void SPI::configureMaster() {
	configureMasterDevice();
	configureMasterPins();
}

void SPI::unconfigureMaster() {
	// Leave device configured, only unconfigure pins
	unconfigureMasterPins();
}


void SPI::configureMasterPins() {
	GPIO_setAsPeripheralModuleFunctionOutputPin(MOSI_PORT,   MOSI_PIN,     GPIO_SECONDARY_MODULE_FUNCTION);
	GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_CLK_PORT, SPI_CLK_PIN, GPIO_SECONDARY_MODULE_FUNCTION);
	// One input pin
	GPIO_setAsPeripheralModuleFunctionInputPin(MISO_PORT,     MISO_PIN,    GPIO_SECONDARY_MODULE_FUNCTION);
}

void SPI::unconfigureMasterPins() {
	/*
	 * All pins output, which is low power and avoids all possibility of floating inputs.
	 */
	GPIO_setAsOutputPin(MOSI_PORT,    MOSI_PIN);
	GPIO_setAsOutputPin(SPI_CLK_PORT, SPI_CLK_PIN);
	GPIO_setAsOutputPin(MISO_PORT,    MISO_PIN);
}


/*
 * Configure device.
 *
 * SPI bus has many variations.
 * Remote device usually hardwires one variation.
 * Configuration of master's SPI device must match that of remote device.
 *
 * Other config: clock source not dictated by SPI standard
 */
void SPI::configureMasterDevice() {
	// require disabled

	EUSCI_A_SPI_initMasterParam param = {0};

	// Other API's just setClockDivider()
	param.selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK;
	param.clockSourceFrequency = 8000000;
	param.desiredSpiClock = 1000000;

	// setBitOrder()
	param.msbFirst = EUSCI_A_SPI_MSB_FIRST;

    // SPI_MODE == 0
	// setDataMode()
	param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
	param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;

	EUSCI_A_SPI_initMaster(SPIInstanceAddress, &param);
}
