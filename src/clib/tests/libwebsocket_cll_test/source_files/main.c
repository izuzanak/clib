
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

// === methods of generated structures =========================================

// === test execution functions ================================================

int ws_prot_conn_cb(ws_conn_s *a_conn)
{
  (void)a_conn;

  return 0;
}

void test_create()
{/*{{{*/
  CONT_INIT_CLEAR(string_array_s,prot_names);
  string_array_s_push_ptr(&prot_names,"protocol_0");
  string_array_s_push_ptr(&prot_names,"protocol_1");

  CONT_INIT_CLEAR(pointer_array_s,prot_callbacks);
  pointer_array_s_push(&prot_callbacks,ws_prot_conn_cb);
  pointer_array_s_push(&prot_callbacks,ws_prot_conn_cb);

  CONT_INIT_CLEAR(ws_context_s,ctx);
  cassert(ws_context_s_create(&ctx,8888,&prot_names,&prot_callbacks,NULL) == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libwebsocket_cll_init(),libwebsocket_cll_clear());

  return 0;
}/*}}}*/

