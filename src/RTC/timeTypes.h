
#pragma once

/*
 * See also AB08xx/rtcTime.h for RTCTime
 */

typedef struct  {
  unsigned char Second;
  unsigned char Minute;
  unsigned char Hour;

  unsigned char Day;
  unsigned char Month;
  unsigned char Year;   // offset from 1970;
} CalendarTime;


// unsigned char Wday;   // day of week, sunday is day 1


/*
 * On most embedded machines, 32 bits, suffers from 2038 problem.
 */
typedef unsigned long EpochTime;
