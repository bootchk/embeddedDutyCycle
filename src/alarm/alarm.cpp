
#include "alarm.h"

#include "../PMM/powerMgtModule.h"
#include "../alarmClock/RTC/realTimeClock.h"  // Avoid clash with rtc.h"
//#include "../AB08xx/bridge.h"	    //  hides SPI
#include "../pinFunction/pinFunction.h"    // hides GPIO functions



#include <src/debug/myAssert.h>






void Alarm::clearAlarm() {
        // May fail reset since RTC is remote device.
        Alarm::clearAlarmOnRTCOrReset();

        Alarm::clearAlarmOnMCU();
}





bool Alarm::clearAlarmOnRTC() {
	// Tell RTC to end interrupt pulse (signal to high) if not already so
	bool result;

	// Simple write to a register of RTC
	// RTC should raise nIRQ signal to high if not already so.
	// mcu interrupt occurred on rising edge.
	// TODO delay needed?
	RTC::clearIRQInterrupt();

	result = isAlarmInterruptSignalHigh();

    // assert alarm signal high or result is false
	return result;
}



void Alarm::clearAlarmOnRTCOrReset() {
	if (!Alarm::clearAlarmOnRTC())  {
		// RTC may be continuing to generate interrupt signal on Fout/nIRQ
		PMM::failClearAlarm();
	}
	// ensure RTC interrupt flag is clear
	// ensure interrupt signal net is high
}


void Alarm::clearAlarmOnMCU() {
	PinFunction::clearAlarmInterruptOnPin();
}


bool Alarm::isAlarmInterruptSignalHigh() {
	// requires pin configured as input

	return PinFunction::isAlarmPinHigh();
}





/*
 * Must be bulletproof since if alarm is failed to set, may sleep forever.
 */
bool Alarm::setAlarmInSeconds(Duration duration) {
	bool result;

	myAssert(isConfiguredForAlarming());

	// delegate to RTC
	result = RTC::setAlarmInSeconds(duration);

	// ensure alarm is set or result is false
	return result;
}


bool Alarm::setAlarmToTime(EpochTime time) {
    bool result;

    myAssert(isConfiguredForAlarming());

    // delegate to RTC
    result = RTC::setAlarmTime(time);

    // ensure alarm is set or result is false
    return result;
}

