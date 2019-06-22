
#ifndef __CL_CURL_H
#define __CL_CURL_H

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
#define ERROR_CURL_MULTI_CANNOT_CREATE_SESSION 1

typedef struct curl_multi_s curl_multi_s;
typedef struct curl_result_s curl_result_s;

// === definition of structure curl_multi_s ====================================

typedef int (*curl_socket_cb_t)(curl_multi_s *a_curl_multi);
typedef int (*curl_response_cb_t)(curl_result_s *a_curl_result);

typedef struct curl_multi_s
{
  CURLM *curlm_ptr;
  curl_socket_cb_t curl_socket_cb;
  curl_response_cb_t curl_response_cb;
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

WUR libcurl_cll_EXPORT int curl_multi_s_create(curl_multi_s *this,
    curl_socket_cb_t a_curl_socket_cb,curl_response_cb_t a_curl_response_cb);

// === definition of structure curl_result_s ===================================

typedef struct curl_result_s
{
  CURL *curl_ptr;
  bc_array_s data;
  void *user_ptr;
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

// === inline methods of structure curl_multi_s ================================

static inline void curl_multi_s_init(curl_multi_s *this)
{/*{{{*/
  this->curlm_ptr = NULL;
  this->curl_socket_cb = NULL;
  this->curl_response_cb = NULL;
}/*}}}*/

static inline void curl_multi_s_clear(curl_multi_s *this)
{/*{{{*/

  // FIXME TODO: release running curl transactions ...

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

// === inline methods of structure curl_result_s ===============================

static inline void curl_result_s_init(curl_result_s *this)
{/*{{{*/
  this->curl_ptr = NULL;
  bc_array_s_init(&this->data);
  this->user_ptr = NULL;
}/*}}}*/

static inline void curl_result_s_clear(curl_result_s *this)
{/*{{{*/
  if (this->curl_ptr != NULL)
  {
    curl_easy_cleanup(&this->curl_ptr);
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

