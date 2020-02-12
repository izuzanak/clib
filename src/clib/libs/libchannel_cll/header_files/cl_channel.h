
#ifndef __CL_CHANNEL_H
#define __CL_CHANNEL_H

@begin
include "cl_linux.h"
include "cl_var.h"
include "cl_openssl.h"
@end

#include <netinet/tcp.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libchannel_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libchannel_cll_EXPORTS
#define libchannel_cll_EXPORT __declspec(dllexport)
#else
#define libchannel_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_CHANNEL_SERVER_LISTEN_ERROR 1
#define ERROR_CHANNEL_SERVER_SOCKOPT_ERROR 2
#define ERROR_CHANNEL_SERVER_INVALID_FD 3
#define ERROR_CHANNEL_SERVER_ACCEPT_ERROR 4
#define ERROR_CHANNEL_SERVER_SSL_INIT_ERROR 5
#define ERROR_CHANNEL_SERVER_SSL_ACCEPT_ERROR 6

#define ERROR_CHANNEL_CONN_INVALID_FD 1
#define ERROR_CHANNEL_CONN_CONNECT_ERROR 2
#define ERROR_CHANNEL_CONN_SOCKOPT_ERROR 3
#define ERROR_CHANNEL_CONN_SEND_ERROR 4
#define ERROR_CHANNEL_CONN_WRITE_ERROR 5
#define ERROR_CHANNEL_CONN_RECEIVE_ERROR 6
#define ERROR_CHANNEL_CONN_READ_ERROR 7
#define ERROR_CHANNEL_CONN_CALLBACK_ERROR 8
#define ERROR_CHANNEL_CONN_EPOLL_ERROR 9
#define ERROR_CHANNEL_CONN_CLIENT_SSL_INIT_ERROR 10

#ifdef CLIB_WITH_OPENSSL
// - ssl repeated actions -
enum
{
  SSL_ACTION_NONE = 0,
  SSL_ACTION_SEND_MSG,
  SSL_ACTION_RECV_MSG
};
#endif

typedef struct channel_server_s channel_server_s;
typedef int (*channel_conn_new_callback_t)(void *a_object,unsigned a_index);
typedef int (*channel_conn_drop_callback_t)(void *a_object,unsigned a_index);
typedef int (*channel_conn_message_callback_t)(void *a_object,unsigned a_index,const bc_array_s *a_message);

// === definition of var type channel_message  =================================

extern unsigned g_type_channel_message;

static inline var_s loc_s_channel_message_buffer_length(const bc_array_s *a_src);
static inline var_s loc_s_channel_message_buffer_swap(bc_array_s *a_src);
static inline void loc_s_channel_message_clear(var_s this);
static inline int loc_s_channel_message_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_channel_message_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_channel_message_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_channel_message_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
static inline const bc_array_s *loc_s_channel_message_value(var_s this);

// === definition of generated structures ======================================

// -- channel_conn_s --
@begin
struct
<
ssl_conn_s:ssl
ui:ssl_action

epoll_fd_s:epoll_fd
bi:connecting

pointer:conn_message_callback
pointer:cb_object
ui:cb_index

bc_array_s:in_msg
ui:in_msg_length

var_queue_s:out_msg_queue
ui:out_msg_offset
>
channel_conn_s;
@end

libchannel_cll_EXPORT void channel_conn_s_create(channel_conn_s *this,epoll_fd_s *a_epoll_fd,
    channel_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object,unsigned a_cb_index);
WUR libchannel_cll_EXPORT int channel_conn_s_create_client(channel_conn_s *this,
    const char *a_server_ip,unsigned short a_server_port,
    channel_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object,unsigned a_cb_index);
