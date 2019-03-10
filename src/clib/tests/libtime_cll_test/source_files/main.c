
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libtime_cll_test";

const char *test_names[] =
{/*{{{*/
  "time",
  "clock",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_time,
  test_clock,
};/*}}}*/

// === methods of generated structures =========================================

// -- datetime_array_s --
@begin
methods datetime_array_s
@end

// -- time_array_s --
@begin
methods time_array_s
@end

// === test execution functions ================================================

void test_time()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define TIME_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  time_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define DATETIME_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  datetime_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  time_s time_0;
  time_s_now(&time_0);
  TIME_S_TO_BUFFER(&time_0);
  puts(buffer.data);

  datetime_s datetime_0;
  time_s_datetime(&time_0,&datetime_0);
  DATETIME_S_TO_BUFFER(&datetime_0);
  puts(buffer.data);

  time_s time_1;
  cassert(time_s_from_string_ptr(&time_1,"20000101103025") == 0);
  TIME_S_TO_BUFFER(&time_1);
  cassert(strcmp(buffer.data,"2000/01/01 10:30:25") == 0);

  datetime_s datetime_1;
  time_s_datetime(&time_1,&datetime_1);
  DATETIME_S_TO_BUFFER(&datetime_1);
  cassert(strcmp(buffer.data,"{year:2000,month:1,day:1,wday:6,hour:10,min:30,sec:25,msec:0,usec:0,nsec:0}") == 0);

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_clock()
{/*{{{*/
  ulli resolution;
  cassert(clock_s_getres(CLOCK_MONOTONIC,&resolution) == 0);
  printf("resulution: %" HOST_LL_FORMAT "u\n",resolution);

  ulli time;
  cassert(clock_s_gettime(CLOCK_MONOTONIC,&time) == 0);
  printf("time: %" HOST_LL_FORMAT "u\n",time);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

