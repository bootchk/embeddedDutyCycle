
class ADC {
    static void configureForSolarCellVoltagePin();
    static void configureForVccMeasure();

    // Configuration common to the above
    static void configureCommon();

public:
    static void configureSolarCellVoltagePin();

    // Requires configureSolarCellVoltagePin
    static bool isSolarCellDark();

    static bool isVccHigh();
};
