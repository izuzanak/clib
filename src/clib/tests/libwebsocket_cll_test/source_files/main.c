
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libwebsocket_cll_test";

const char *test_names[] =
{/*{{{*/
  "create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_create,
};/*}}}*/

// === test execution functions ================================================

void test_create()
{/*{{{*/

  // TODO (devel) continue ...
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libwebsocket_cll_init(),libwebsocket_cll_clear());

  return 0;
}/*}}}*/

