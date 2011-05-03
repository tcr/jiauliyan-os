/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 */

#ifndef TIME_H
#define TIME_H

#include <stddef.h>

#define CLOCKS_PER_SEC  1000

typedef unsigned int time_t;
typedef long clock_t;

struct tm
{
  int tm_sec;                   // Seconds after the minute [0, 59]
  int tm_min;                   // Minutes after the hour [0, 59]
  int tm_hour;                  // Hours since midnight [0, 23]
  int tm_mday;                  // Day of the month [1, 31]
  int tm_mon;                   // Months since January [0, 11]
  int tm_year;                  // Years since 1900
  int tm_wday;                  // Days since Sunday [0, 6]
  int tm_yday;                  // Days since January 1 [0, 365]
  int tm_isdst;                 // Daylight Saving Time flag
};

struct timeval 
{
  long tv_sec;                  // Seconds
  long tv_usec;                 // Microseconds
};

struct timezone 
{
  int tz_minuteswest;           // Minutes west of Greenwich
  int tz_dsttime;               // Type of daylight saving correction
};

extern int _daylight;     // Non-zero if daylight savings time is used
extern long _dstbias;     // Offset for Daylight Saving Time
extern long _timezone;    // Difference in seconds between GMT and local time
extern char *_tzname[2];  // Standard/daylight savings time zone names

#define difftime(time2, time1) ((double)((time2) - (time1)))

extern struct tm *gmtime_r(const time_t *timer, struct tm *tmbuf);
extern struct tm *localtime_r(const time_t *timer, struct tm *tmbuf);

extern struct tm *gmtime(const time_t *timer);
extern struct tm *localtime(const time_t *timer);

extern time_t mktime(struct tm *tmbuf);

extern size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *t);

extern clock_t clock();
extern time_t time(time_t *timeptr);

extern char *asctime(const struct tm *tp);
extern char *ctime(const time_t *tp);
extern char *_strdate(char *s);
extern char *_strtime(char *s);
extern void _tzset();

#endif
