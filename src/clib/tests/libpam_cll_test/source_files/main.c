
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libpam_cll_test";

const char *test_names[] =
{/*{{{*/
  "pam",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_pam,
};/*}}}*/

// === test execution functions ================================================

void test_pam()
{/*{{{*/
#ifndef DISABLE_PAM_AUTH
  cassert(pam_user_auth("test","test","invalid"));
#endif

  //cassert(pam_user_auth("test","test","password") == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

