
#ifndef CL_LIB_WEBSOCKETS_H
#define CL_LIB_WEBSOCKETS_H

@begin
include "cl_struct.h"
@end

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#include <poll.h>
#endif

#include <libwebsockets.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libwebsockets_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libwebsockets_cll_EXPORTS
#define libwebsockets_cll_EXPORT __declspec(dllexport)
#else
#define libwebsockets_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_WS_CONTEXT_CREATE_MISMATCH_PROTOCOL_ARRAYS_SIZE 1
#define ERROR_WS_CONTEXT_CREATE_EMPTY_PROTOCOL_ARRAYS 2
#define ERROR_WS_CONTEXT_CREATE_ERROR 3
#define ERROR_WS_CONTEXT_SERVICE_FD_ERROR 4

#define ERROR_WS_CONN_WRITE_ERROR 1

#define ERROR_WS_CLIENT_CREATE_ERROR 1

typedef struct ws_context_s ws_context_s;
typedef struct ws_conn_s ws_conn_s;
typedef struct ws_client_s ws_client_s;

// === definition of structure ws_context_s ====================================

typedef int (*ws_fd_event_cb_t)(ws_context_s *a_ws_context,
    enum lws_callback_reasons a_reason,int a_fd,unsigned a_events);
typedef int (*ws_prot_conn_cb_t)(ws_conn_s *a_conn);

typedef struct ws_context_s
{
  struct lws_context *context;
  struct lws_protocols *protocols;
  void *user_data;

  string_array_s prot_names;
  pointer_array_s prot_callbacks;
  ws_fd_event_cb_t ws_fd_event_cb;

  unsigned ret_code;
} ws_context_s;

@begin
define ws_context_s dynamic
@end

