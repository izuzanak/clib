
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "librtsp_cll_test";

const char *test_names[] =
{/*{{{*/
  "rtsp",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_rtsp,
};/*}}}*/

// === test execution functions ================================================

void test_rtsp()
{/*{{{*/

  // FIXME TODO continue ...
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

