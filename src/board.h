
#pragma once

/*
 * Depends on hardware design i.e. connections between pins of mcu and rtc chips
 *
 * Local alias to definitions specific to:
 * - choice of family member (here MSP430FR2433)
 * - choice of instance of serial device
 * - choice of alarm pin
 *
 * Right hand sides from gpio.h
 */

/*
 * Address of chosen device instance on chosen family member.
 * MSP430FR2433 choices are A0, A1, B0
 * !!! if family member does not contain the choice of instance, you get compile errors.
 */

// msp430fr2433 specific, using instance UCB0: 1.2, 1.3, 1.1

#ifdef USE_USCIA1

#define SPIInstanceAddress      EUSCI_A1_BASE

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

#else

#define SPIInstanceAddress      EUSCI_A0_BASE

// msp430fr2433 specific, using instance UCA0

// P1.4
#define MOSI_PORT     GPIO_PORT_P1
#define MOSI_PIN      GPIO_PIN4
// P1.5
#define MISO_PORT     GPIO_PORT_P1
#define MISO_PIN      GPIO_PIN5
// P1.6
#define SPI_CLK_PORT  GPIO_PORT_P1
#define SPI_CLK_PIN   GPIO_PIN6

#endif



/*
 * Alarm pin and ChipSelect (SlaveSelect)
 */

#ifdef OLD

// P1.3
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN3

// P1.2
#define RTCSelectPort GPIO_PORT_P1
#define RTCSelectPin  GPIO_PIN2

#else

// P2.7
#define AlarmSignalPort GPIO_PORT_P2
#define AlarmSignalPin  GPIO_PIN7

// P1.7 UCA0STE doesn't work?
// P2.0 (must not conflict with Xin)
// p3.2
#define RTCSelectPort GPIO_PORT_P3
#define RTCSelectPin  GPIO_PIN2

#endif



#define SolarCellVoltagePort GPIO_PORT_P1
#define SolarCellVoltagePin  GPIO_PIN4
#define SolarCellVoltagePinFunction GPIO_PRIMARY_MODULE_FUNCTION
