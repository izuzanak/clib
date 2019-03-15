
#ifndef __CL_TIME_H
#define __CL_TIME_H

@begin
include "cl_struct.h"
@end

#include <time.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libtime_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libtime_cll_EXPORTS
#define libtime_cll_EXPORT __declspec(dllexport)
#else
#define libtime_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_DATETIME_CANNOT_CONVERT_TO_NANOSEC 1

#define ERROR_TIME_WRONG_INIT_STRING 1

#define ERROR_CLOCK_CANNOT_GET_RESOLUTION 1
#define ERROR_CLOCK_CANNOT_GET_TIME 2

// === constants and definitions ===============================================

// - days in constants -
#define c_days_to_epoch     134774
#define c_days_in_400_years 146097
#define c_days_in_100_years 36524
#define c_days_in_4_years   1461

// === definition of generated structures ======================================

// -- datetime_s --
@begin
struct
<
usi:year
uc:month
uc:day
uc:wday
uc:hour
uc:min
uc:sec
usi:msec
usi:usec
usi:nsec
>
datetime_s;
@end

libtime_cll_EXPORT void datetime_s_from_nanosec(datetime_s *this,ulli a_nanosec);
static inline void datetime_s_from_seconds(datetime_s *this,ulli a_seconds);
libtime_cll_EXPORT int datetime_s_to_nanosec(const datetime_s *this,ulli *a_nanosec);
static inline int datetime_s_is_leap_year(unsigned a_year);

// === definition of structure time_s ==========================================

typedef ulli time_s;
@begin
define time_s basic
@end

#if OPTION_TO_STRING == ENABLED
static inline void time_s_to_string(const time_s *this,bc_array_s *a_trg);
#endif
static inline void time_s_to_json(const time_s *this,bc_array_s *a_trg);
static inline void time_s_now(time_s *this);
WUR libtime_cll_EXPORT int time_s_from_string(time_s *this,unsigned a_length,const char *a_data);
WUR static inline int time_s_from_string_ptr(time_s *this,const char *a_data);
static inline ulli time_s_nano(const time_s *this);
static inline ulli time_s_micro(const time_s *this);
static inline ulli time_s_milli(const time_s *this);
static inline ulli time_s_seconds(const time_s *this);
static inline ulli time_s_minutes(const time_s *this);
static inline ulli time_s_hours(const time_s *this);
static inline ulli time_s_days(const time_s *this);
static inline void time_s_datetime(const time_s *this,datetime_s *a_datetime);

// === definition of structure clock_s =========================================

WUR static inline int clock_s_getres(clockid_t a_clock_id,ulli *a_nanosec);
WUR static inline int clock_s_gettime(clockid_t a_clock_id,ulli *a_nanosec);

// === inline methods of generated structures ==================================

// -- datetime_s --
@begin
inlines datetime_s
@end

static inline void datetime_s_from_seconds(datetime_s *this,ulli a_seconds)
{/*{{{*/
  datetime_s_from_nanosec(this,a_seconds*1000000000ULL);
}/*}}}*/

static inline int datetime_s_is_leap_year(unsigned a_year)
{/*{{{*/
  return (a_year % 4 == 0 && a_year % 100 != 0) || (a_year % 400 == 0);
}/*}}}*/

// === inline methods of structure time_s ======================================

#if OPTION_TO_STRING == ENABLED
static inline void time_s_to_string(const time_s *this,bc_array_s *a_trg)
{/*{{{*/

  // - convert time to datetime structure -
  datetime_s datetime;
  datetime_s_from_nanosec(&datetime,*this);

  bc_array_s_append_format(a_trg,"%4.4hu/%2.2hu/%2.2hu %2.2hu:%2.2hu:%2.2hu"
    ,datetime.year,datetime.month,datetime.day,datetime.hour,datetime.min,datetime.sec);
}/*}}}*/
#endif

static inline void time_s_to_json(const time_s *this,bc_array_s *a_trg)
{/*{{{*/
  ulli_to_json(this,a_trg);
}/*}}}*/

static inline void time_s_now(time_s *this)
{/*{{{*/
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
  struct timeval tv;
  gettimeofday(&tv,NULL);

  *this = (tv.tv_sec*1000000000ULL + tv.tv_usec*1000ULL);
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);

  ULARGE_INTEGER ularge_int = { ft.dwLowDateTime,ft.dwHighDateTime };
  *this = ((ularge_int.QuadPart - 116444736000000000ULL)*100ULL);
#else
  cassert(0);
#endif
}/*}}}*/

static inline int time_s_from_string_ptr(time_s *this,const char *a_data)
{/*{{{*/
  return time_s_from_string(this,strlen(a_data),a_data);
}/*}}}*/

static inline ulli time_s_nano(const time_s *this)
{/*{{{*/
  return *this;
}/*}}}*/

static inline ulli time_s_micro(const time_s *this)
{/*{{{*/
  return *this/1000ULL;
}/*}}}*/

static inline ulli time_s_milli(const time_s *this)
{/*{{{*/
  return *this/1000000ULL;
}/*}}}*/

static inline ulli time_s_seconds(const time_s *this)
{/*{{{*/
  return *this/1000000000ULL;
}/*}}}*/

static inline ulli time_s_minutes(const time_s *this)
{/*{{{*/
  return *this/60000000000ULL;
}/*}}}*/

static inline ulli time_s_hours(const time_s *this)
{/*{{{*/
  return *this/3600000000000ULL;
}/*}}}*/

static inline ulli time_s_days(const time_s *this)
{/*{{{*/
  return *this/86400000000000ULL;
}/*}}}*/

static inline void time_s_datetime(const time_s *this,datetime_s *a_datetime)
{/*{{{*/
  datetime_s_from_nanosec(a_datetime,*this);
}/*}}}*/

// === inline methods of structure clock_s =====================================

static inline int clock_s_getres(clockid_t a_clock_id,ulli *a_nanosec)
{/*{{{*/
  struct timespec res;

  // - ERROR -
  if (clock_getres(a_clock_id,&res) != 0)
  {
    throw_error(CLOCK_CANNOT_GET_RESOLUTION);
  }

  *a_nanosec = res.tv_sec*1000000000LL + res.tv_nsec;

  return 0;
}/*}}}*/

static inline int clock_s_gettime(clockid_t a_clock_id,ulli *a_nanosec)
{/*{{{*/
  struct timespec tp;

  // - ERROR -
  if (clock_gettime(a_clock_id,&tp) != 0)
  {
    throw_error(CLOCK_CANNOT_GET_TIME);
  }

  *a_nanosec = tp.tv_sec*1000000000ULL + tp.tv_nsec;

  return 0;
}/*}}}*/

#endif

