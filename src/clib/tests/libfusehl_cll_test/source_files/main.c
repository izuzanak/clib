
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libfusehl_cll_test";

const char *test_names[] =
{/*{{{*/
  "ino_size",
  "create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_ino_size,
  test_create,
};/*}}}*/

// === test execution functions ================================================

void test_ino_size()
{/*{{{*/
  cassert(sizeof(fuse_ino_t) == 8);
}/*}}}*/

void test_create()
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

