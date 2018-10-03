
#include "adc.h"

#include "../../board.h"

// DriverLib
#include <gpio.h>
#include <adc.h>
#include <pmm.h>


/*
 * See mspware/examples/device/.../msp430fr243x_adc10_05.c
 * for example code to measure vcc
 * from which I derived this code.
 */

void ADC::configureSolarCellVoltagePin() {
    GPIO_setAsPeripheralModuleFunctionInputPin(
            SolarCellVoltagePort,
            SolarCellVoltagePin,
            SolarCellVoltagePinFunction);
}


// 8-bit resolution
#define MaxADCRead 255


unsigned long ADC::measureVccCentiVolts() {
    configureForVccMeasure();

    // Wait for VBG internal bandgap reference to stabilize ???
    unsigned int adcResult = read();

    return convertADCReadToCentiVolts(adcResult);
}


/*
 * Compare voltage on solar cell to Vcc
 * Vcc is above 1.9V else would not be booted.
 * Return if solar cell voltage is 1/2 of Vcc i.e. about 0.9V
 */
unsigned long ADC::measureSolarCellCentiVolts() {
    configureForSolarCellVoltagePin();

    unsigned int adcResult = read();

    return convertADCReadToCentiVolts(adcResult);
}



/*
 * Private read routine
 */
unsigned int ADC::read()
{
    // Require configuration

    //Enable and Start conversion  in Single-Channel, Single Conversion Mode
    ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);

    // Spin until result ready
    while (ADC_isBusy(ADC_BASE) == ADC_BUSY) ;

    return ADC_getResults(ADC_BASE);
}



unsigned long ADC::convertADCReadToCentiVolts(unsigned int adcResult) {
    /*
     * DVcc is voltage on the digital Vcc pin (versus analog Vcc pin?)
     * In other words, the voltage on the Vcc network in the system.
     *
     * To calculate DVCC with 10-bit resolution, the following float equation is used
     * DVCC = (1023 * 1.5) / adcResult
     *
     * The following equation is modified to use only integers instead of float.
     * (A time optimization.)
     * The result is in hundreths of volts; needs to be divided by 100 to obtain units volts.
     * DVCC = (1023 * 150) / adcResult
     *
     * For 10-bit resolution need a long to hold 1023*150)
     * For 8-bit resolution, further optimizations possible (not need a long.)
     */
    unsigned long result = ((unsigned long) MaxADCRead * (unsigned long) 150)
            / (unsigned long) (adcResult);
    return result;
}



/*
 * Private configuration routines
 */


void ADC::configureVoltageBandgapReference() {
    PMM_enableInternalReference();

    // spin
    while (not PMM_getVariableReferenceVoltageStatus == PMM_REFGEN_READY) ;
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
    configureVoltageBandgapReference();

    configureCommon();

    /*
     * To measure vcc,
     * sample the 1.5V bandgap reference from the PMM
     * with DVCC as input reference.
     * The sampled bandgap varies in proportion to vcc.
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