#ifdef CLIB_WITH_OPENSSL
WUR libchannel_cll_EXPORT int channel_conn_s_init_ssl(channel_conn_s *this);
#endif
WUR int channel_conn_s_recv_msg(channel_conn_s *this);
WUR int channel_conn_s_send_msg(channel_conn_s *this);
WUR libchannel_cll_EXPORT int channel_conn_s_fd_event(channel_conn_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR static inline int channel_conn_s_schedule_message_var(channel_conn_s *this,var_s a_length_var,var_s a_data_var);
WUR static inline int channel_conn_s_schedule_message(channel_conn_s *this,bc_array_s *a_message);

// -- channel_conn_list_s --
@begin
list<channel_conn_s> channel_conn_list_s;
@end

// -- channel_server_s --
@begin
struct
<
string_s:ip
usi:port

pointer:conn_new_callback
pointer:conn_drop_callback
pointer:conn_message_callback
pointer:cb_object

ssl_context_s:ssl_ctx
epoll_fd_s:epoll_fd
channel_conn_list_s:conn_list
>
channel_server_s;
@end

WUR libchannel_cll_EXPORT int channel_server_s_create(channel_server_s *this,
    const char *a_ip,unsigned short a_port,
    channel_conn_new_callback_t a_conn_new_callback,
    channel_conn_drop_callback_t a_conn_drop_callback,
    channel_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object);
#ifdef CLIB_WITH_OPENSSL
WUR libchannel_cll_EXPORT int channel_server_s_init_ssl(channel_server_s *this,const char *a_cert_file,const char *a_pkey_file);
#endif
WUR libchannel_cll_EXPORT int channel_server_s_fd_event(channel_server_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR libchannel_cll_EXPORT int channel_server_s_conn_fd_event(void *a_channel_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// === definition of global functions ==========================================

void libchannel_cll_init();
void libchannel_cll_clear();

// === inline methods of var type channel_message  =============================

static inline var_s loc_s_channel_message_buffer_length(const bc_array_s *a_src)
{/*{{{*/
  bc_array_s *buffer = (bc_array_s *)cmalloc(sizeof(bc_array_s));
  bc_array_s_init(buffer);
  bc_array_s_append_format(buffer,"0x%8.8x;",a_src->used);

  var_s var = loc_s___new();
  var->v_type = g_type_channel_message;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = buffer;

  return var;
}/*}}}*/

static inline var_s loc_s_channel_message_buffer_swap(bc_array_s *a_src)
{/*{{{*/
  bc_array_s *buffer = (bc_array_s *)cmalloc(sizeof(bc_array_s));
  bc_array_s_init(buffer);
  bc_array_s_swap(buffer,a_src);

  var_s var = loc_s___new();
  var->v_type = g_type_channel_message;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = buffer;

  return var;
}/*}}}*/

static inline void loc_s_channel_message_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == g_type_channel_message);

  bc_array_s *buffer = (bc_array_s *)this->v_data.ptr;
  bc_array_s_clear(buffer);
  cfree(buffer);
}/*}}}*/

static inline int loc_s_channel_message_order(var_s a_first,var_s a_second)
{/*{{{*/
  debug_assert(a_first->v_type == g_type_channel_message &&
               a_second->v_type == g_type_channel_message);

  bc_array_s *first = (bc_array_s *)a_first->v_data.ptr;
  bc_array_s *second = (bc_array_s *)a_second->v_data.ptr;

  if (first->used < second->used) { return -1; }
  if (first->used > second->used) { return 1; }

  return memcmp(first->data,second->data,first->used - 1);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_channel_message_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  (void)this;

  bc_array_s_append_ptr(a_trg,"{}");
}/*}}}*/
#endif

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_channel_message_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  (void)this;

  bc_array_s_append_ptr(a_trg,"{}");
}/*}}}*/

static inline void loc_s_channel_message_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  (void)this;
  (void)a_json_nice;

  bc_array_s_append_ptr(a_trg,"{}");
}/*}}}*/
#endif

static inline const bc_array_s *loc_s_channel_message_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == g_type_channel_message);

  return (bc_array_s *)this->v_data.ptr;
}/*}}}*/

// === inline methods of generated structures ==================================

// -- channel_conn_s --
@begin
inlines channel_conn_s
@end

static inline int channel_conn_s_schedule_message_var(channel_conn_s *this,var_s a_length_var,var_s a_data_var)
{/*{{{*/
  var_queue_s_insert(&this->out_msg_queue,&a_length_var);
  var_queue_s_insert(&this->out_msg_queue,&a_data_var);

  // - modify fd epoll events: input and output -
  if (epoll_fd_s_modify_events(&this->epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI))
  {
    throw_error(CHANNEL_CONN_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

static inline int channel_conn_s_schedule_message(channel_conn_s *this,bc_array_s *a_message)
{/*{{{*/
  var_s length_var = loc_s_channel_message_buffer_length(a_message);
  var_s data_var = loc_s_channel_message_buffer_swap(a_message);

  return channel_conn_s_schedule_message_var(this,length_var,data_var);
}/*}}}*/

// -- channel_conn_list_s --
@begin
inlines channel_conn_list_s
@end

// -- channel_server_s --
@begin
inlines channel_server_s
@end

#endif

