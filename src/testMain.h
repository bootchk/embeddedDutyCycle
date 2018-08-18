class TestMain {
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
     * Warble green LED.  Does not return.
     */
    static void warbleGreenLEDForever();
};
