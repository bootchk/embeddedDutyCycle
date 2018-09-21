
#include "app.h"

/// simple
///#include "../blinkerApp/blinker.h"
#include "../blinkerApp/smartBlinker.h"

#include "../OS/taskScheduler.h"





void App::onPowerOnReset() {
	configureSleepingGPIO();
	// assert app's pins (LED) configured

	// initialize state

	///Blinker::init();
	SmartBlinker::init();

	/// For dev
	/// SmartBlinker::testTasks();
}


void App::onWakeForAlarm() {
	// require app's persistent GPIO still configured (LED)

	/*
	 * app state transition
	 *
	 * In general design, there can be many tasks, using scheduler.
	 * For a design with only one task, do it now.
	 */
    /*
     * Execute ready task, and usually schedule more tasks.
     */
    TaskScheduler::onAlarm();

    /// Blinker::onAlarm();
}

#ifdef TESTSIMPLE
Duration App::durationOfSleep() {
	/*
	 * For a design with only one task, could be a constant.
	 */
	return Duration::TenSeconds;
}
#endif




EpochTime App::timeToWake() {
    /*
     * A general design:
     * set alarm with a time from scheduler
     */
    return TaskScheduler::timeOfNextTask();
}

void App::configureSleepingGPIO() {
	// App uses an LED during sleep
	SmartBlinker::configureGPIO();
}
