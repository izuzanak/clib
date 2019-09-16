
#ifndef __CL_HTTP_H
#define __CL_HTTP_H

@begin
include "cl_sys.h"
@end

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#include <fcntl.h>
#include <poll.h>
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#endif

#include <microhttpd.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libhttp_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libhttp_cll_EXPORTS
#define libhttp_cll_EXPORT __declspec(dllexport)
#else
#define libhttp_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_HTTP_SERVER_CANNOT_START_DAEMON 1
#define ERROR_HTTP_SERVER_INTERNAL_ERROR 2

#define ERROR_HTTP_CONN_CANNOT_QUEUE_RESPONSE 1

#define ERROR_HTTP_RESP_CREATE_ERROR 1

typedef struct http_server_s http_server_s;
typedef struct http_conn_s http_conn_s;
typedef struct http_resp_s http_resp_s;

// - connection types -
enum
{
  c_http_conn_type_NONE = 0,
  c_http_conn_type_CONNECT,
  c_http_conn_type_DELETE,
  c_http_conn_type_GET,
  c_http_conn_type_HEAD,
  c_http_conn_type_OPTIONS,
  c_http_conn_type_POST,
  c_http_conn_type_PUT,
  c_http_conn_type_TRACE,
};

// === definition of generated structures ======================================

// -- http_key_value_s --
@begin
struct
<
string_s:key
string_s:value
>
http_key_value_s;
@end

// -- http_key_value_tree_s --
@begin
rb_tree<http_key_value_s> http_key_value_tree_s;
@end

// === definition of structure http_server_s ===================================

typedef int (*http_connection_cb_t)(http_conn_s *a_conn);
typedef int (*http_completed_cb_t)(http_conn_s *a_conn);

typedef struct http_server_s
{
  struct MHD_Daemon *daemon;
  http_connection_cb_t connection_cb;
  http_completed_cb_t completed_cb;
  void *user_data;
  pointer_list_s suspended_conns;

  int ret_code;
} http_server_s;

@begin
define http_server_s dynamic
@end

