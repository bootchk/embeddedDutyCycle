
#include "app.h"

#include "../blinkerApp/blinker.h"
#include "../OS/taskScheduler.h"





void App::onPowerOnReset() {
	configureSleepingGPIO();
	// assert app's pins (LED) configured

	// initialize state
	Blinker::init();
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


unsigned int App::durationOfSleep() {
	/*
	 * In general design, duration til next task.
	 * For a design with only one task, could be a constant.
	 */
	return TaskScheduler::durationTilNextTask();
}


void App::configureSleepingGPIO() {
	// App uses an LED during sleep
	Blinker::configureGPIO();
}
