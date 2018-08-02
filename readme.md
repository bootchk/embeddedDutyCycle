
embeddedDutyCycle
-

A framework for duty cycling an embedded computer.

Duty cycle: a verb meaning sleep mostly, and wake (go on duty) rarely.  Duty cycling saves energy e.g. battery life.

For an embedded computer (mcu), abstracts an external RTC and GPIO pins for an interrupt from it.


---Work in Progress---



Motivation
-

I am using:

     - MSP430FR2433 mcu
     - AB0815 rtc
     - SPI bus between them
     
 The mcu has an internal RTC but it takes much more power than the external RTC.
 The external rtc wakes up the mcu from deep sleep (16nA) by raising an interrupt signal on a GPIO pin.
 The rtc takes only about 20nA.
 Thus the system draws only about 40nA except when alarm goes off.
 
 Both the chips are extremely low power due to unique technology. 
 Relevant search terms about the technology:
 
     - Abracon RTC subthreshold
     - TI MSP430 FRAM and LPM4.5
     
The combination duty-cycles the mcu.  Average current drawn by the system depends on the duty cycle ( the proportion of sleeping to active time.)

The duty cycle can be very large.  For example, the system theoretically could wake up for one second every hundred years, powered by a coin cell battery.

Dev env
-

The project is a Code Composer Studio CCS (TI's variant of Eclipse) IDE project.

The repository needs but does not contain a driverlib/ directory, which you can download from TI's MSPWare.

The project derives from an example project for DriverLib.


Low level time libraries
-

"Calendar time" is broken into second, minute, hour, day, ...
"Epoch time" is seconds from a reference instant.

The RTC chip implements calendar time.  (Do any of them support epoch time?)

Unix has a standard library for calendar time and epoch time. (time.h)
Energia doesn't support it.
Paul Stoffgren's Arduino Time library is similar to the Unix standard library (implements calendar and epoch time.)

The design for setting alarm converts from calendar time to epoch time, does the math (simple integer math), and converts back.



Features of Alarm class
-

Only uses alarm feature of the rtc chip.  The rtc keeps time and raises an alarm.

No timesetting.  The time on the rtc has as epoch (zero time) the instant the rtc comes out of reset.

The rtc is not synchronized to any other clock (not wall time).

An rtc chip typically has many other features that are not used here:

     - "real" time (synchronized to sideral time)
     - calendar features (e.g. what is the day of the week)
     - incoming interrupts to the RTC
     - other interrupts from the RTC
     - oscillator failure detection
     - external RAM on the rtc

Dependencies
-

Depends on these other libraries: 

    not:  https://github.com/nigelb/Arduino-AB08XX
    SPI library (in Arduino or Energia distribution)
    MSP430 DriverLib
    Arduino Time, Paul Stoffgren

Classes
-

    app main.c
       DutyCycleLib
           AlarmLib
               RTC  ( similar to Arduino-AB08XX library)
                  Bridge
                  SPI 
               Time (for Arduino, author Paul Stoffgren)
               Some mcu HAL e.g. MSP430 DriverLib (to set up the interrupt pin)

Power on reset (POR) and independent processes
-

The library assumes both chips are powered on and off together, only when Vcc is above both chips minimum.
The two chips are independent processes and either could be in reset (and not communicating) independently.
The library does not assume that mcu and rtc reset timing is the same.

Two cases:

     Vcc falling into reset
     Vcc rising out of reset
     

The library runs on the mcu and is SPI master.
The library checks that the rtc is out of reset before trying to communicate with it.
The library implements the interrupt from the rtc as a pulse, as part of the solution.
See the API about isSPIReady().

API
-

See externalAlarmLib.h

Implementation
-

Library defines static classes (class methods only.)
No instances need be allocated.
No class has any state variables.
The library has state, but it is in the rtc registers.
 and mcu registers.


Configuration
=

MCU Configuration
-

Some configuration is via macros.
Configuration is static, at compile time.
The macros configure the mcu pin used by the library for the alarm interrupt.

Other configuration of the mcu (the peripheral and set of pins used for SPI)
is configured by the choice of implementation choice of the Arduino/Energia SPI library,
which uses a predefined set of mcu pins.
(But there are still choices that could be hacked in this library, 
of a second SPI peripheral using SPI::setModule()
and of a different SS pin using SPI::begin(SSpin) (the overloaded method currently not used.)

This library configures mcu's SPI parameters for the AB0815 chip (i.e. SPI mode, clock frequency, etc.)

RTC Configuration
-

The library configures the RTC to use its Fout/nIRQ pin for the alarm interrupt.


Wiring
-

Four wires for SPI: SClk, MOSI, MISO, SS (aka chip select).
One wire for alarm interrupt (from RTC to MCU.)


MSP430FR low power mode LPM4.5
-

The mcu is powered down while sleeping.  The cpu is reset: all ram and cpu registers are lost.  FRAM memory is retained.  
Most importantly, during sleep, configuration of GPIO pins is locked and retained.

On entering sleep, you must configure GPIO pins to a low-power state.
That means only pins for wake by interrupt should be configured.

On waking, you must pre-reconfigure GPIO pins to their state before sleep was entered.
Pre-reconfigure means that the configuration has no effect until you unlock the GPIO configuration.

Here, the state of the GPIO pins on entering sleep is: 

    - only one pin configured as input for an alarm interrupt from RTC.
    - GPIO pins normally used for SPI are configured not-for-SPI, i.e. just output pins

The sequence of GPIO pin configuration is:

On power on reset (GPIO configuration is not locked):

    configure SPI pins
    configure alarm pin
    configure rtc
    set alarm
    enter sleep

On enter sleep:

    un configure SPI pins
    sleep
    
On waking from low-power mode (GPIO configuration is locked):

     pre reconfigure alarm pin
     unlock GPIO configuration
     configure SPI pins
     (rtc remains configured)
     service alarm
     set alarm
     enter sleep
    
