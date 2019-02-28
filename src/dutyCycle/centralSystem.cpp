
#include "centralSystem.h"

// msp430Drivers
#include <SoC/SoC.h>

 /*
 * Configure system (after reset)
 * - faults we want to catch (NMI for vacant memory access)
 * - capability we want (write to FRAM)
 */
void CentralSystem::configureAfterReset() {

    // We want to catch bus errors for accessing memory out of bounds
    SoC::enableBSLOffAndVacantMemoryNMI();

    // We have persistent variables in FRAM that we write.
    // Prevent NMI on FRAM writes
    // TODO protect program code
    SoC::disableFRAMWriteProtect();

    /*
     *  XT1 defaults to autooff.
     *  Framework does not use XT1, so it should be off and this is not required.
     */
    /// SoC::disableXT1();

    // This doesn't seem to have any effect.
    // Docs show that SMCLK defaults to on, but since it is driven by MCLK and divisor is 1,
    // turning it off does not save power.  But we drive eUSCI_B with it?
    //SoC::turnOffSMCLK();
}
