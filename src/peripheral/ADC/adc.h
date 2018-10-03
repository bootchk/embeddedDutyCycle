
class ADC {
    static void configureForSolarCellVoltagePin();
    static void configureForVccMeasure();

    // Configuration common to the above
    static void configureCommon();
    // used to measure vcc
    static void configureVoltageBandgapReference();

    /*
     * Sample ADC using existing configuration.
     */
    static unsigned int read();

    static unsigned long convertADCReadToCentiVolts(unsigned int);

public:
    // Does not require configure GPIO pin
    static unsigned long measureVccCentiVolts();
    // Requires prior configuration of GPIO pin
    static unsigned long measureSolarCellCentiVolts();

    static void configureSolarCellVoltagePin();
};
