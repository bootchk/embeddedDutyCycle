
#pragma once


class SPIPins {
private:
    static void configureSelectSPIPin();
    static void configureDataSPIPins();


public:

    /*
     * Configure all pins needed for SPI
     */
    static void configure();
    static void unconfigure();

    static void selectSPISlave();
    static void deselectSPISlave();

};
