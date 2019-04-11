
#pragma once

#include <alarmClock/time/timeTypes.h>   // Duration

//#include "../debug/test.h"
#include "task.h"



/*
 * Knows a scheduled task.
 *
 * Many instances, in a container.
 */

class ScheduledTaskSlot {
public:
    Duration durationUntilExecution;
    TaskMethodPtr taskMethodPtr;
    bool isEmpty = true;

    void execute() {
        // Can reuse slot
        // Do this before executing, so executed task can reuse slot
        isEmpty = true;

        ///Test::blinkForcedGreenLED(4);

        // Dereference function pointer and call function
        // C allows either syntax *fp() of fp()
        /// OLD (*taskMethodPtr)();
        taskMethodPtr();

        ///Test::blinkForcedGreenLED(6);
    }
};
