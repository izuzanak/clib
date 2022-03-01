
#ifndef CL_LIB_CURL_H
#define CL_LIB_CURL_H

@begin
include "cl_struct.h"
@end

#include <curl/curl.h>

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#include <poll.h>
#endif

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libcurl_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libcurl_cll_EXPORTS
#define libcurl_cll_EXPORT __declspec(dllexport)
#else
#define libcurl_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_CURL_CANNOT_CREATE_SESSION 1

#define ERROR_CURL_MULTI_CANNOT_CREATE_SESSION 1
#define ERROR_CURL_MULTI_CANNOT_ADD_HANDLER 2
#define ERROR_CURL_MULTI_SOCKET_ACTION_ERROR 3
#define ERROR_CURL_MULTI_INVALID_REQUEST_INDEX 4

typedef struct curl_multi_s curl_multi_s;
typedef struct curl_props_s curl_props_s;
typedef struct curl_result_s curl_result_s;

// === definition of structure curl_multi_s ====================================

typedef int (*curl_socket_cb_t)(curl_multi_s *a_curl_multi,int a_what,int a_fd,unsigned a_events);
typedef int (*curl_response_cb_t)(curl_multi_s *a_curl_multi,curl_result_s *a_curl_result);

typedef struct curl_multi_s
{
  CURLM *curlm_ptr;
  pointer_list_s curl_list;
  long long int unique_counter;
  curl_socket_cb_t curl_socket_cb;
  curl_response_cb_t curl_response_cb;
  void *user_data;
} curl_multi_s;

@begin
define curl_multi_s dynamic
@end

