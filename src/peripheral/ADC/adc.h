
class ADC {
    static void configureForSolarCellVoltagePin();
    static void configureForVccMeasure();

    // Configuration common to the above
    static void configureCommon();
    // used to measure vcc
    static void configureVoltageBandgapReference();

public:
    static void configureSolarCellVoltagePin();

    // Requires configureSolarCellVoltagePin
    static bool isSolarCellDark();

    static bool isVccHigh();
};
