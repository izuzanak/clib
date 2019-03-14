
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libchannel_cll_test";

const char *test_names[] =
{/*{{{*/
  "dummy",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_dummy,
};/*}}}*/

// === methods of generated structures =========================================

// === test execution functions ================================================

void test_dummy()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  // FIXME TODO continue ...

  bc_array_s_clear(&buffer);
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

