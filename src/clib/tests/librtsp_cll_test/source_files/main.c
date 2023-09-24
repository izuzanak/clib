
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "librtsp_cll_test";

const char *test_names[] =
{/*{{{*/
  "rtsp_parse_range_time",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_rtsp_parse_range_time,
};/*}}}*/

// === test execution functions ================================================

void test_rtsp_parse_range_time()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

#define RTSP_RANGE_TO_BUFFER(TIME) \
{/*{{{*/\
  buffer.used = 0;\
  time_s_to_string(&(TIME),&buffer);\
  bc_array_s_append_format(&buffer,".%9.9llu",(TIME) % 1000000000ULL);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  time_s time;
  char *range = NULL;

  range = "20211213T123508Z-";
  cassert(rtsp_conn_s_parse_range_time(&range,&time) == 0);
  RTSP_RANGE_TO_BUFFER(time);
  cassert(strcmp(buffer.data,"2021/12/13 12:35:08.000000000") == 0);

  range = "20211213T123508.480Z-";
  cassert(rtsp_conn_s_parse_range_time(&range,&time) == 0);
  RTSP_RANGE_TO_BUFFER(time);
  cassert(strcmp(buffer.data,"2021/12/13 12:35:08.480000000") == 0);

  range = "20211213T123508.480480Z-";
  cassert(rtsp_conn_s_parse_range_time(&range,&time) == 0);
  RTSP_RANGE_TO_BUFFER(time);
  cassert(strcmp(buffer.data,"2021/12/13 12:35:08.480480000") == 0);

  range = "20211213T123508.480480480Z-";
  cassert(rtsp_conn_s_parse_range_time(&range,&time) == 0);
  RTSP_RANGE_TO_BUFFER(time);
  cassert(strcmp(buffer.data,"2021/12/13 12:35:08.480480480") == 0);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

