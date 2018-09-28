
#pragma once


/*
 * Routines for testing
 *
 * Some can be made impotent at compile time.
 */


class Test {


public:
    static bool isResetAwakeFromSleep();
    static void initAllGpioOutLow();

    static void configureButtonWakeupSource();


    static void blinkRedLED();
    static void blinkGreenLED(unsigned int);\
    static void blinkRedLED(unsigned int);

    static void lightGreenLED();



    /*
     * Blink green LED.
     * Does not require prior configuration (and may mess prior GPIO config up.)
     */
    static void blinkForcedGreenLED(unsigned int);




    static void delayBriefly();
};
