class TestMain {
public:
    static bool isResetAwakeFromSleep();
    static void initAllGpioOutLow();

    static void configureButtonWakeupSource();

    static void blinkRedLED();
    static void lightGreenLED();
};
