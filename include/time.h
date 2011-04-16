//
// time.h
//
// Time routines
//
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.  
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.  
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
// SUCH DAMAGE.
// 

#define ASCBUFSIZE        (26 + 2)

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef TIME_H
#define TIME_H
 typedef unsigned int size_t;
typedef int ssize_t;
typedef long time_t;
typedef long clock_t;
typedef unsigned long tid_t;
typedef unsigned long pid_t;
/*#include <types.h>

#define osapi __declspec(dllimport)
*/

#define CLOCKS_PER_SEC  1000

#ifndef _TM_DEFINED
#define _TM_DEFINED

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


struct tib
{
  struct xcptrec *except;          // Exception handler list
  void *stacktop;                  // Topmost address of the threads stack
  void *stacklimit;                // Lowest committed page of the threads stack

  void *subsystemtib;
  unsigned long fiberdata;

  void *arbitrary;
  struct tib *self;                // Virtual address of TIB

  unsigned long unknown1;
  pid_t pid;                       // Process id
  tid_t tid;                       // Thread id
  unsigned long unknown2;

  void *tlsbase;                   // Pointer to TLS array
  //struct peb *peb;                 // Process environment block

  void *stackbase;                 // Lowest reserved address of the threads stack
  int errnum;                      // Per thread last error
  void *startaddr;                 // Start address for thread
  void *startarg;                  // Argument to thread start routine
  int flags;                       // Thread creation flags

  //handle_t hndl;                   // Handle for thread
  //struct job *job;                 // Job object for thread

  //struct siginfo *cursig;          // Current signal used by getsiginfo()

  //struct hostent host;             // Per-thread hostent buffer
  //unsigned char host_addr[sizeof(struct in_addr)];
  //char *h_addr_ptrs[MAX_HOST_ADDRS + 1];
  //char *host_aliases[MAX_HOST_ALIASES];
  //char hostbuf[HOSTBUF_SIZE];

  struct tm tmbuf;                 // For gmtime() and localtime()
  char *nexttoken;                 // For strtok()
  //char cvtbuf[CVTBUFSIZE];         // For ecvt() and fcvt()
  char ascbuf[ASCBUFSIZE];         // For asctime()
  //char tmpnambuf[MAXPATH];         // For tmpnam()
  //char cryptbuf[CRYPTBUFSIZE];     // For crypt()

  char reserved1[1498];

  //void *tls[MAX_TLS];              // Thread local storage
  char reserved2[240];
};

#endif

#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED

struct timeval 
{
  long tv_sec;                  // Seconds
  long tv_usec;                 // Microseconds
};

#endif

#ifndef _TIMEZONE_DEFINED
#define _TIMEZONE_DEFINED

struct timezone 
{
  int tz_minuteswest;           // Minutes west of Greenwich
  int tz_dsttime;               // Type of daylight saving correction
};

#endif

extern int _daylight;     // Non-zero if daylight savings time is used
extern long _dstbias;     // Offset for Daylight Saving Time
extern long _timezone;    // Difference in seconds between GMT and local time
extern char *_tzname[2];  // Standard/daylight savings time zone names


#define difftime(time2, time1) ((double)((time2) - (time1)))

#ifdef  __cplusplus
extern "C" {
#endif

struct tib *gettib();
struct tm *gmtime_r(const time_t *timer, struct tm *tmbuf);
struct tm *localtime_r(const time_t *timer, struct tm *tmbuf);

struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);

time_t mktime(struct tm *tmbuf);

size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *t);
/*
osapi clock_t clock();
osapi time_t time(time_t *timeptr);
*/
char *asctime(const struct tm *tp);
char *ctime(const time_t *tp);
char *_strdate(char *s);
char *_strtime(char *s);
void _tzset();

#ifdef  __cplusplus
}
#endif

#endif


