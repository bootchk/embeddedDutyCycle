
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

    if (PowerMgr::isPowerForBlinking()) {
        onPowerForBlinking();
        // check sunrise much later tonight
        // scheduleLaggedCheckSunriseTask();
        // OR after morning blink period
    }
    else {
        // Not enough power to blink, check sunrise all night long
        scheduleCheckSunriseTask();
        // No blinking task
    }
    // assert blinking task or checkSunriseTask is scheduled
}






void SmartBlinker::onPowerForBlinking() {
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
    else {
        /*
         * We don't know when to start, so omit morning blinking.
         * But we must schedule same tasks as if we had done morning blinking,
         * i.e. checkSunrise
         */
        scheduleCheckSunriseTask();
    }
}


void SmartBlinker::onMorningBlinkPeriodOver() {
    /*
     * In this design, we don't check for sunrise until morning blink is over.
     * That should be soon enough for an advancing sunrise due to seasons,
     * That is also safer: no possiblity of falsely detecting sunrise, then sunset while it is really night.
     */
    scheduleCheckSunriseTask();
}

