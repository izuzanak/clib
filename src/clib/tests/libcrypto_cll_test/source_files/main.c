
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libcrypto_cll_test";

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

  // FIXME TODO continue ...
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libcrypto_cll_init(),libcrypto_cll_clear());

  return 0;
}/*}}}*/

