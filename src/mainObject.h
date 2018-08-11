
class Main {
public:

    static bool isResetAwakeFromSleep();

    /*
     * Early in reset, no matter what the cause.
     */
    static void onResetPreamble();

    static void onWakeFromLPM();

    static void onColdReset();

    static void configureWakeupSource();
};
