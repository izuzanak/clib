
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libgpsd_cll_test";

const char *test_names[] =
{/*{{{*/
  "gpsd_conn",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_gpsd_conn,
};/*}}}*/

// === test execution functions ================================================

void test_gpsd_conn()
{/*{{{*/
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

