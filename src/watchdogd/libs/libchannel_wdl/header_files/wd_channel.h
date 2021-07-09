
#ifndef __WD_CHANNEL_H
#define __WD_CHANNEL_H

@begin
include "cl_json.h"
include "cl_logger.h"
include "cl_channel.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libchannel_wdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libchannel_wdl_EXPORTS
#define libchannel_wdl_EXPORT __declspec(dllexport)
#else
#define libchannel_wdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_WD_CHANNEL_SERVER_CREATE_ERROR 1
#define ERROR_WD_CHANNEL_SERVER_EPOLL_ERROR 2
#define ERROR_WD_CHANNEL_SERVER_FD_EVENT_ERROR 3
#define ERROR_WD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_WD_CHANNEL_SERVER_SEND_MESSAGE_ERROR 5
#define ERROR_WD_CHANNEL_SERVER_CALLBACK_ERROR 6
#define ERROR_WD_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR 7
#define ERROR_WD_CHANNEL_MESSAGE_ERROR 8

#define ERROR_WD_CHANNEL_CLIENT_EPOLL_ERROR 1
#define ERROR_WD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR 2
#define ERROR_WD_CHANNEL_CLIENT_TIMER_READ_ERROR 3
#define ERROR_WD_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_WD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR 5
#define ERROR_WD_CHANNEL_CLIENT_CALLBACK_ERROR 6

// === constants and definitions ===============================================

#define WD_CHANNEL_RECONNECT_PERIOD {0,500000000}
#define WD_CHANNEL_PING_PERIOD {5,0}
#define WD_CHANNEL_PING_TIMEOUT {30,0}

typedef int (*wd_channel_cbreq_t)(void *a_object,unsigned a_index,unsigned a_type,va_list a_ap);

// === constants and definitions ===============================================

enum
{/*{{{*/
  wd_channel_FIRST = 1,

  wd_channel_ID,

  wd_channel_TYPE,
  wd_channel_RESP,
  wd_channel_NAME,
  wd_channel_TIMEOUT,
  wd_channel_DATA,

  wd_channel_ENABLE,
  wd_channel_DISABLE,
  wd_channel_KEEPALIVE,
  wd_channel_STATUS,
  wd_channel_PING,

  wd_channel_LAST,
};/*}}}*/

enum
{/*{{{*/
  wd_channel_cbreq_NEW  = -1,
  wd_channel_cbreq_DROP = -2,

  wd_channel_cbreq_ENABLE = 0,
  wd_channel_cbreq_DISABLE,
  wd_channel_cbreq_KEEPALIVE,
  wd_channel_cbreq_STATUS,

  wd_channel_cbresp_ENABLE,
  wd_channel_cbresp_DISABLE,
  wd_channel_cbresp_STATUS,
};/*}}}*/

// === definition of generated structures ======================================

// -- wd_channel_s --
@begin
struct
<
channel_server_s:server

pointer:channel_callback
pointer:cb_object

bc_array_s:buffer
>
wd_channel_s;
@end

WUR libchannel_wdl_EXPORT int wd_channel_s_create(wd_channel_s *this,const char *a_ip,unsigned short a_port,
    wd_channel_cbreq_t a_channel_callback,
    void *a_cb_object);
WUR static inline int wd_channel_s_message_call(wd_channel_s *this,unsigned a_index,unsigned a_type,...);
WUR static inline int wd_channel_s_send_message(wd_channel_s *this,unsigned a_index,bc_array_s *a_message);
WUR libchannel_wdl_EXPORT int wd_channel_s_send_multi_message_var(wd_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var);
WUR static inline int wd_channel_s_send_multi_message(wd_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message);
WUR int wd_channel_s_conn_new(void *a_wd_channel,unsigned a_index);
WUR int wd_channel_s_conn_drop(void *a_wd_channel,unsigned a_index);
WUR libchannel_wdl_EXPORT int wd_channel_s_conn_message(void *a_wd_channel,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_wdl_EXPORT int wd_channel_s_fd_event(void *a_wd_channel,unsigned a_index,epoll_event_s *a_epoll_event);

// -- wd_channel_client_s --
@begin
struct
<
string_s:server_ip
usi:server_port
channel_conn_s:connection
bi:connected
lli:message_id

pointer:channel_callback
pointer:cb_object
ui:cb_index

bc_array_s:buffer

epoll_timer_s:connect_timer
epoll_timer_s:ping_timer
>
wd_channel_client_s;
@end

WUR libchannel_wdl_EXPORT int wd_channel_client_s_create(wd_channel_client_s *this,
  const char *a_server_ip,unsigned short a_server_port,
  wd_channel_cbreq_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index);
WUR static inline int wd_channel_client_s_message_call(wd_channel_client_s *this,unsigned a_type,...);
WUR static inline int wd_channel_client_s_send_message(wd_channel_client_s *this,bc_array_s *a_message);

WUR libchannel_wdl_EXPORT int wd_channel_client_s_conn_message(void *a_wd_channel_client,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_wdl_EXPORT int wd_channel_client_s_fd_event(void *a_wd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event);
WUR libchannel_wdl_EXPORT int wd_channel_client_s_connect_time_event(void *a_wd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event);
WUR libchannel_wdl_EXPORT int wd_channel_client_s_ping_time_event(void *a_wd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event);

// === definition of global functions ==========================================

extern var_tree_s g_wd_channel_vars;

void libchannel_wdl_init();
void libchannel_wdl_clear();

// === inline methods of generated structures ==================================

// -- wd_channel_s --
@begin
inlines wd_channel_s
@end

static inline int wd_channel_s_message_call(wd_channel_s *this,unsigned a_index,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((wd_channel_cbreq_t)this->channel_callback)(this->cb_object,a_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int wd_channel_s_send_message(wd_channel_s *this,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
#if OPTION_BRUTAL_ASSERT == ENABLED
  CONT_INIT_CLEAR(var_s,msg_var);
  CONT_INIT_CLEAR(json_parser_s,json_parser);
  brutal_assert(json_parser_s_parse(&json_parser,a_message,&msg_var) == 0);
#endif

  // - log message -
  log_info_2("channel server %u, --> " LOG_MSG_FORMAT,a_index,
      LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  // - schedule message to send -
  if (channel_conn_s_schedule_message(&this->server.conn_list.data[a_index].object,a_message))
  {
    throw_error(WD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int wd_channel_s_send_multi_message(wd_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message)
{/*{{{*/
  VAR_CLEAR(data_var,loc_s_channel_message_buffer_swap(a_message));

  if (wd_channel_s_send_multi_message_var(this,a_indexes,data_var))
  {
    throw_error(WD_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR);
  }

  return 0;
}/*}}}*/

// -- wd_channel_client_s --
@begin
inlines wd_channel_client_s
@end

static inline int wd_channel_client_s_message_call(wd_channel_client_s *this,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((wd_channel_cbreq_t)this->channel_callback)(this->cb_object,this->cb_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int wd_channel_client_s_send_message(wd_channel_client_s *this,bc_array_s *a_message)
{/*{{{*/
#if OPTION_BRUTAL_ASSERT == ENABLED
  CONT_INIT_CLEAR(var_s,msg_var);
  CONT_INIT_CLEAR(json_parser_s,json_parser);
  brutal_assert(json_parser_s_parse(&json_parser,a_message,&msg_var) == 0);
#endif

  // - log message -
  log_info_2("channel client, --> " LOG_MSG_FORMAT,
      LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  if (channel_conn_s_schedule_message(&this->connection,a_message))
  {
    throw_error(WD_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

#endif

