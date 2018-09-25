
/*
 * Part of implementation of class RTC.
 * The part dealing with alarm setting.
 * Which is sufficiently complex and unrelated that it is in a separate file.
 */

#include "../AB08xx/bridge.h"
#include "realTimeClock.h"
#include "timeConverter.h"




namespace {

/*
 * Time is not zero and greater than previous given time.
 */

bool timeIsMonotonic(EpochTime nowTime) {
	// TODO compare to previous read time, must be greater.
	return true;
}

}





/*
 * !!! If the RTC stops responding, system is failed, there is no practical action to take.
 * During testing, without proper connection to RTC,
 * this code seems to function since reads and writes to RTC will work as far as mcu can tell,
 * except that reads will return zero e.g. now time will be zero.
 */



EpochTime RTC::timeNow() {
    RTCTime now;
    EpochTime result;


    Bridge::readTime(&now);

    /*
     * If RTC has failed, Bridge reads time as all zeroes.
     */
    if (not TimeConverter::isValidRTCTime(now)) {
        result = 0;
    }
    else
    {
        result = TimeConverter::convertRTCTimeToEpochTime(now);
    }
    return result;
}




/*
 * Implementation is largely converting type (RTCTime) that RTC delivers
 * to type EpochTime (seconds since epoch) so we can use simple math to add Duration
 * then reverse conversion back to the type (RTCTime) that RTC expects.
 */
bool RTC::setAlarmInSeconds(Duration duration) {
	bool result;

	// TODO later, check preconditions for setting alarm
	// duration is great enough

	EpochTime nowTime = timeNow();
	// could be zero when RTC chip fails

	//if ( not timeIsMonotonic(nowTime)) {
	if ( nowTime == 0 ) {
		result = false;
	}
	else {
		// calculate time of alarm
		EpochTime alarmEpochTime = nowTime + static_cast<unsigned int> (duration);

		result = setAlarmTime(alarmEpochTime);
	}

	return result;
}


bool RTC::setAlarmTime(EpochTime& alarmEpochTime) {
    RTCTime alarmRTCTime;

    TimeConverter::convertEpochTimeToRTCTime(alarmEpochTime, alarmRTCTime);
    Bridge::writeAlarm(alarmRTCTime);

    return true;
    // TODO ? implementation correct
    ///return verifyAlarmTime(&alarmRTCTime);
}




bool RTC::verifyAlarmTime(const RTCTime* writtenTime) {
    RTCTime readAlarmRTCTime;

    /*
     * Verify alarm is properly set by reading and compare to what
     * If it is not set properly, the system may sleep a very long time.
     * Also verify that now time is not zero????
     *
     * Note the alarm register is not ticking.
     */

    Bridge::readAlarm(&readAlarmRTCTime);
    return(readAlarmRTCTime == writtenTime);
}

