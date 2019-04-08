
@begin
include "cl_time.h"
@end

// === constants and definitions ===============================================

// - number of days in month -
const unsigned c_month_days[] = {31,28,31,30,31,30,31,31,30,31,30,31,0};
const unsigned c_month_days_leap[] = {31,29,31,30,31,30,31,31,30,31,30,31,0};

// - number of days of month to end of year -
const unsigned c_month_days_to_year_end[] = {0,31,59,90,120,151,181,212,243,273,304,334};

const unsigned *c_month_days_arr[2] = {c_month_days,c_month_days_leap};

// === methods of generated structures =========================================

// -- datetime_s --
@begin
methods datetime_s
@end

void datetime_s_from_nanosec(datetime_s *this,long long unsigned a_nanosec)
{/*{{{*/
  long long unsigned tmp = a_nanosec;

  // - retrieve time fields -
  this->nsec = tmp % 1000; tmp /= 1000;
  this->usec = tmp % 1000; tmp /= 1000;
  this->msec = tmp % 1000; tmp /= 1000;
  this->sec  = tmp % 60;   tmp /= 60;
  this->min  = tmp % 60;   tmp /= 60;
  this->hour = tmp % 24;   tmp /= 24;

  // - number of days plus days to epoch -
  unsigned days = tmp + c_days_to_epoch;

  // - calculate week day -
  this->wday = (days % 7) + 1;

  // - calculate year -
  this->year = 1601;

  this->year += (days / c_days_in_400_years) * 400;
  days %= c_days_in_400_years;

  this->year += (days / c_days_in_100_years) * 100;
  days %= c_days_in_100_years;

  this->year += (days / c_days_in_4_years) * 4;
  days %= c_days_in_4_years;

  // - last year of 4 year period can be leap year -
  if (days < 3 * 365)
  {
    this->year += days / 365;
    days %= 365;
  }
  else
  {
    this->year += 3;
    days -= 3 * 365;
  }
  // - calculate month -
  this->month = 1;
  const unsigned *month_days = datetime_s_is_leap_year(this->year) ? c_month_days_leap : c_month_days;
  while (days >= *month_days)
  {
    ++this->month;
    days -= *month_days++;
  }

  // - calculate day -
  this->day = days + 1;
}/*}}}*/

int datetime_s_to_nanosec(const datetime_s *this,long long unsigned *a_nanosec)
{/*{{{*/
  if (this->year < 1970)
  {
    throw_error(DATETIME_CANNOT_CONVERT_TO_NANOSEC);
  }

  unsigned tmp_year = this->year - 1601;

  // - number of days from epoch -
  long long unsigned time = tmp_year*365
    + tmp_year/4
    - tmp_year/100
    + tmp_year/400
    + c_month_days_to_year_end[this->month - 1]
    + ((datetime_s_is_leap_year(this->year) && this->month > 2) ? 1 : 0)
    + this->day - 1
    - c_days_to_epoch;

  // - calculate time from 1.1.1970 at nanoseconds -
  time *= 24;   time += this->hour;
  time *= 60;   time += this->min;
  time *= 60;   time += this->sec;
  time *= 1000; time += this->msec;
  time *= 1000; time += this->usec;
  time *= 1000; time += this->nsec;

  // - set value to result time -
  *a_nanosec = time;

  return 0;
}/*}}}*/

// === methods of structure time_s =============================================

int time_s_from_string(time_s *this,unsigned a_length,const char *a_data)
{/*{{{*/

  // - ERROR -
  if (a_length != 14)
  {
    throw_error(TIME_WRONG_INIT_STRING);
  }

  char str[15];
  memcpy(str,a_data,14);
  str[14] = '\0';

#define BIC_TIME_RETRIEVE_COMP(TARGET,START,END) \
{/*{{{*/\
  char tmp_char = str[END];\
  str[END] = '\0';\
\
  char *end_ptr;\
  TARGET = strtol(str + START,&end_ptr,10);\
\
  /* - ERROR - */\
  if (end_ptr != str + END)\
  {\
    throw_error(TIME_WRONG_INIT_STRING);\
  }\
\
  str[END] = tmp_char;\
}/*}}}*/

  datetime_s dt;

  dt.nsec = 0;
  dt.usec = 0;
  dt.msec = 0;

  BIC_TIME_RETRIEVE_COMP(dt.sec  ,12,14);
  BIC_TIME_RETRIEVE_COMP(dt.min  ,10,12);
  BIC_TIME_RETRIEVE_COMP(dt.hour ,8 ,10);
  BIC_TIME_RETRIEVE_COMP(dt.day  ,6 ,8);
  BIC_TIME_RETRIEVE_COMP(dt.month,4 ,6);
  BIC_TIME_RETRIEVE_COMP(dt.year ,0 ,4);

  // - ERROR -
  if (dt.year < 1970 || dt.month < 1 || dt.month > 12 || dt.day < 1 ||
      dt.day > c_month_days_arr[datetime_s_is_leap_year(dt.year)][dt.month - 1] ||
      dt.hour >= 24 || dt.min >= 60 || dt.sec >= 60)
  {
    throw_error(TIME_WRONG_INIT_STRING);
  }

  // - convert datetime to time value -
  datetime_s_to_nanosec(&dt,this);

  return 0;
}/*}}}*/

