
#include "app.h"

#include "../LED/led.h"





void App::onPowerOnReset() {
	configureSleepingGPIO();
	// assert app's pins (LED) configured

	// initialize state
	LED::turnOn();

	// decide first alarm duration
}

void App::onWakeForAlarm() {
	// require app's persistent GPIO still configured (LED)

	// app state transition

	///LED::toggle();
    LED::blink();


	// TODO decide next alarm duration
}


unsigned int App::durationOfSleep() {
	// TODO later, app get from state
	return 10;
}


void App::configureSleepingGPIO() {
	// App uses an LED during sleep
	LED::configure();
}
