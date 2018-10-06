
class ADC {
    // configure to measure SC proportion to Vcc
    // Since Vcc varies, this doesn't give an absolute voltage
    static void configureForSolarCellVoltageProportionToVcc();
    // configure to measure SC proportion to 1.5V band gap ref
    // The VBG also varies slightly with Vcc, but this is more accurate than the above.
    static void configureForSolarCellVoltageProportionTo1_5VBG();
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
    static unsigned int measureSolarCellProportionTo1_5VBG();
    static unsigned int measureSolarCellProportionToVcc();

    static void configureSolarCellVoltagePin();
};