static inline void ws_context_s_init(ws_context_s *this);
static inline void ws_context_s_clear(ws_context_s *this);
static inline void ws_context_s_flush_all(ws_context_s *this);
static inline void ws_context_s_swap(ws_context_s *this,ws_context_s *a_second);
static inline void ws_context_s_copy(const ws_context_s *this,const ws_context_s *a_src);
static inline int ws_context_s_compare(const ws_context_s *this,const ws_context_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void ws_context_s_to_string(const ws_context_s *this,bc_array_s *a_trg);
#endif

void ws_context_s_log_emit(int level,const char *line);
int ws_context_s_http_func(struct lws *wsi,enum lws_callback_reasons reason,void *user,void *in,size_t len);
int ws_context_s_protocol_func(struct lws *wsi,enum lws_callback_reasons reason,void *user,void *in,size_t len);

WUR libwebsockets_cll_EXPORT int ws_context_s_create(ws_context_s *this,
    usi a_port,string_array_s *a_prot_names,pointer_array_s *a_prot_callbacks,
    ws_fd_event_cb_t a_ws_fd_event_cb,void *a_user_data);
WUR static inline int ws_context_s_process_fd(ws_context_s *this,struct pollfd *a_pollfd);

static inline unsigned ws_context_s_get_protocol_idx(ws_context_s *this,struct lws *a_wsi);

// === definition of structure ws_conn_s =======================================

typedef struct ws_conn_s
{
  ws_context_s *wsc_ptr;
  unsigned prot_idx;
  struct lws *ws_ptr;
  enum lws_callback_reasons reason;
  void *user_data;
  bc_array_s message_buffer;
  bc_array_s data_buffer;
  void *data_in;
  size_t data_len;
} ws_conn_s;

@begin
define ws_conn_s dynamic
@end

static inline void ws_conn_s_init(ws_conn_s *this);
static inline void ws_conn_s_clear(ws_conn_s *this);
static inline void ws_conn_s_flush_all(ws_conn_s *this);
static inline void ws_conn_s_swap(ws_conn_s *this,ws_conn_s *a_second);
static inline void ws_conn_s_copy(const ws_conn_s *this,const ws_conn_s *a_src);
static inline int ws_conn_s_compare(const ws_conn_s *this,const ws_conn_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void ws_conn_s_to_string(const ws_conn_s *this,bc_array_s *a_trg);
#endif

static inline void **ws_conn_s_ctx_user_data(ws_conn_s *this);
static inline const char *ws_conn_s_protocol_name(ws_conn_s *this);
static inline void ws_conn_s_callback_on_writable(ws_conn_s *this);
static inline void ws_conn_s_set_timeout(ws_conn_s *this,enum pending_timeout a_reason,int a_seconds);
WUR libwebsockets_cll_EXPORT int ws_conn_s_write(ws_conn_s *this,
    const char *a_data,size_t a_size,enum lws_write_protocol a_protocol);

// === definition of structure ws_client_s =====================================

typedef struct ws_client_s
{
  ws_context_s *wsc_ptr;
  struct lws *ws_ptr;
} ws_client_s;

@begin
define ws_client_s dynamic
@end

static inline void ws_client_s_init(ws_client_s *this);
static inline void ws_client_s_clear(ws_client_s *this);
static inline void ws_client_s_flush_all(ws_client_s *this);
static inline void ws_client_s_swap(ws_client_s *this,ws_client_s *a_second);
static inline void ws_client_s_copy(const ws_client_s *this,const ws_client_s *a_src);
static inline int ws_client_s_compare(const ws_client_s *this,const ws_client_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void ws_client_s_to_string(const ws_client_s *this,bc_array_s *a_trg);
#endif

WUR libwebsockets_cll_EXPORT int ws_client_s_create(ws_client_s *this,
    ws_context_s *a_ctx,const char *a_address,usi a_port,const char *a_path,
    const char *a_protocol);

// === definition of global functions ==========================================

void libwebsocket_cll_init();
void libwebsocket_cll_clear();

// === inline methods of structure ws_context_s ================================

static inline void ws_context_s_init(ws_context_s *this)
{/*{{{*/
  this->context = NULL;
  this->protocols = NULL;
  this->user_data = NULL;

  string_array_s_init(&this->prot_names);
  pointer_array_s_init(&this->prot_callbacks);
  this->ws_fd_event_cb = NULL;

  this->ret_code = 0;
}/*}}}*/

static inline void ws_context_s_clear(ws_context_s *this)
{/*{{{*/

  // - destroy websocket context -
  if (this->context != NULL)
  {
    lws_context_destroy(this->context);
  }

  // - release protocols -
  if (this->protocols != NULL)
  {
    cfree(this->protocols);
  }

  string_array_s_clear(&this->prot_names);
  pointer_array_s_clear(&this->prot_callbacks);

  ws_context_s_init(this);
}/*}}}*/

static inline void ws_context_s_flush_all(ws_context_s *this)
{/*{{{*/
}/*}}}*/

static inline void ws_context_s_swap(ws_context_s *this,ws_context_s *a_second)
{/*{{{*/
  ws_context_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void ws_context_s_copy(const ws_context_s *this,const ws_context_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int ws_context_s_compare(const ws_context_s *this,const ws_context_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void ws_context_s_to_string(const ws_context_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"ws_context_s{%p}",this);
}/*}}}*/
#endif

static inline int ws_context_s_process_fd(ws_context_s *this,struct pollfd *a_pollfd)
{/*{{{*/

  // - reset return code -
  this->ret_code = 0;

  if (lws_service_fd(this->context,a_pollfd) < 0)
  {
    throw_error(WS_CONTEXT_SERVICE_FD_ERROR);
  }

  // - ERROR -
  if (this->ret_code)
  {
    return this->ret_code;
  }

  return 0;
}/*}}}*/

static inline unsigned ws_context_s_get_protocol_idx(ws_context_s *this,struct lws *a_wsi)
{/*{{{*/

  // - retrieve websocket protocol -
  const struct lws_protocols *protocol = lws_get_protocol(a_wsi);

  string_s search_string = {strlen(protocol->name) + 1,(char *)protocol->name};
  return string_array_s_get_idx(&this->prot_names,&search_string);
}/*}}}*/

// === inline methods of structure ws_conn_s ===================================

static inline void ws_conn_s_init(ws_conn_s *this)
{/*{{{*/
  this->wsc_ptr = NULL;
  this->prot_idx = c_idx_not_exist;
  this->ws_ptr = NULL;
  this->user_data = NULL;
  bc_array_s_init(&this->message_buffer);
  bc_array_s_init(&this->data_buffer);
  this->data_in = NULL;
  this->data_len = 0;
}/*}}}*/

static inline void ws_conn_s_clear(ws_conn_s *this)
{/*{{{*/
  bc_array_s_clear(&this->message_buffer);
  bc_array_s_clear(&this->data_buffer);

  ws_conn_s_init(this);
}/*}}}*/

static inline void ws_conn_s_flush_all(ws_conn_s *this)
{/*{{{*/
}/*}}}*/

static inline void ws_conn_s_swap(ws_conn_s *this,ws_conn_s *a_second)
{/*{{{*/
  ws_conn_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void ws_conn_s_copy(const ws_conn_s *this,const ws_conn_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int ws_conn_s_compare(const ws_conn_s *this,const ws_conn_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void ws_conn_s_to_string(const ws_conn_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"ws_conn_s{%p}",this);
}/*}}}*/
#endif

static inline void **ws_conn_s_ctx_user_data(ws_conn_s *this)
{/*{{{*/
  return &this->wsc_ptr->user_data;
}/*}}}*/

static inline const char *ws_conn_s_protocol_name(ws_conn_s *this)
{/*{{{*/
  return lws_get_protocol(this->ws_ptr)->name;
}/*}}}*/

static inline void ws_conn_s_callback_on_writable(ws_conn_s *this)
{/*{{{*/
  lws_callback_on_writable(this->ws_ptr);
}/*}}}*/

static inline void ws_conn_s_set_timeout(ws_conn_s *this,enum pending_timeout a_reason,int a_seconds)
{/*{{{*/
  lws_set_timeout(this->ws_ptr,a_reason,a_seconds);
}/*}}}*/

// === inline methods of structure ws_client_s =================================

static inline void ws_client_s_init(ws_client_s *this)
{/*{{{*/
  this->wsc_ptr = NULL;
  this->ws_ptr = NULL;
}/*}}}*/

static inline void ws_client_s_clear(ws_client_s *this)
{/*{{{*/
  ws_client_s_init(this);
}/*}}}*/

static inline void ws_client_s_flush_all(ws_client_s *this)
{/*{{{*/
}/*}}}*/

static inline void ws_client_s_swap(ws_client_s *this,ws_client_s *a_second)
{/*{{{*/
  ws_client_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void ws_client_s_copy(const ws_client_s *this,const ws_client_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int ws_client_s_compare(const ws_client_s *this,const ws_client_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void ws_client_s_to_string(const ws_client_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"ws_client_s{%p}",this);
}/*}}}*/
#endif

#endif

