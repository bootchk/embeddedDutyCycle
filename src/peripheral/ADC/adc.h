
class ADC {
    // configure to measure SC proportion to Vcc
    static void configureForSolarCellVoltagePin();
    // configure to measure SC centivolts
    static void configureForSolarCellVoltagePinFoo();
    // configure to measure Vcc in centiVolts
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
    static unsigned int measureSolarCellProportionToVcc();

    static void configureSolarCellVoltagePin();
};
