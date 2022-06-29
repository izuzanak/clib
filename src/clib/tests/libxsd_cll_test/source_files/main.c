
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libxsd_cll_test";

const char *test_names[] =
{/*{{{*/
  "xs_date",
  "xs_time",
  "xs_datetime",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_xs_date,
  test_xs_time,
  test_xs_datetime,
};/*}}}*/

// === test execution functions ================================================

#define VAR_S_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  var_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

void test_xs_date()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(string_s,xsdate);

  time_s date;
  cassert(clock_s_gettime(CLOCK_REALTIME,&date) == 0);
  DEBUG_PRINT(time_s,&date);

  xs_date_s_create(&xsdate,date);
  fprintf(stderr,"%.*s\n",xsdate.size - 1,xsdate.data);

  time_s date_parsed;
  cassert(xs_date_s_parse(&xsdate,&date_parsed) == 0);
  DEBUG_PRINT(time_s,&date_parsed);

  // - remove time component from date -
  date -= date % (24*60*60*1000000000ULL);

  cassert(date_parsed == date);
#endif
}/*}}}*/

void test_xs_time()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(string_s,xstime);

  time_s time;
  cassert(clock_s_gettime(CLOCK_REALTIME,&time) == 0);
  time -= time % 1000000ULL;

  DEBUG_PRINT(time_s,&time);

  xs_time_s_create(&xstime,time);
  fprintf(stderr,"%.*s\n",xstime.size - 1,xstime.data);

  time_s time_parsed;
  cassert(xs_time_s_parse(&xstime,&time_parsed) == 0);
  DEBUG_PRINT(time_s,&time_parsed);

  // - remove date component from time -
  time_parsed %= 24*60*60*1000000000ULL;
  time %= 24*60*60*1000000000ULL;

  cassert(time_parsed == time);
#endif
}/*}}}*/

void test_xs_datetime()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(string_s,xsdatetime);

  time_s datetime;
  cassert(clock_s_gettime(CLOCK_REALTIME,&datetime) == 0);
  datetime -= datetime % 1000000ULL;

  DEBUG_PRINT(time_s,&datetime);

  xs_dateTime_s_create(&xsdatetime,datetime);
  fprintf(stderr,"%.*s\n",xsdatetime.size - 1,xsdatetime.data);

  time_s datetime_parsed;
  cassert(xs_dateTime_s_parse(&xsdatetime,&datetime_parsed) == 0);
  DEBUG_PRINT(time_s,&datetime_parsed);

  cassert(datetime_parsed == datetime);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libxml_cll_init(),libxml_cll_clear());

  return 0;
}/*}}}*/

