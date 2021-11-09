
@begin
include "cl_vdv301_basic.h"
@end

// === constants and definitions ===============================================

const char XML_HEADER[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

// === global variables ========================================================

vdv301_operation_descr_tree_s g_vdv301_operation_descr_tree;

// === methods of generated structures =========================================

// -- vdv301_operation_descr_s --
@begin
methods vdv301_operation_descr_s
@end

// -- vdv301_operation_descr_tree_s --
@begin
methods vdv301_operation_descr_tree_s
@end

// -- lli_array_s --
@begin
methods lli_array_s
@end

// -- bd_array_s --
@begin
methods bd_array_s
@end

// -- xs_dateTime_s --
void xs_dateTime_s_create(xs_dateTime_s *this,time_s a_time)
{/*{{{*/
  datetime_s dt;
  datetime_s_from_nanosec(&dt,a_time);
  string_s_set_format(this,
      "%4.4hu-%2.2hhu-%2.2hhuT%2.2hhu:%2.2hhu:%2.2hhu.%3.3hu",
      dt.year,dt.month,dt.day,dt.hour,dt.min,dt.sec,dt.msec);
}/*}}}*/

int xs_dateTime_s_parse(xs_dateTime_s *this,time_s *a_time)
{/*{{{*/
  char *ptr = this->data;
  char *end_ptr;

  do {
    datetime_s dt = {};

    dt.year = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 4 || *end_ptr != '-') { break; }

    ptr = end_ptr + 1;
    dt.month = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != '-') { break; }

    ptr = end_ptr + 1;
    dt.day = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != 'T') { break; }

    ptr = end_ptr + 1;
    dt.hour = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

    ptr = end_ptr + 1;
    dt.min = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

    ptr = end_ptr + 1;
    double sec = strtod(ptr,&end_ptr);
    dt.sec = sec;
    dt.msec = (sec - dt.sec)*1000;

    if (datetime_s_to_nanosec(&dt,a_time))
    {
      break;
    }

    char mod = *end_ptr;
    switch (mod)
    {
    case '\0':
    case 'Z':
      return 0;
    case '-':
    case '+':
      do {
        ptr = end_ptr + 1;
        dt.hour = strtoll(ptr,&end_ptr,10);
        if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

        ptr = end_ptr + 1;
        dt.min = strtoll(ptr,&end_ptr,10);
        if (end_ptr - ptr != 2 || *end_ptr != '\0') { break; }

        ulli mod_value = (dt.hour*60ULL + dt.min)*60000000000ULL;

        if (mod == '-')
        {
          *a_time -= mod_value;
        }
        else
        {
          *a_time += mod_value;
        }

        return 0;
      } while(0);
      break;
    }
  } while(0);

  throw_error(VDV301_DATE_TIME_PARSE_ERROR);
}/*}}}*/

// -- xs_duration_s --
void xs_duration_s_create(xs_duration_s *this,lli a_period)
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,buffer);

  if (a_period < 0)
  {
    bc_array_s_push(&buffer,'-');
    a_period = -a_period;
  }

  bc_array_s_push(&buffer,'P');

  if (a_period >= CL_TIME_NANOSEC_IN_DAY)
  {
    lli days = a_period / CL_TIME_NANOSEC_IN_DAY;
    a_period %= CL_TIME_NANOSEC_IN_DAY;
    bc_array_s_append_format(&buffer,"%lldD",days);
  }

  int time_section = 0;

  if (a_period >= CL_TIME_NANOSEC_IN_HOUR)
  {
    lli hours = a_period / CL_TIME_NANOSEC_IN_HOUR;
    a_period %= CL_TIME_NANOSEC_IN_HOUR;
    bc_array_s_append_format(&buffer,"T%lldH",hours);
    time_section = 1;
  }

  if (a_period >= CL_TIME_NANOSEC_IN_MINUTE)
  {
    lli minutes = a_period / CL_TIME_NANOSEC_IN_MINUTE;
    a_period %= CL_TIME_NANOSEC_IN_MINUTE;
    bc_array_s_append_format(&buffer,"%s%lldM",time_section ? "" : "T",minutes);
    time_section = 1;
  }

  // - truncate to milliseconds -
  a_period -= a_period % 1000000ULL;

  if (a_period != 0 || buffer.data[buffer.used - 1] == 'P')
  {
    if (!time_section)
    {
      bc_array_s_push(&buffer,'T');
    }

    if (a_period % CL_TIME_NANOSEC_IN_SECOND != 0)
    {
      double seconds = (double)a_period / CL_TIME_NANOSEC_IN_SECOND;
      bc_array_s_append_format(&buffer,"%.3fS",seconds);
    }
    else
    {
      lli seconds = a_period / CL_TIME_NANOSEC_IN_SECOND;
      bc_array_s_append_format(&buffer,"%lldS",seconds);
    }
  }

  string_s_set(this,buffer.used,buffer.data);
}/*}}}*/

int xs_duration_s_parse(xs_duration_s *this,lli *a_period)
{/*{{{*/
  int negative = 0;
  lli period = 0;
  lli value;

  char *ptr = this->data;
  char *end_ptr;

  if (*ptr == '-')
  {
    negative = 1;
    ++ptr;
  }

  if (*ptr++ != 'P')
  {
    throw_error(VDV301_DURATION_PARSE_ERROR);
  }

  do
  {
    value = strtoll(ptr,&end_ptr,10);

    // - termination -
    if (*end_ptr == '\0')
    {
      if (ptr != end_ptr)
      {
        throw_error(VDV301_DURATION_PARSE_ERROR);
      }

      break;
    }

    switch (*end_ptr)
    {
    case 'D':
      period += value*CL_TIME_NANOSEC_IN_DAY;
      break;
    case 'T':
      break;
    case 'H':
      period += value*CL_TIME_NANOSEC_IN_HOUR;
      break;
    case 'M':
      period += value*CL_TIME_NANOSEC_IN_MINUTE;
      break;
    case 'S':
      period += value*CL_TIME_NANOSEC_IN_SECOND;
      break;
    case '.':
      {
        double secs = strtod(ptr,&end_ptr);

        if (*end_ptr != 'S')
        {
          throw_error(VDV301_DURATION_PARSE_ERROR);
        }

        period += secs*CL_TIME_NANOSEC_IN_SECOND;
      }
      break;
    default:
      throw_error(VDV301_DURATION_PARSE_ERROR);
    }

    ptr = end_ptr + 1;

  } while(1);

  *a_period = negative ? -period : period;

  return 0;
}/*}}}*/

