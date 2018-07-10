
/*
 * Knows how to convert time formats.
 *
 * !!! With many subtle omissions.
 *
 * Goal here is to use Unix std implementations when possible.
 */

/*
 * A library similar to Unix std library
 *
 * If the platform support Unix std library:
 * #include <ctime>	// C time.h
 */
#include "timeTypes.h"


// Depends on type defined by RTC chip
#include "../AB08xx/rtcTime.h"



class TimeConverter {
public:
	/*
	 * Convert BCD encoded calendar into int encoded calendar.
	 * and vice versa
	 */
	static CalendarTime convertRTCTimeToCalendarTime(RTCTime&);
	static RTCTime convertCalendarTimeToRTCTime(CalendarTime&);

	/*
	 * Convert calendar time to epoch time and vice versa
	 */
	static EpochTime convertCalendarTimeToEpochTime(CalendarTime&);
	static CalendarTime convertEpochTimeToCalendarTime(EpochTime);

	static bool isValidRTCTime(RTCTime&);
};
