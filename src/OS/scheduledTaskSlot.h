
#pragma once

#include <alarmClock/time/timeTypes.h>   // Duration

//#include "../debug/test.h"
#include "task.h"



/*
 * Knows a scheduled task.
 *
 * Many instances, in a container.
 *
 * Tasks are created with two actions: schedule() and ready().
 * Scheduled when the task and MomentMethod is known but not the moment.
 * Later ready()ed at a moment defined by the time of ready() plus duration returned by MomentMethod.
 * In other words, at the time of scheduling, the duration til the moment need not be known,
 * only a method that will return the moment when the task is actually ready()ed.
 *
 */

class ScheduledTaskSlot {
public:
    /*
     * Method returning moment to be scheduled.
     */
    MomentMethodPtr momentMethodPtr;

    //OLD: duration known when slotted
    // Duration durationUntilExecution;


    /*
     * Task to be executed.
     */
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
