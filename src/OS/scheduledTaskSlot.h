
#pragma once

#include "../RTC/timeTypes.h"   // EpochTime
#include "task.h"

#include "../debug/testMain.h"


/*
 * Knows a scheduled task.
 *
 * Many instances, in a container.
 */

class ScheduledTaskSlot {
public:
    EpochTime scheduledTime;
    TaskMethodPtr taskMethodPtr;
    bool isEmpty = true;

    void execute() {
        // Can reuse slot
        // Do this before executing, so executed task can reuse slot
        isEmpty = true;

        //TestMain::blinkForcedGreenLED(4);

        // Dereference function pointer and call function
        // C allows either syntax *fp() of fp()
        /// OLD (*taskMethodPtr)();
        taskMethodPtr();

        TestMain::blinkForcedGreenLED(6);
    }
};
