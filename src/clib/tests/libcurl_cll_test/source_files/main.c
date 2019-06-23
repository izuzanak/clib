
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libcurl_cll_test";

const char *test_names[] =
{/*{{{*/
  "create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_create,
};/*}}}*/

// === test execution functions ================================================

int curl_socket_cb(curl_multi_s *a_curl_multi,int a_what,int a_fd,unsigned a_events)
{/*{{{*/
  debug_message_6(fprintf(stderr,"curl_socket_cb\n"));

  // FIXME TODO continue ...

  return 0;
}/*}}}*/

int curl_response_cb(curl_result_s *a_curl_result)
{/*{{{*/
  debug_message_6(fprintf(stderr,"curl_response_cb\n"));

  // FIXME TODO continue ...

  return 0;
}/*}}}*/

void test_create()
{/*{{{*/
  CONT_INIT_CLEAR(curl_multi_s,curl);

  // FIXME TODO continue ...
  cassert(curl_multi_s_create(&curl,curl_socket_cb,curl_response_cb) == 0);

  cassert(curl_multi_s_GET(&curl,"http://127.0.0.1",NULL) == 0);
  cassert(curl_multi_s_GET(&curl,"http://127.0.0.1",NULL) == 0);
  cassert(curl_multi_s_GET(&curl,"http://127.0.0.1",NULL) == 0);

  int running;
  cassert(curl_multi_s_socket_action(&curl,CURL_SOCKET_TIMEOUT,0,&running) == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libcurl_cll_init(),libcurl_cll_clear());

  return 0;
}/*}}}*/

