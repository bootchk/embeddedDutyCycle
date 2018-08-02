
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



/*
 * Implementation is largely converting type (RTCTime) that RTC delivers
 * to type EpochTime (seconds since epoch) so we can use simple math to add Duration
 * then reverse conversion back to the type (RTCTime) that RTC expects.
 */
bool RTC::setAlarm(Duration duration) {
	bool result;

	// TODO later, check preconditions for setting alarm
	// duration is great enough

	RTCTime now;
	Bridge::readTime(&now);

	/*
	 * If RTC has failed, Bridge reads time as all zeroes.
	 */
	if ( not TimeConverter::isValidRTCTime(now) ) return false;

	// 2 step conversion from RTCTime to epoch time
	CalendarTime calendarTime = TimeConverter::convertRTCTimeToCalendarTime(now);
	EpochTime nowTime = TimeConverter::convertCalendarTimeToEpochTime(calendarTime);

	if ( not timeIsMonotonic(nowTime)) {
		result = false;
	}
	else {

		// calculate time of alarm
		EpochTime alarmEpochTime = nowTime + duration;

		// Reverse conversions
		CalendarTime alarmCalendarTime = TimeConverter::convertEpochTimeToCalendarTime(alarmEpochTime);
		RTCTime alarmRTCTime = TimeConverter::convertCalendarTimeToRTCTime(alarmCalendarTime);

		Bridge::writeAlarm(alarmRTCTime);

		// TODO later
		/*
		 * Verify alarm is properly set by reading and compare
		 * If it is not set properly, the system may sleep a very long time.
		 * Also verify that now time is not zero.
		 */
		RTCTime readAlarmRTCTime;
		Bridge::readAlarm(&readAlarmRTCTime);
		// assert(readAlarmRTCTime == alarmRTCTime);

		result = true;
	}

	return result;
}



