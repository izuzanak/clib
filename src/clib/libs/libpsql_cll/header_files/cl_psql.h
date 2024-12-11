
#ifndef CL_LIB_PSQL_H
#define CL_LIB_PSQL_H

@begin
include "cl_sys.h"
include "cl_linux.h"
include "cl_var.h"
@end

#include <libpq-fe.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libpsql_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libpsql_cll_EXPORTS
#define libpsql_cll_EXPORT __declspec(dllexport)
#else
#define libpsql_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_PSQL_CONN_DB_CONNECT_ERROR 1
#define ERROR_PSQL_CONN_EPOLL_ERROR 2
#define ERROR_PSQL_CONN_SET_NONBLOCK_ERROR 3
#define ERROR_PSQL_CONN_PIPELINE_MODE_ERROR 4
#define ERROR_PSQL_CONN_SEND_FLUSH_ERROR 5
#define ERROR_PSQL_CONN_SEND_QUERY_ERROR 6
#define ERROR_PSQL_CONN_CONSUME_INPUT_ERROR 7
#define ERROR_PSQL_CONN_FLUSH_ERROR 8
#define ERROR_PSQL_CONN_PROCESS_ERROR 9
#define ERROR_PSQL_CONN_CALLBACK_ERROR 10

// - postgresql oids -
#define BOOLOID 16

#define INT8OID 20
#define INT2OID 21
#define INT4OID 23

#define FLOAT4OID 700
#define FLOAT8OID 701

typedef struct psql_conn_s psql_conn_s;
typedef struct psql_result_s psql_result_s;
typedef struct psql_notify_s psql_notify_s;

// === definition of structure psql_conn_s =======================================

WUR typedef int (*psql_result_cb_t)(void *a_object,unsigned a_index,psql_result_s *a_result);
WUR typedef int (*psql_notify_cb_t)(void *a_object,unsigned a_index,psql_notify_s *a_notify);

typedef struct psql_conn_s
{
  PGconn *conn_ptr;
  int last_blank;
  psql_result_cb_t result_cb;
  psql_notify_cb_t notify_cb;
  void *cb_object;
  unsigned cb_index;
  epoll_borrow_fd_s epoll_fd;
} psql_conn_s;

@begin
define psql_conn_s dynamic
@end

