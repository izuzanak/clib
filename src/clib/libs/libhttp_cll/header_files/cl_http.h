
#ifndef __CL_HTTP_H
#define __CL_HTTP_H

@begin
include "cl_sys.h"
@end

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#include <poll.h>
#include <fcntl.h>
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#include <io.h>
#include <fcntl.h>
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

typedef struct http_server_s http_server_s;
typedef struct http_conn_s http_conn_s;

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

// -- pointer_list_s --
@begin
safe_list<pointer> pointer_list_s;
@end

// === definition of structure http_server_s ===================================

typedef int (*http_connection_cb_t)(http_conn_s *a_conn);

typedef struct http_server_s
{
  struct MHD_Daemon *daemon_ptr;
  http_connection_cb_t connection_cb;
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
void http_server_s_completed_func(void *cls,struct MHD_Connection *connection,
    void **con_cls,enum MHD_RequestTerminationCode toe);

int http_server_s_create(http_server_s *this,usi a_port,http_connection_cb_t a_connection_cb);
int http_server_s_get_fds(http_server_s *this,pollfd_array_s *a_trg);

// === definition of structure http_conn_s =====================================

typedef struct http_conn_s
{
  http_server_s *srv_ptr;

  unsigned conn_type;
  struct MHD_Connection *connection_ptr;
  pointer_array_s *key_value_arr_ptr;
  pointer user_data_ptr;
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

// === inline methods of generated structures ==================================

// -- pointer_list_s --
@begin
inlines pointer_list_s
@end

// === inline methods of structure http_server_s ===============================

static inline void http_server_s_init(http_server_s *this)
{/*{{{*/
  this->daemon_ptr = NULL;
  this->connection_cb = NULL;
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
      MHD_resume_connection(conn_ptr->connection_ptr);
      conn_ptr->suspend_idx = c_idx_not_exist;

      sc_idx = pointer_list_s_next_idx(&this->suspended_conns,sc_idx);
    } while(sc_idx != c_idx_not_exist);
  }

  pointer_list_s_clear(&this->suspended_conns);

  if (this->daemon_ptr != NULL)
  {
    MHD_stop_daemon(this->daemon_ptr);
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

// === inline methods of structure http_conn_s =================================

static inline void http_conn_s_init(http_conn_s *this)
{/*{{{*/
  this->srv_ptr = NULL;

  this->conn_type = c_http_conn_type_NONE;
  this->connection_ptr = NULL;
  this->key_value_arr_ptr = NULL;
  this->user_data_ptr = NULL;
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

#endif

