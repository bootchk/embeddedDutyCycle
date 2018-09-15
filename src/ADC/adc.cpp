
#include "adc.h"

#include "../board.h"

// DriverLib
#include <gpio.h>
#include <adc.h>


void ADC::configureSolarCellVoltagePin() {
    GPIO_setAsPeripheralModuleFunctionInputPin(
            SolarCellVoltagePort,
            SolarCellVoltagePin,
            SolarCellVoltagePinFunction);
}



bool ADC::isVccHigh() {
    configureForVccMeasure();

    // Cloned from below
    ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);
    while ( ADC_isBusy(ADC_BASE) == ADC_BUSY );
    unsigned int voltage = ADC_getResults(ADC_BASE);

    // TODO
    // Voltage is 90% of reference of xxV
    bool result = (voltage < 0x80);
    return result;
    return true;
}


/*
 * Compare voltage on solar cell to Vcc
 * Vcc is above 1.9V else would not be booted.
 * Return if solar cell voltage is 1/2 of Vcc i.e. about 0.9V
 */
bool ADC::isSolarCellDark() {
    configureForSolarCellVoltagePin();

    //Enable and Start the conversion
    //in Single-Channel, Single Conversion Mode
    ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);

    // Spin until result ready
    while ( ADC_isBusy(ADC_BASE) == ADC_BUSY );


    unsigned int voltage = ADC_getResults(ADC_BASE);
    bool result = (voltage < 0x80);
    return result;
}

void ADC::configureCommon() {
    //Initialize the ADC Module
    /*
     * Base Address for the ADC Module
     * Use internal ADC bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC_init(ADC_BASE,
    ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC,
             ADC_CLOCKDIVIDER_1);

    ADC_enable(ADC_BASE);

    /*
     * Base Address for the ADC Module
     * Sample/hold for 16 clock cycles
     * Do not enable Multiple Sampling
     */
    ADC_setupSamplingTimer(ADC_BASE,
    ADC_CYCLEHOLD_16_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);

    // Default read-back format is unsigned

    ADC_setResolution(ADC_BASE,
            ADC_RESOLUTION_8BIT);
}



// See adc.h for these constants for input sources
//ADC_INPUT_DVSS

void ADC::configureForVccMeasure() {
    configureCommon();

    /*
     * To measure vcc,
     * sample the 1.5V reference from the PMM
     * with DVCC as input reference
     */
    ADC_configureMemory(ADC_BASE,
                ADC_INPUT_REFVOLTAGE,              // <<<<<<<<
                ADC_VREFPOS_AVCC,
                ADC_VREFNEG_AVSS);
}


void ADC::configureForSolarCellVoltagePin() {
    configureCommon();

    //Configure Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input A7
     * Use positive reference of AVcc
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
            ADC_INPUT_A7,              // <<<<<<<<
            ADC_VREFPOS_AVCC,
            ADC_VREFNEG_AVSS);
    // TODO correct input
}
