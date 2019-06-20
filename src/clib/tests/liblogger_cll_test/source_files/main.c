
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "liblogger_cll_test";

const char *test_names[] =
{/*{{{*/
  "logger",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_logger,
};/*}}}*/

// === test execution functions ================================================

void test_logger()
{/*{{{*/
  CONT_INIT_CLEAR(logger_s,logger);
  cassert(logger_s_add_file(&logger,9,"/dev/shm/log/test.log",4096,3) == 0);
  cassert(logger_s_add_file(&logger,9,"/dev/shm/log/test1.log",4096,3) == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