static inline void curl_multi_s_init(curl_multi_s *this);
static inline void curl_multi_s_clear(curl_multi_s *this);
static inline void curl_multi_s_flush_all(curl_multi_s *this);
static inline void curl_multi_s_swap(curl_multi_s *this,curl_multi_s *a_second);
static inline void curl_multi_s_copy(const curl_multi_s *this,const curl_multi_s *a_src);
static inline int curl_multi_s_compare(const curl_multi_s *this,const curl_multi_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void curl_multi_s_to_string(const curl_multi_s *this,bc_array_s *a_trg);
#endif

int curl_multi_s_socket_func(CURL *easy,curl_socket_t socket,int what,void *userp,void *socketp);
size_t curl_multi_s_write_buffer_func(void *ptr,size_t size,size_t nmemb,void *stream);

WUR libcurl_cll_EXPORT int curl_multi_s_create(curl_multi_s *this,
    curl_socket_cb_t a_curl_socket_cb,curl_response_cb_t a_curl_response_cb,void *a_user_data);

WUR libcurl_cll_EXPORT int curl_multi_s_STUB(curl_multi_s *this,
    void *a_user_data,unsigned *a_index,curl_props_s **a_curl_props);
WUR libcurl_cll_EXPORT int curl_multi_s_GET(curl_multi_s *this,
    const char *a_address,void *a_user_data,unsigned *a_index);
WUR static inline int curl_multi_s_at(curl_multi_s *this,unsigned a_index,CURL **a_curl_ptr);
WUR libcurl_cll_EXPORT int curl_multi_s_cancel(curl_multi_s *this,unsigned a_index);

WUR static inline int curl_multi_s_socket_action(curl_multi_s *this,
    curl_socket_t a_sockfd,int a_events,int *a_running);
WUR libcurl_cll_EXPORT int curl_multi_s_response_actions(curl_multi_s *this);

// === definition of structure curl_props_s ====================================

typedef struct curl_props_s
{
  CURL *curl_ptr;
  struct curl_httppost *form_ptr;
  struct curl_slist *headers;
  unsigned index;
  long long int unique_id;
  bc_array_s write_buffer;
  bc_array_s read_buffer;
  void *user_data;
} curl_props_s;

@begin
define curl_props_s dynamic
@end

static inline void curl_props_s_init(curl_props_s *this);
static inline void curl_props_s_clear(curl_props_s *this);
static inline void curl_props_s_flush_all(curl_props_s *this);
static inline void curl_props_s_swap(curl_props_s *this,curl_props_s *a_second);
static inline void curl_props_s_copy(const curl_props_s *this,const curl_props_s *a_src);
static inline int curl_props_s_compare(const curl_props_s *this,const curl_props_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void curl_props_s_to_string(const curl_props_s *this,bc_array_s *a_trg);
#endif

// === definition of structure curl_result_s ===================================

typedef struct curl_result_s
{
  CURL *curl_ptr;
  struct curl_httppost *form_ptr;
  bc_array_s data;
  void *user_data;
} curl_result_s;

@begin
define curl_result_s dynamic
@end

static inline void curl_result_s_init(curl_result_s *this);
static inline void curl_result_s_clear(curl_result_s *this);
static inline void curl_result_s_flush_all(curl_result_s *this);
static inline void curl_result_s_swap(curl_result_s *this,curl_result_s *a_second);
static inline void curl_result_s_copy(const curl_result_s *this,const curl_result_s *a_src);
static inline int curl_result_s_compare(const curl_result_s *this,const curl_result_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void curl_result_s_to_string(const curl_result_s *this,bc_array_s *a_trg);
#endif

// === definition of global functions ==========================================

void libcurl_cll_init();
void libcurl_cll_clear();

// === inline methods of structure curl_multi_s ================================

static inline void curl_multi_s_init(curl_multi_s *this)
{/*{{{*/
  this->curlm_ptr = NULL;
  pointer_list_s_init(&this->curl_list);
  this->unique_counter = 0;
  this->curl_socket_cb = NULL;
  this->curl_response_cb = NULL;
  this->user_data = NULL;
}/*}}}*/

static inline void curl_multi_s_clear(curl_multi_s *this)
{/*{{{*/

  // - release all running curl transactions -
  if (this->curl_list.first_idx != c_idx_not_exist)
  {
    unsigned curl_idx = this->curl_list.first_idx;
    do {
      CURL *curl_ptr = (CURL *)this->curl_list.data[curl_idx].object;

      // - retrieve curl properties -
      curl_props_s *curl_props;
      curl_easy_getinfo(curl_ptr,CURLINFO_PRIVATE,(char **)&curl_props);

      // - remove curl easy from multi -
      curl_multi_remove_handle(this->curlm_ptr,curl_ptr);

      // - release curl properties -
      curl_props_s_clear(curl_props);
      cfree(curl_props);

      curl_idx = pointer_list_s_next_idx(&this->curl_list,curl_idx);
    } while(curl_idx != c_idx_not_exist);
  }

  pointer_list_s_clear(&this->curl_list);

  if (this->curlm_ptr != NULL)
  {
    curl_multi_cleanup(this->curlm_ptr);
  }

  curl_multi_s_init(this);
}/*}}}*/

static inline void curl_multi_s_flush_all(curl_multi_s *this)
{/*{{{*/
}/*}}}*/

static inline void curl_multi_s_swap(curl_multi_s *this,curl_multi_s *a_second)
{/*{{{*/
  curl_multi_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void curl_multi_s_copy(const curl_multi_s *this,const curl_multi_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int curl_multi_s_compare(const curl_multi_s *this,const curl_multi_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void curl_multi_s_to_string(const curl_multi_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"curl_multi_s{%p}",this);
}/*}}}*/
#endif

static inline int curl_multi_s_at(curl_multi_s *this,unsigned a_index,CURL **a_curl_ptr)
{/*{{{*/

  // - ERROR -
  if (a_index >= this->curl_list.used || !this->curl_list.data[a_index].valid)
  {
    throw_error(CURL_MULTI_INVALID_REQUEST_INDEX);
  }

  *a_curl_ptr = (CURL *)this->curl_list.data[a_index].object;

  return 0;
}/*}}}*/

static inline int curl_multi_s_socket_action(curl_multi_s *this,
    curl_socket_t a_sockfd,int a_events,int *a_running)
{/*{{{*/
  int events = 0;

  if (a_events & (POLLIN | POLLPRI))
  {
    events |= CURL_CSELECT_IN;
  }

  if (a_events & POLLOUT)
  {
    events |= CURL_CSELECT_OUT;
  }

  if (a_events & (POLLERR | POLLHUP | POLLNVAL))
  {
    events |= CURL_CSELECT_ERR;
  }

  // - ERROR -
  if (curl_multi_socket_action(this->curlm_ptr,a_sockfd,events,a_running) != CURLM_OK ||
      curl_multi_s_response_actions(this))
  {
    throw_error(CURL_MULTI_SOCKET_ACTION_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure curl_props_s ================================

static inline void curl_props_s_init(curl_props_s *this)
{/*{{{*/
  this->curl_ptr = NULL;
  this->form_ptr = NULL;
  this->headers = NULL;
  bc_array_s_init(&this->write_buffer);
  bc_array_s_init(&this->read_buffer);
  this->user_data = NULL;
}/*}}}*/

static inline void curl_props_s_clear(curl_props_s *this)
{/*{{{*/
  if (this->curl_ptr != NULL)
  {
    curl_easy_cleanup(this->curl_ptr);
  }

  if (this->form_ptr != NULL)
  {
    curl_formfree(this->form_ptr);
  }

  if (this->headers != NULL)
  {
    curl_slist_free_all(this->headers);
  }

  bc_array_s_clear(&this->write_buffer);
  bc_array_s_clear(&this->read_buffer);

  curl_props_s_init(this);
}/*}}}*/

static inline void curl_props_s_flush_all(curl_props_s *this)
{/*{{{*/
}/*}}}*/

static inline void curl_props_s_swap(curl_props_s *this,curl_props_s *a_second)
{/*{{{*/
  curl_props_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void curl_props_s_copy(const curl_props_s *this,const curl_props_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int curl_props_s_compare(const curl_props_s *this,const curl_props_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void curl_props_s_to_string(const curl_props_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"curl_props_s{%p}",this);
}/*}}}*/
#endif

// === inline methods of structure curl_result_s ===============================

static inline void curl_result_s_init(curl_result_s *this)
{/*{{{*/
  this->curl_ptr = NULL;
  this->form_ptr = NULL;
  bc_array_s_init(&this->data);
  this->user_data = NULL;
}/*}}}*/

static inline void curl_result_s_clear(curl_result_s *this)
{/*{{{*/
  if (this->curl_ptr != NULL)
  {
    curl_easy_cleanup(this->curl_ptr);
  }

  if (this->form_ptr != NULL)
  {
    curl_formfree(this->form_ptr);
  }

  bc_array_s_clear(&this->data);

  curl_result_s_init(this);
}/*}}}*/

static inline void curl_result_s_flush_all(curl_result_s *this)
{/*{{{*/
}/*}}}*/

static inline void curl_result_s_swap(curl_result_s *this,curl_result_s *a_second)
{/*{{{*/
  curl_result_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void curl_result_s_copy(const curl_result_s *this,const curl_result_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int curl_result_s_compare(const curl_result_s *this,const curl_result_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void curl_result_s_to_string(const curl_result_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"curl_result_s{%p}",this);
}/*}}}*/
#endif

#endif

