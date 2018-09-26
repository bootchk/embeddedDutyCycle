
#include <src/blinkerAppTasked/blinkPeriod.h>
#include <src/blinkerAppTasked/day.h>
#include <src/blinkerAppTasked/powerMgr.h>
#include <src/blinkerAppTasked/smartBlinker.h>




/*
 * Events
 */

void SmartBlinker::onSunriseDetected() {
    Day::setSunriseTime();

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

/*
 * We schedule evening blinking from a detected sunset.
 * We can only schedule morning blinking if there exists a prior detected sunrise.
 * On startup, might not exist.
 */

void SmartBlinker::onEveningBlinkPeriodOver() {
    if (Day::isSunriseTimeValid()) {
        BlinkPeriod::initForMorningBlinking();
        scheduleFirstMorningBlinkTask();
    }
    // else omit morning blinking.
}

