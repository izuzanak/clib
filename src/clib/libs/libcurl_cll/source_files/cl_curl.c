
@begin
include "cl_curl.h"
@end

// === methods of structure curl_multi_s =======================================

int curl_multi_s_socket_func(CURL *easy,curl_socket_t socket,int what,void *userp,void *socketp)
{/*{{{*/
  (void)easy;
  (void)socketp;

  unsigned events = 0;

  switch (what)
  {
  case CURL_POLL_IN:
    events = POLLIN;
    break;
  case CURL_POLL_OUT:
    events = POLLOUT;
    break;
  case CURL_POLL_INOUT:
    events = POLLIN | POLLOUT;
    break;
  case CURL_POLL_REMOVE:
    break;
  }

  curl_multi_s *this = (curl_multi_s *)userp;

  if (this->curl_socket_cb(this,what,socket,events))
  {
    return 1;
  }

  return 0;
}/*}}}*/

size_t curl_multi_s_write_buffer_func(void *ptr,size_t size,size_t nmemb,void *stream)
{/*{{{*/
  (void)size;

  bc_array_s *buff_ptr = (bc_array_s *)stream;
  bc_array_s_append(buff_ptr,nmemb,(const char *)ptr);

  return nmemb;
}/*}}}*/

int curl_multi_s_create(curl_multi_s *this,
    curl_socket_cb_t a_curl_socket_cb,curl_response_cb_t a_curl_response_cb,void *a_user_data)
{/*{{{*/
  curl_multi_s_clear(this);

  this->curlm_ptr = curl_multi_init();

  // - ERROR -
  if (this->curlm_ptr == NULL)
  {
    throw_error(CURL_MULTI_CANNOT_CREATE_SESSION);
  }

  curl_multi_setopt(this->curlm_ptr,CURLMOPT_SOCKETFUNCTION,curl_multi_s_socket_func);
  curl_multi_setopt(this->curlm_ptr,CURLMOPT_SOCKETDATA,this);

  // - set curl socket callback -
  this->curl_socket_cb = a_curl_socket_cb;

  // - set curl response callback -
  this->curl_response_cb = a_curl_response_cb;

  // - store user data -
  this->user_data = a_user_data;

  return 0;
}/*}}}*/

int curl_multi_s_GET(curl_multi_s *this,
    const char *a_address,void *a_user_data,unsigned *a_index)
{/*{{{*/

  // - create curl session -
  CURL *curl_ptr = curl_easy_init();

  // - ERROR -
  if (curl_ptr == NULL)
  {
    throw_error(CURL_CANNOT_CREATE_SESSION);
  }

  // - ERROR -
  if (curl_multi_add_handle(this->curlm_ptr,curl_ptr) != CURLM_OK)
  {
    curl_easy_cleanup(curl_ptr);

    throw_error(CURL_MULTI_CANNOT_ADD_HANDLER);
  }

  curl_props_s *curl_props = (curl_props_s *)cmalloc(sizeof(curl_props_s));
  curl_props_s_init(curl_props);

  curl_props->curl_ptr = curl_ptr;

  // - append curl to list -
  *a_index = pointer_list_s_append(&this->curl_list,curl_ptr);
  curl_props->index = *a_index;
  curl_props->unique_id = this->unique_counter++;
  curl_props->user_data = a_user_data;

  // - setup curl easy -
  curl_easy_setopt(curl_ptr,CURLOPT_URL,a_address);
  curl_easy_setopt(curl_ptr,CURLOPT_WRITEFUNCTION,curl_multi_s_write_buffer_func);
  curl_easy_setopt(curl_ptr,CURLOPT_WRITEDATA,&curl_props->write_buffer);
  curl_easy_setopt(curl_ptr,CURLOPT_PRIVATE,curl_props);

  return 0;
}/*}}}*/

int curl_multi_s_response_actions(curl_multi_s *this)
{/*{{{*/
  int msg_cnt;
  CURLMsg *msg = curl_multi_info_read(this->curlm_ptr,&msg_cnt);

  // - if there are any messages in completed transfers queue -
  if (msg != NULL)
  {
    do {
      CURL *curl_ptr = msg->easy_handle;

      // - retrieve curl properties -
      curl_props_s *curl_props;
      curl_easy_getinfo(curl_ptr,CURLINFO_PRIVATE,(char **)&curl_props);

      // - reset private data -
      curl_easy_setopt(curl_ptr,CURLOPT_PRIVATE,NULL);

      CONT_INIT_CLEAR(curl_result_s,result);

      // - set result curl pointer -
      result.curl_ptr = curl_ptr;
      curl_props->curl_ptr = NULL;

      // - set result data -
      bc_array_s_swap(&result.data,&curl_props->write_buffer);

      // - set user data location -
      result.user_data = curl_props->user_data;
      curl_props->user_data = NULL;

      // - remove curl from list -
      pointer_list_s_remove(&this->curl_list,curl_props->index);

      // - release curl properties -
      curl_props_s_clear(curl_props);
      cfree(curl_props);

      // - call response callback -
      if (this->curl_response_cb(&result))
      {
        return 1;
      }

      msg = curl_multi_info_read(this->curlm_ptr,&msg_cnt);
    } while(msg != NULL);
  }

  return 0;
}/*}}}*/

// === methods of structure curl_result_s ======================================

// === global functions ========================================================

void libcurl_cll_init()
{/*{{{*/
  curl_global_init(CURL_GLOBAL_ALL);
}/*}}}*/

void libcurl_cll_clear()
{/*{{{*/
  curl_global_cleanup();
}/*}}}*/

