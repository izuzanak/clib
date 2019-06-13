
#ifndef __CL_WEBSOCKETS_H
#define __CL_WEBSOCKETS_H

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

typedef struct ws_context_s ws_context_s;
typedef struct ws_conn_s ws_conn_s;

// === definition of structure ws_context_s ====================================

int (*ws_fd_event_cb_t)(int a_fd,unsigned a_events);
int (*ws_prot_conn_cb_t)(ws_conn_s *a_conn);

typedef struct ws_context_s
{
  struct libwebsocket_context *context;
  struct libwebsocket_protocols *protocols;
  void *user_data;

  string_array_s prot_names;
  pointer_array_s prot_callbacks;
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
int ws_context_s_http_func(struct libwebsocket_context *ctx,struct libwebsocket *wsi,
    enum libwebsocket_callback_reasons reason,void *user,void *in,size_t len);
int ws_context_s_protocol_func(struct libwebsocket_context *ctx,struct libwebsocket *wsi,
    enum libwebsocket_callback_reasons reason,void *user,void *in,size_t len);

WUR libwebsockets_cll_EXPORT int ws_context_s_create(ws_context_s *this,
    usi a_port,string_array_s *a_prot_names,pointer_array_s *a_prot_callbacks,void *a_user_data);

// === definition of generated structures ======================================

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
}/*}}}*/

static inline void ws_context_s_clear(ws_context_s *this)
{/*{{{*/
  
  // - destroy websocket context -
  if (this->context != NULL)
  {
    libwebsocket_context_destroy(this->context);
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

// === inline methods of generated structures ==================================

#endif

