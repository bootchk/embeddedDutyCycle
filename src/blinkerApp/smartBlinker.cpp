
#include "smartBlinker.h"

#include "../OS/taskScheduler.h"
#include "../peripheral/ADC/adc.h"


#include "../blinkerApp/blinker.h"
#include "../epochClock/epochClock.h"




void SmartBlinker::checkSunriseTask() {
    if ( not ADC::isSolarCellDark()) {
        // sunrise
        onSunriseDetected();
    }
    else {
        // still dark, schedule self again short time later
        scheduleCheckSunriseTask();
    }

}



void SmartBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(0,
                                Blinker::onAlarm,
                                EpochClock::timeDurationFromNow(10) );
}

void SmartBlinker::scheduleCheckSunriseTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunriseTask,
                                EpochClock::timeDurationFromNow(900));
}
