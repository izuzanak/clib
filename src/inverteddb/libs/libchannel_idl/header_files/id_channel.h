
#ifndef ID_LIB_CHANNEL_H
#define ID_LIB_CHANNEL_H

@begin
include "cl_json.h"
include "cl_logger.h"
include "cl_channel.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libchannel_idl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libchannel_idl_EXPORTS
#define libchannel_idl_EXPORT __declspec(dllexport)
#else
#define libchannel_idl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_ID_CHANNEL_SERVER_CREATE_ERROR 1
#define ERROR_ID_CHANNEL_SERVER_EPOLL_ERROR 2
#define ERROR_ID_CHANNEL_SERVER_FD_EVENT_ERROR 3
#define ERROR_ID_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_ID_CHANNEL_SERVER_SEND_MESSAGE_ERROR 5
#define ERROR_ID_CHANNEL_SERVER_CALLBACK_ERROR 6
#define ERROR_ID_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR 7
#define ERROR_ID_CHANNEL_MESSAGE_ERROR 8

#define ERROR_ID_CHANNEL_CLIENT_EPOLL_ERROR 1
#define ERROR_ID_CHANNEL_CLIENT_TIMER_SETTIME_ERROR 2
#define ERROR_ID_CHANNEL_CLIENT_TIMER_READ_ERROR 3
#define ERROR_ID_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_ID_CHANNEL_CLIENT_SEND_MESSAGE_ERROR 5
#define ERROR_ID_CHANNEL_CLIENT_CALLBACK_ERROR 6

// === constants and definitions ===============================================

#define ID_CHANNEL_RECONNECT_PERIOD {0,500000000}
#define ID_CHANNEL_PING_PERIOD {5,0}
#define ID_CHANNEL_PING_TIMEOUT {30,0}

typedef int (*id_channel_cbreq_t)(void *a_object,unsigned a_index,unsigned a_type,va_list a_ap);

// === constants and definitions ===============================================

enum
{/*{{{*/
  id_channel_ID = 1,

  id_channel_TYPE,
  id_channel_RESP,
  id_channel_PATH,
  id_channel_MOD,
  id_channel_DATABASE,
  id_channel_DOCUMENT,
  id_channel_DATA,

  id_channel_EXTRACT,
  id_channel_INDEX,
  id_channel_REINDEX,
  id_channel_REMOVE,
  id_channel_QUERY,

  id_channel_WATCH,
  id_channel_IGNORE,
  id_channel_UPDATE,
  id_channel_PING,

  id_channel_OPTIONS,
};/*}}}*/

enum
{/*{{{*/
  id_channel_cbreq_NEW  = -1,
  id_channel_cbreq_DROP = -2,

  id_channel_cbreq_EXTRACT = 0,
  id_channel_cbreq_INDEX,
  id_channel_cbreq_REINDEX,
  id_channel_cbreq_REMOVE,
  id_channel_cbreq_QUERY,

  id_channel_cbresp_EXTRACT,
  id_channel_cbresp_INDEX,
  id_channel_cbresp_REINDEX,
  id_channel_cbresp_REMOVE,
  id_channel_cbresp_QUERY,

  id_channel_cbevt_UPDATE,
};/*}}}*/

// === definition of generated structures ======================================

// -- id_channel_s --
@begin
struct
<
channel_server_s:server

pointer:channel_callback
pointer:cb_object

bc_array_s:buffer
>
id_channel_s;
@end

WUR libchannel_idl_EXPORT int id_channel_s_create(id_channel_s *this,const char *a_ip,unsigned short a_port,
    id_channel_cbreq_t a_channel_callback,
    void *a_cb_object);
WUR static inline int id_channel_s_message_call(id_channel_s *this,unsigned a_index,unsigned a_type,...);
WUR static inline int id_channel_s_send_message(id_channel_s *this,unsigned a_index,bc_array_s *a_message);
WUR libchannel_idl_EXPORT int id_channel_s_send_multi_message_var(id_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var);
WUR static inline int id_channel_s_send_multi_message(id_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message);
WUR int id_channel_s_conn_new(void *a_id_channel,unsigned a_index);
WUR int id_channel_s_conn_drop(void *a_id_channel,unsigned a_index);
WUR libchannel_idl_EXPORT int id_channel_s_conn_message(void *a_id_channel,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_idl_EXPORT int id_channel_s_fd_event(void *a_id_channel,unsigned a_index,epoll_event_s *a_epoll_event);

// -- id_channel_client_s --
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
id_channel_client_s;
@end

WUR libchannel_idl_EXPORT int id_channel_client_s_create(id_channel_client_s *this,
  const char *a_server_ip,unsigned short a_server_port,
  id_channel_cbreq_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index);
WUR static inline int id_channel_client_s_message_call(id_channel_client_s *this,unsigned a_type,...);
WUR static inline int id_channel_client_s_send_message(id_channel_client_s *this,bc_array_s *a_message);

WUR libchannel_idl_EXPORT int id_channel_client_s_conn_message(void *a_id_channel_client,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_idl_EXPORT int id_channel_client_s_fd_event(void *a_id_channel_client,unsigned a_index,epoll_event_s *a_epoll_event);
WUR libchannel_idl_EXPORT int id_channel_client_s_connect_time_event(void *a_id_channel_client,unsigned a_index,epoll_event_s *a_epoll_event);
WUR libchannel_idl_EXPORT int id_channel_client_s_ping_time_event(void *a_id_channel_client,unsigned a_index,epoll_event_s *a_epoll_event);

// === definition of global functions ==========================================

extern var_tree_s g_id_channel_vars;

void libchannel_idl_init();
void libchannel_idl_clear();

// === inline methods of generated structures ==================================

// -- id_channel_s --
@begin
inlines id_channel_s
@end

static inline int id_channel_s_message_call(id_channel_s *this,unsigned a_index,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((id_channel_cbreq_t)this->channel_callback)(this->cb_object,a_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int id_channel_s_send_message(id_channel_s *this,unsigned a_index,bc_array_s *a_message)
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
    throw_error(ID_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int id_channel_s_send_multi_message(id_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message)
{/*{{{*/
  VAR_CLEAR(data_var,loc_s_channel_message_buffer_swap(a_message));

  if (id_channel_s_send_multi_message_var(this,a_indexes,data_var))
  {
    throw_error(ID_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR);
  }

  return 0;
}/*}}}*/

// -- id_channel_client_s --
@begin
inlines id_channel_client_s
@end

static inline int id_channel_client_s_message_call(id_channel_client_s *this,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((id_channel_cbreq_t)this->channel_callback)(this->cb_object,this->cb_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int id_channel_client_s_send_message(id_channel_client_s *this,bc_array_s *a_message)
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
    throw_error(ID_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

#endif

