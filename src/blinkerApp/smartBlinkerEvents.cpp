
#include "smartBlinker.h"

// modules of app
#include "day.h"
#include "powerMgr.h"
#include "blinkPeriod.h"




/*
 * Events
 */

void SmartBlinker::onSunriseDetected() {
    Day::setSunrise();

    scheduleCheckSunsetTask();
    /// Optional optimization of power:
    /// check sunset much later today
    /// scheduleLaggedCheckSunsetTask();
}



void SmartBlinker::onSunsetDetected() {
    // We don't record sunset

    scheduleCheckSunriseTask();

    if (PowerMgr::isPowerForBlinking()) {
        onPowerLevelGood();
    }
    else {
        // No task except checkSunrise
    }

    // check sunrise much later tonight
    // scheduleLaggedCheckSunriseTask();
}




void SmartBlinker::onPowerLevelGood() {
    BlinkPeriod::initForEveningBlinking();
    scheduleFirstEveningBlinkTask();
}


void SmartBlinker::onEveningBlinkPeriodOver() {
    BlinkPeriod::initForMorningBlinking();
    scheduleFirstMorningBlinkTask();
}
