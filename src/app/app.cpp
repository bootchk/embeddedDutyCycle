
#include "app.h"


// Alternate apps: constant duty cycle, or task scheduled duty cycle
///#define CONSTANT_DUTY

#ifdef CONSTANT_DUTY
#include "../blinkerAppFixed/blinker.h"
#else
#include <src/blinkerAppTasked/smartBlinker.h>
#endif

// Test
///#include <src/debug/test.h>




void App::onPowerOnReset() {
	configureSleepingGPIO();
	// assert app's pins (LED) configured

	// initialize state

#ifdef CONSTANT_DUTY
	Blinker::init();
#else
	SmartBlinker::init();
#endif
}


void App::onWakeForAlarm() {
	// require app's persistent GPIO still configured (LED)

	/*
	 * app state transition
	 *
	 * In general design, there can be many tasks, using scheduler.
	 * For a design with only one task, do it now.
	 */
    ///TestMain::blinkForcedGreenLED(4);

    /*
     * Execute ready task, and usually schedule more tasks.
     */
#ifdef CONSTANT_DUTY
    Blinker::onAlarm();
#else
    SmartBlinker::onAlarm();
#endif
}




Duration App::constantDurationOfSleep() {
	return 10;
}




EpochTime App::timeToWake() {

#ifdef CONSTANT_DUTY
    return Blinker::timeToWake();
#else
    /*
     * A general design:
     * set alarm with a time from scheduler
     */
    return SmartBlinker::timeToWake();
#endif
}



void App::configureSleepingGPIO() {
	// App uses an LED during sleep
#ifdef CONSTANT_DUTY
    Blinker::configureGPIO();
#else
	SmartBlinker::configureGPIO();
#endif
}
