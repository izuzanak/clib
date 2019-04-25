
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libfuse_cll_test";

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
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  struct fuse_lowlevel_ops fuse_oper = {};

  CONT_INIT_CLEAR(string_array_s,args);
  string_array_s_push_ptr(&args,"create");

  CONT_INIT_CLEAR(fuse_session_s,session);
  cassert(fuse_session_s_create(&session,&args,&fuse_oper,NULL) == 0);
  cassert(fuse_session_s_mount(&session,"tests/libfuse_cll_test/create") == 0);

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

