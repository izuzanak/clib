
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libcurl_cll_test";

const char *test_names[] =
{/*{{{*/
  "dummy",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_dummy,
};/*}}}*/

// === test execution functions ================================================

int curl_socket_cb(curl_multi_s *a_curl_multi)
{/*{{{*/
  return 0;
}/*}}}*/

int curl_response_cb(curl_result_s *a_curl_result)
{/*{{{*/
  return 0;
}/*}}}*/

void test_dummy()
{/*{{{*/
  CONT_INIT_CLEAR(curl_multi_s,curl);
  cassert(curl_multi_s_create(&curl,curl_socket_cb,curl_response_cb) == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