static inline void psql_conn_s_init(psql_conn_s *this);
static inline void psql_conn_s_clear(psql_conn_s *this);
static inline void psql_conn_s_flush_all(psql_conn_s *this);
static inline void psql_conn_s_swap(psql_conn_s *this,psql_conn_s *a_second);
static inline void psql_conn_s_copy(const psql_conn_s *this,const psql_conn_s *a_src);
static inline int psql_conn_s_compare(const psql_conn_s *this,const psql_conn_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void psql_conn_s_to_string(const psql_conn_s *this,bc_array_s *a_trg);
#endif

WUR libpsql_cll_EXPORT int psql_conn_s_create(psql_conn_s *this,
    const char *a_conninfo,
    psql_result_cb_t a_result_cb,
    psql_notify_cb_t a_notify_cb,
    void *a_cb_object,
    unsigned a_cb_index);
static inline int psql_conn_s_nonblocking(psql_conn_s *this,int a_enable);
#ifdef LIBPQ_HAS_PIPELINING
static inline int psql_conn_s_pipeline_mode(psql_conn_s *this,int a_enable);
static inline int psql_conn_s_flush(psql_conn_s *this);
#endif
static inline int psql_conn_s_send_query(psql_conn_s *this,const char *a_query);
WUR libpsql_cll_EXPORT int psql_conn_s_process(psql_conn_s *this);
WUR libpsql_cll_EXPORT int psql_conn_s_fd_event(void *a_psql_conn,unsigned a_index,epoll_event_s *a_epoll_event);

// === definition of structure psql_result_s ===================================

typedef struct psql_result_s
{
  PGresult *res_ptr;
} psql_result_s;

@begin
define psql_result_s dynamic
@end

static inline void psql_result_s_init(psql_result_s *this);
static inline void psql_result_s_clear(psql_result_s *this);
static inline void psql_result_s_flush_all(psql_result_s *this);
static inline void psql_result_s_swap(psql_result_s *this,psql_result_s *a_second);
static inline void psql_result_s_copy(const psql_result_s *this,const psql_result_s *a_src);
static inline int psql_result_s_compare(const psql_result_s *this,const psql_result_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void psql_result_s_to_string(const psql_result_s *this,bc_array_s *a_trg);
#endif

WUR libpsql_cll_EXPORT int psql_result_s_tuples(psql_result_s *this,var_arrays_s *a_tuples);

// === definition of structure psql_notify_s ===================================

typedef struct psql_notify_s
{
  PGnotify *ntf_ptr;
} psql_notify_s;

@begin
define psql_notify_s dynamic
@end

static inline void psql_notify_s_init(psql_notify_s *this);
static inline void psql_notify_s_clear(psql_notify_s *this);
static inline void psql_notify_s_flush_all(psql_notify_s *this);
static inline void psql_notify_s_swap(psql_notify_s *this,psql_notify_s *a_second);
static inline void psql_notify_s_copy(const psql_notify_s *this,const psql_notify_s *a_src);
static inline int psql_notify_s_compare(const psql_notify_s *this,const psql_notify_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void psql_notify_s_to_string(const psql_notify_s *this,bc_array_s *a_trg);
#endif

static inline const char *psql_notify_s_channel(const psql_notify_s *this);
static inline int psql_notify_s_pid(const psql_notify_s *this);
static inline const char *psql_notify_s_payload(const psql_notify_s *this);

// === inline methods of structure psql_conn_s ===================================

static inline void psql_conn_s_init(psql_conn_s *this)
{/*{{{*/
  this->conn_ptr = NULL;
  this->last_blank = 1;
  epoll_borrow_fd_s_init(&this->epoll_fd);
}/*}}}*/

static inline void psql_conn_s_clear(psql_conn_s *this)
{/*{{{*/
  epoll_borrow_fd_s_clear(&this->epoll_fd);

  if (this->conn_ptr != NULL)
  {
    PQfinish(this->conn_ptr);
  }

  psql_conn_s_init(this);
}/*}}}*/

static inline void psql_conn_s_flush_all(psql_conn_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void psql_conn_s_swap(psql_conn_s *this,psql_conn_s *a_second)
{/*{{{*/
  psql_conn_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void psql_conn_s_copy(const psql_conn_s *this,const psql_conn_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int psql_conn_s_compare(const psql_conn_s *this,const psql_conn_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void psql_conn_s_to_string(const psql_conn_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"psql_conn_s{%p}",this);
}/*}}}*/
#endif

static inline int psql_conn_s_nonblocking(psql_conn_s *this,int a_enable)
{/*{{{*/

  // - ERROR -
  if (PQsetnonblocking(this->conn_ptr,a_enable) != 0)
  {
    throw_error(PSQL_CONN_SET_NONBLOCK_ERROR);
  }

  return 0;
}/*}}}*/

#ifdef LIBPQ_HAS_PIPELINING
static inline int psql_conn_s_pipeline_mode(psql_conn_s *this,int a_enable)
{/*{{{*/

  // - ERROR -
  if ((a_enable ? PQenterPipelineMode(this->conn_ptr) : PQexitPipelineMode(this->conn_ptr)) != 1)
  {
    throw_error(PSQL_CONN_PIPELINE_MODE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int psql_conn_s_flush(psql_conn_s *this)
{/*{{{*/

  // - ERROR -
  if (PQsendFlushRequest(this->conn_ptr) != 1)
  {
    throw_error(PSQL_CONN_SEND_FLUSH_ERROR);
  }

  if (epoll_fd_s_modify_events(&this->epoll_fd,EPOLLIN | EPOLLPRI | EPOLLOUT))
  {
    throw_error(PSQL_CONN_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/
#endif

static inline int psql_conn_s_send_query(psql_conn_s *this,const char *a_query)
{/*{{{*/

  // - ERROR -
  if (PQsendQueryParams(this->conn_ptr,a_query,0,NULL,NULL,NULL,NULL,0) != 1)
  {
    //PQerrorMessage(conn_ptr);
    throw_error(PSQL_CONN_SEND_QUERY_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure psql_result_s ===============================

static inline void psql_result_s_init(psql_result_s *this)
{/*{{{*/
  this->res_ptr = NULL;
}/*}}}*/

static inline void psql_result_s_clear(psql_result_s *this)
{/*{{{*/
  if (this->res_ptr != NULL)
  {
    PQclear(this->res_ptr);
  }

  psql_result_s_init(this);
}/*}}}*/

static inline void psql_result_s_flush_all(psql_result_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void psql_result_s_swap(psql_result_s *this,psql_result_s *a_second)
{/*{{{*/
  psql_result_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void psql_result_s_copy(const psql_result_s *this,const psql_result_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int psql_result_s_compare(const psql_result_s *this,const psql_result_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void psql_result_s_to_string(const psql_result_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"psql_result_s{%p}",this);
}/*}}}*/
#endif

// === inline methods of structure psql_notify_s ===============================

static inline void psql_notify_s_init(psql_notify_s *this)
{/*{{{*/
  this->ntf_ptr = NULL;
}/*}}}*/

static inline void psql_notify_s_clear(psql_notify_s *this)
{/*{{{*/
  if (this->ntf_ptr != NULL)
  {
    PQfreemem(this->ntf_ptr);
  }

  psql_notify_s_init(this);
}/*}}}*/

static inline void psql_notify_s_flush_all(psql_notify_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void psql_notify_s_swap(psql_notify_s *this,psql_notify_s *a_second)
{/*{{{*/
  psql_notify_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void psql_notify_s_copy(const psql_notify_s *this,const psql_notify_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int psql_notify_s_compare(const psql_notify_s *this,const psql_notify_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void psql_notify_s_to_string(const psql_notify_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"psql_notify_s{%p}",this);
}/*}}}*/
#endif

static inline const char *psql_notify_s_channel(const psql_notify_s *this)
{/*{{{*/
  return this->ntf_ptr->relname;
}/*}}}*/

static inline int psql_notify_s_pid(const psql_notify_s *this)
{/*{{{*/
  return this->ntf_ptr->be_pid;
}/*}}}*/

static inline const char *psql_notify_s_payload(const psql_notify_s *this)
{/*{{{*/
  return this->ntf_ptr->extra;
}/*}}}*/

#endif

