
#ifndef __TD_CHANNEL_H
#define __TD_CHANNEL_H

@begin
include "cl_logger.h"
include "cl_channel.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libchannel_tdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libchannel_tdl_EXPORTS
#define libchannel_tdl_EXPORT __declspec(dllexport)
#else
#define libchannel_tdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_TD_CHANNEL_SERVER_CREATE_ERROR 1
#define ERROR_TD_CHANNEL_SERVER_EPOLL_ERROR 2
#define ERROR_TD_CHANNEL_SERVER_FD_EVENT_ERROR 3
#define ERROR_TD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_TD_CHANNEL_SERVER_CALLBACK_ERROR 5
#define ERROR_TD_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR 6

#define ERROR_TD_CHANNEL_CLIENT_EPOLL_ERROR 1
#define ERROR_TD_CHANNEL_CLIENT_TIMER_READ_ERROR 2
#define ERROR_TD_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR 3
#define ERROR_TD_CHANNEL_CLIENT_CALLBACK_ERROR 4

typedef int (*td_channel_callback_t)(void *a_object,unsigned a_index,unsigned a_type,va_list a_ap);

// === constants and definitions ===============================================

enum
{/*{{{*/
  td_channel_callback_NEW = 0,
  td_channel_callback_DROP,
};/*}}}*/

// === definition of generated structures ======================================

// -- td_channel_s --
@begin
struct
<
channel_server_s:server

pointer:channel_callback
pointer:cb_object
>
td_channel_s;
@end

WUR libchannel_tdl_EXPORT int td_channel_s_create(td_channel_s *this,const char *a_ip,unsigned short a_port,epoll_s *a_epoll,
    td_channel_callback_t a_channel_callback,
    void *a_cb_object);
WUR static inline int td_channel_s_message_call(td_channel_s *this,unsigned a_index,unsigned a_type,...);
WUR static inline int td_channel_s_send_message(td_channel_s *this,unsigned a_index,bc_array_s *a_message);
WUR libchannel_tdl_EXPORT int td_channel_s_send_multi_message_var(td_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var);
WUR static inline int td_channel_s_send_multi_message(td_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message);
WUR int td_channel_s_conn_new(void *a_td_channel,unsigned a_index);
WUR int td_channel_s_conn_drop(void *a_td_channel,unsigned a_index);
WUR libchannel_tdl_EXPORT int td_channel_s_conn_message(void *a_td_channel,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_tdl_EXPORT int td_channel_s_fd_event(void *a_td_channel,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// -- td_channel_client_s --
@begin
struct
<
string_s:server_ip
usi:server_port
channel_conn_s:connection
lli:message_id

pointer:channel_callback
pointer:cb_object
ui:cb_index

epoll_timer_s:epoll_timer
>
td_channel_client_s;
@end

WUR libchannel_tdl_EXPORT int td_channel_client_s_create(td_channel_client_s *this,epoll_s *a_epoll,
  const char *a_server_ip,unsigned short a_server_port,
  td_channel_callback_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index);
WUR static inline int td_channel_client_s_message_call(td_channel_client_s *this,unsigned a_type,...);
WUR static inline int td_channel_client_s_send_message(td_channel_client_s *this,bc_array_s *a_message);
WUR libchannel_tdl_EXPORT int td_channel_client_s_conn_message(void *a_td_channel_client,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_tdl_EXPORT int td_channel_client_s_fd_event(void *a_td_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR libchannel_tdl_EXPORT int td_channel_client_s_connect_time_event(void *a_td_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// === inline methods of generated structures ==================================

// -- td_channel_s --
@begin
inlines td_channel_s
@end

static inline int td_channel_s_message_call(td_channel_s *this,unsigned a_index,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((td_channel_callback_t)this->channel_callback)(this->cb_object,a_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int td_channel_s_send_message(td_channel_s *this,unsigned a_index,bc_array_s *a_message)
{/*{{{*/

  // - log message -
  log_info_2("channel server %u, --> " LOG_MSG_FORMAT,
      a_index,LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  // - schedule message to send -
  if (channel_conn_s_schedule_message(&this->server.conn_list.data[a_index].object,a_message))
  {
    throw_error(TD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int td_channel_s_send_multi_message(td_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message)
{/*{{{*/
  VAR_CLEAR(data_var,loc_s_channel_message_buffer_swap(a_message));

  if (td_channel_s_send_multi_message_var(this,a_indexes,data_var))
  {
    throw_error(TD_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR);
  }

  return 0;
}/*}}}*/

// -- td_channel_client_s --
@begin
inlines td_channel_client_s
@end

static inline int td_channel_client_s_message_call(td_channel_client_s *this,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((td_channel_callback_t)this->channel_callback)(this->cb_object,this->cb_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int td_channel_client_s_send_message(td_channel_client_s *this,bc_array_s *a_message)
{/*{{{*/

  // - log message -
  log_info_2("channel client, --> " LOG_MSG_FORMAT,
      LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  if (channel_conn_s_schedule_message(&this->connection,a_message))
  {
    throw_error(TD_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

#endif