static inline void http_server_s_init(http_server_s *this);
static inline void http_server_s_clear(http_server_s *this);
static inline void http_server_s_flush_all(http_server_s *this);
static inline void http_server_s_swap(http_server_s *this,http_server_s *a_second);
static inline void http_server_s_copy(const http_server_s *this,const http_server_s *a_src);
static inline int http_server_s_compare(const http_server_s *this,const http_server_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void http_server_s_to_string(const http_server_s *this,bc_array_s *a_trg);
#endif

int http_server_s_connection_func(void *cls,struct MHD_Connection *connection,
    const char *url,const char *method,const char *version,
    const char *upload_data,size_t *upload_data_size,void **con_cls);
int conn_key_value_func(void *cls,enum MHD_ValueKind kind,
    const char *key,const char *value);
void http_server_s_completed_func(void *cls,struct MHD_Connection *connection,
    void **con_cls,enum MHD_RequestTerminationCode toe);

WUR libhttp_cll_EXPORT int http_server_s_create(http_server_s *this,usi a_port,
    http_connection_cb_t a_connection_cb,
    http_completed_cb_t a_completed_cb,
    void *a_user_data);
WUR libhttp_cll_EXPORT int http_server_s_fds(http_server_s *this,pollfd_array_s *a_trg);
static inline ulli http_server_s_timeout(http_server_s *this);
WUR static inline int http_server_s_process(http_server_s *this);

// === definition of structure http_conn_s =====================================

typedef struct http_conn_s
{
  http_server_s *server;

  unsigned conn_type;
  struct MHD_Connection *connection;
  http_key_value_tree_s *http_key_value_tree;
  void *user_data;
  unsigned suspend_idx;

  const char *url;
  const char *method;
  const char *version;
  const char *upload_data;
  size_t *upload_data_size;
} http_conn_s;

@begin
define http_conn_s dynamic
@end

static inline void http_conn_s_init(http_conn_s *this);
static inline void http_conn_s_clear(http_conn_s *this);
static inline void http_conn_s_flush_all(http_conn_s *this);
static inline void http_conn_s_swap(http_conn_s *this,http_conn_s *a_second);
static inline void http_conn_s_copy(const http_conn_s *this,const http_conn_s *a_src);
static inline int http_conn_s_compare(const http_conn_s *this,const http_conn_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void http_conn_s_to_string(const http_conn_s *this,bc_array_s *a_trg);
#endif

void http_conn_s_values(http_conn_s *this,enum MHD_ValueKind a_value_kind,http_key_value_tree_s *a_trg);
WUR libhttp_cll_EXPORT int http_conn_s_queue_response(http_conn_s *this,
    unsigned a_status_code,http_resp_s *a_resp);

// === definition of structure http_resp_s =====================================

typedef struct http_resp_s
{
  struct MHD_Response *response;
} http_resp_s;

@begin
define http_resp_s dynamic
@end

static inline void http_resp_s_init(http_resp_s *this);
static inline void http_resp_s_clear(http_resp_s *this);
static inline void http_resp_s_flush_all(http_resp_s *this);
static inline void http_resp_s_swap(http_resp_s *this,http_resp_s *a_second);
static inline void http_resp_s_copy(const http_resp_s *this,const http_resp_s *a_src);
static inline int http_resp_s_compare(const http_resp_s *this,const http_resp_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void http_resp_s_to_string(const http_resp_s *this,bc_array_s *a_trg);
#endif

static inline int http_resp_s_create_from_buffer(http_resp_s *this,
    size_t a_size,void *a_data,enum MHD_ResponseMemoryMode a_mode);
static inline int http_resp_s_create_from_fd(http_resp_s *this,uint64_t a_size,int a_fd);
static inline int http_resp_s_create_from_callback(http_resp_s *this,uint64_t a_size,size_t a_block_size,
    MHD_ContentReaderCallback a_crc,void *a_crc_cls,MHD_ContentReaderFreeCallback a_crfc);

// === inline methods of generated structures ==================================

// -- http_key_value_s --
@begin
inlines http_key_value_s
@end

// -- http_key_value_tree_s --
@begin
inlines http_key_value_tree_s
@end

static inline int http_key_value_tree_s___compare_value(const http_key_value_tree_s *this,const http_key_value_s *a_first,const http_key_value_s *a_second)
{/*{{{*/
  (void)this;

  if (a_first->key.size < a_second->key.size) { return -1; }
  if (a_first->key.size > a_second->key.size) { return 1; }

  return memcmp(a_first->key.data,a_second->key.data,a_first->key.size - 1);
}/*}}}*/

// === inline methods of structure http_server_s ===============================

static inline void http_server_s_init(http_server_s *this)
{/*{{{*/
  this->daemon = NULL;
  this->connection_cb = NULL;
  this->user_data = NULL;
  pointer_list_s_init(&this->suspended_conns);
  this->ret_code = 0;
}/*}}}*/

static inline void http_server_s_clear(http_server_s *this)
{/*{{{*/

  // - resume suspended connections -
  if (this->suspended_conns.count != 0)
  {
    unsigned sc_idx = this->suspended_conns.first_idx;
    do {

      // - retrieve suspended connection -
      http_conn_s *conn_ptr = (http_conn_s *)pointer_list_s_at(&this->suspended_conns,sc_idx);

      // - resume suspended connection -
      MHD_resume_connection(conn_ptr->connection);
      conn_ptr->suspend_idx = c_idx_not_exist;

      sc_idx = pointer_list_s_next_idx(&this->suspended_conns,sc_idx);
    } while(sc_idx != c_idx_not_exist);
  }

  pointer_list_s_clear(&this->suspended_conns);

  if (this->daemon != NULL)
  {
    MHD_stop_daemon(this->daemon);
  }

  http_server_s_init(this);
}/*}}}*/

static inline void http_server_s_flush_all(http_server_s *this)
{/*{{{*/
}/*}}}*/

static inline void http_server_s_swap(http_server_s *this,http_server_s *a_second)
{/*{{{*/
  http_server_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void http_server_s_copy(const http_server_s *this,const http_server_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int http_server_s_compare(const http_server_s *this,const http_server_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void http_server_s_to_string(const http_server_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"http_server_s{%p}",this);
}/*}}}*/
#endif

static inline ulli http_server_s_timeout(http_server_s *this)
{/*{{{*/
  MHD_UNSIGNED_LONG_LONG mhd_timeout;

  // - retrieve mhd timeout -
  if (MHD_get_timeout(this->daemon,&mhd_timeout) == MHD_YES)
  {
    return mhd_timeout*1000000ULL;
  }

  return 0;
}/*}}}*/

static inline int http_server_s_process(http_server_s *this)
{/*{{{*/

  // - reset return code -
  this->ret_code = 0;

  // - ERROR -
  if (MHD_run(this->daemon) != MHD_YES)
  {
    throw_error(HTTP_SERVER_INTERNAL_ERROR);
  }

  // - ERROR -
  if (this->ret_code)
  {
    return this->ret_code;
  }

  return 0;
}/*}}}*/

// === inline methods of structure http_conn_s =================================

static inline void http_conn_s_init(http_conn_s *this)
{/*{{{*/
  this->server = NULL;

  this->conn_type = c_http_conn_type_NONE;
  this->connection = NULL;
  this->http_key_value_tree = NULL;
  this->user_data = NULL;
  this->suspend_idx = c_idx_not_exist;

  this->url = NULL;
  this->method = NULL;
  this->version = NULL;
  this->upload_data = NULL;
  this->upload_data_size = NULL;
}/*}}}*/

static inline void http_conn_s_clear(http_conn_s *this)
{/*{{{*/
  http_conn_s_init(this);
}/*}}}*/

static inline void http_conn_s_flush_all(http_conn_s *this)
{/*{{{*/
}/*}}}*/

static inline void http_conn_s_swap(http_conn_s *this,http_conn_s *a_second)
{/*{{{*/
  http_conn_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void http_conn_s_copy(const http_conn_s *this,const http_conn_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int http_conn_s_compare(const http_conn_s *this,const http_conn_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void http_conn_s_to_string(const http_conn_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"http_conn_s{%p}",this);
}/*}}}*/
#endif

// === inline methods of structure http_resp_s =================================

static inline void http_resp_s_init(http_resp_s *this)
{/*{{{*/
  this->response = NULL;
}/*}}}*/

static inline void http_resp_s_clear(http_resp_s *this)
{/*{{{*/
  if (this->response != NULL)
  {
    MHD_destroy_response(this->response);
  }

  http_resp_s_init(this);
}/*}}}*/

static inline void http_resp_s_flush_all(http_resp_s *this)
{/*{{{*/
}/*}}}*/

static inline void http_resp_s_swap(http_resp_s *this,http_resp_s *a_second)
{/*{{{*/
  http_resp_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void http_resp_s_copy(const http_resp_s *this,const http_resp_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int http_resp_s_compare(const http_resp_s *this,const http_resp_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void http_resp_s_to_string(const http_resp_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"http_resp_s{%p}",this);
}/*}}}*/
#endif

static inline int http_resp_s_create_from_buffer(http_resp_s *this,
    size_t a_size,void *a_data,enum MHD_ResponseMemoryMode a_mode)
{/*{{{*/
  http_resp_s_clear(this);

  // - ERROR -
  if ((this->response = MHD_create_response_from_buffer(a_size,a_data,a_mode)) == NULL)
  {
    throw_error(HTTP_RESP_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int http_resp_s_create_from_fd(http_resp_s *this,uint64_t a_size,int a_fd)
{/*{{{*/
  http_resp_s_clear(this);

  // - ERROR -
  if ((this->response = MHD_create_response_from_fd(a_size,a_fd)) == NULL)
  {
    throw_error(HTTP_RESP_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int http_resp_s_create_from_callback(http_resp_s *this,uint64_t a_size,size_t a_block_size,
    MHD_ContentReaderCallback a_crc,void *a_crc_cls,MHD_ContentReaderFreeCallback a_crfc)
{/*{{{*/
  http_resp_s_clear(this);

  // - ERROR -
  if ((this->response = MHD_create_response_from_callback(
          a_size,a_block_size,a_crc,a_crc_cls,a_crfc)) == NULL)
  {
    throw_error(HTTP_RESP_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

#endif

