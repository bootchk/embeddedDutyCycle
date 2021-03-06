
#pragma once


/*

 * This board.h is only for the test harness of msp430Drivers.
 * This is NOT in /src and hence not part of the library.
 * A real app would define board.h higher in the dependency tree.
 */

// Some right hand sides from gpio.h in DriverLib
#include <gpio.h>



/*
 * Describes hardware design i.e. connections between pins of mcu and external chips
 *
 * Local alias to definitions specific to:
 * - choice of family member (here MSP430FR2433)
 * - choice of instance of serial device
 * - choice of alarm pin
 */


/*
 * Address of chosen device instance on chosen family member.
 * MSP430FR2433 choices are A0, A1, B0
 * !!! if family member does not contain the choice of instance, you get compile errors.
 */

// Unused config: msp430fr2433 specific, using instance UCB0: 1.2, 1.3, 1.1




#define I2C_HAS_INTERNAL_PULLUPS

/*
 * SPI config
 */
#define SPIInstanceAddress      EUSCI_A1_BASE
#define I2CInstanceAddress      EUSCI_B0_BASE

// msp430fr2433 and Launchpad MSP-EXP430FR2433, using instance UCA1:
// MOSI 2.6
// MISO 2.5
// CLK 2.4
#define MOSI_PORT     GPIO_PORT_P2
#define MOSI_PIN      GPIO_PIN6

#define MISO_PORT     GPIO_PORT_P2
#define MISO_PIN      GPIO_PIN5

#define SPI_CLK_PORT  GPIO_PORT_P2
#define SPI_CLK_PIN   GPIO_PIN4




/*
 * Alarm pin and ChipSelect (SPI SlaveSelect)
 */

// P1.3
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN3

// P1.2
#define RTCSelectPort GPIO_PORT_P1
#define RTCSelectPin  GPIO_PIN2




/*
 * I2C config
 */

// msp430fr2433 and Launchpad MSP-EXP430FR2433, using instance UCB0:
// data P1.2
// clock P1.3
#define I2C_SDA_PORT  GPIO_PORT_P1
#define I2C_SDA_PIN   GPIO_PIN2

#define I2C_SCL_PORT  GPIO_PORT_P1
#define I2C_SCL_PIN   GPIO_PIN3







/*
 * LED configuration
 */

/*
 * Definitions for MSP-EXP430FR2433: two LED's
 *
 * !!! These are only for class LED, where LED is driven from a single side.
 * See also LEDAndLightSensor class
 */


// On PCB, LED is sunk
// TODO for prototype on launchpad, led is sourced
#define LED_SOURCED


// P1.0, red on launchpad
#define APP_LED1_PORT     GPIO_PORT_P1
#define APP_LED1_PIN      GPIO_PIN0

// P1.1 green
#define APP_LED2_PORT     GPIO_PORT_P1
#define APP_LED2_PIN      GPIO_PIN1



/*
 * LEDAndLightSensor configuration
 */
/*
 * For rev 1 board hack #1, LEDP 1.0, LEDN 1.4
 * For rev 1 board hack #2, LEDP 1.1, LEDN 1.0 (P1.1 was LEDpwm)
 */
/*
 * What pins are used.
 * Two GPIO pins, one for each side of LED.
 * Must not conflict with other uses.
 */
#define NSideLEDPort GPIO_PORT_P1
#define NSideLEDPin  GPIO_PIN0

// P1.1 is green led on launchpad

// P side high to light LED
#define PSideLEDPort GPIO_PORT_P1
#define PSideLEDPin  GPIO_PIN1



/*
 * A voltage which ensures a slow-rising power supply (solar)
 * has enough reserve to continue the boot code.
 * Used at coldstart time, to sleep until Vcc reaches this level.
 *
 * This depends on the board: the trigger voltage of the voltage monitor
 * and the size of the power supply storage.
 *
 * Units centivolts
 */
#define MinVccForStarting 190






/*
 * 7-bit address
 * 0011100 if SDO pin is grounded
 * 0x1e  0b0011110 if SDO pin is high
 * Pololu breakout board pullups SDO pin high by default.
 */
#define LIS3MDLAddress 0x1e




/*
 * Abracon says 7-bit address is 0b1101001 (0x69)
 * But they say "0xD2/3", which includes the R/W bit which is transmitted AFTER the seven bit address
 */
#define AB0805BusAddress 0x69

/*
 * Aliases
 */
#define RTCBusAddress AB0805BusAddress

