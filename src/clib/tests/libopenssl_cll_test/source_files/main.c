
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libopenssl_cll_test";

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
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(ssl_context_s,server_ctx);
  cassert(ssl_context_s_create_server(&server_ctx) == 0);
  DEBUG_PRINT(ssl_context_s,&server_ctx);

  cassert(ssl_context_s_use_certificate_file(&server_ctx,
        "tests/libopenssl_cll_test/resources/mycert.pem",SSL_FILETYPE_PEM) == 0);
  cassert(ssl_context_s_use_private_key_file(&server_ctx,
        "tests/libopenssl_cll_test/resources/mycert.pem",SSL_FILETYPE_PEM) == 0);

  CONT_INIT_CLEAR(ssl_context_s,client_ctx);
  cassert(ssl_context_s_create_client(&client_ctx) == 0);
  DEBUG_PRINT(ssl_context_s,&client_ctx);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libopenssl_cll_init(),libopenssl_cll_clear());

  return 0;
}/*}}}*/

