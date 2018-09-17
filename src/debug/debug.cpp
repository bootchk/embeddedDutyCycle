
#include "debug.h"

#include "../MCU/mcu.h"
/*
 * Not a trail, just the last mark.
 */


#pragma PERSISTENT
unsigned int breadCrumbTrail = 0;

#pragma PERSISTENT
unsigned int assertLineNumber = 0;



void Debug::leaveCrumb(unsigned int mark) {
    // ensure can write FRAM even after reset
    // Enable FRAM write
    MCU::disableFRAMWriteProtect();

    breadCrumbTrail = mark;
}

void Debug::persistLineNumber(unsigned int mark) {
    // ensure can write FRAM even after reset
    // Enable FRAM write
    MCU::disableFRAMWriteProtect();

    assertLineNumber = mark;
}
