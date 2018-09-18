
#pragma once

#include "../RTC/timeTypes.h"   // EpochTime
#include "task.h"




class ScheduledTask {
public:
    EpochTime scheduledTime;
    TaskMethodPtr taskMethodPtr;

    void execute() {
        //_isReady = false;

        // Dereference function pointer and call function
        (*taskMethodPtr)();
    }
};
