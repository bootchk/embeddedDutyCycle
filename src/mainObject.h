
class Main {
public:
    /*
     * Early in reset, no matter what the cause.
     */
    static void onResetPreamble();
    static void onWakeFromLPM();
    static void onColdReset();

    static void configureWakeupSource();
};
