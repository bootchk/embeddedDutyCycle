class TestMain {
private:
    /*
     * Ensure green LED change to OUT will be visible.
     * !!! Regardless of cpu LOCKLPM5 state and prior GPIO configuration
     */
    static void ensureGreenLEDLightable();
    static void ensureRedLEDLightable();

public:
    static bool isResetAwakeFromSleep();
    static void initAllGpioOutLow();

    static void configureButtonWakeupSource();

    static void blinkRedLED();
    static void blinkGreenLED(unsigned int);\
    static void blinkRedLED(unsigned int);


    static void lightGreenLED();

    /*
     * Light green LED.  Does not return.
     */
    static void abortGreenLED();

    /*
     * Blink green LED.
     * Does not require prior configuration (and may mess prior GPIO config up.)
     */
    static void blinkForcedGreenLED(unsigned int);

    /*
     * Warble LED.  Does not return.
     */
    static void warbleGreenLEDForever();
    static void warbleRedLEDForever();
};
