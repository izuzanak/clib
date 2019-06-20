
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
  logger_s_create(&logger,"test");

  cassert(logger_s_add_file(&logger,1,"tests/liblogger_cll_test/logger/test.log",4096,3) == 0);
  cassert(logger_s_add_file(&logger,2,"tests/liblogger_cll_test/logger/test1.log",4096,3) == 0);
  cassert(logger_s_add_file(&logger,3,"tests/liblogger_cll_test/logger/test2.log",4096,3) == 0);

  unsigned idx = 0;
  do {
    cassert(logger_s_write(&logger,2,"Hello logger #%u ...",idx) == 0);
  } while(++idx < 100);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

