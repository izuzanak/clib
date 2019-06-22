
@begin
include "cl_curl.h"
@end

// === methods of structure curl_multi_s =======================================

int curl_multi_s_create(curl_multi_s *this,
    curl_socket_cb_t a_curl_socket_cb,curl_response_cb_t a_curl_response_cb)
{/*{{{*/
  curl_multi_s_clear(this);

  this->curlm_ptr = curl_multi_init();

  // - ERROR -
  if (this->curlm_ptr == NULL)
  {
    throw_error(CURL_MULTI_CANNOT_CREATE_SESSION);
  }

  // - set curl socket callback -
  this->curl_socket_cb = a_curl_socket_cb;

  // - set curl response callback -
  this->curl_response_cb = a_curl_response_cb;

  return 0;
}/*}}}*/

// === methods of structure curl_result_s ======================================

