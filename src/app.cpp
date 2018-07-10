
#include "app.h"

#include "LED/led.h"

// C syntax: #pragma PERSISTENT(ledState)
#pragma PERSISTENT
bool ledState = false;




void App::onPowerOnReset() {
	configureSleepingGPIO();
	// assert app's pins (LED) configured

	// initialize state
	LED::turnOn();
	ledState = true;

	// decide first alarm duration
}

void App::onWakeForAlarm() {
	// require app's persistent GPIO still configured (LED)

	// app state transition

	// toggle LED every time we wake
	if (ledState) {
		LED::turnOff();
	}
	else {
		LED::turnOn();
	}
	ledState = ! ledState;

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
