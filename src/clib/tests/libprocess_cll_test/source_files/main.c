
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libprocess_cll_test";

const char *test_names[] =
{/*{{{*/
  "dummy",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_dummy,
};/*}}}*/

// === test execution functions ================================================

void test_dummy()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(process_s,process);
  cassert(process_s_create(&process,"libprocess_cll_test") == 0);
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

