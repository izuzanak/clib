
#ifndef __OD_CHANNEL_H
#define __OD_CHANNEL_H

@begin
include "cl_json.h"
include "cl_logger.h"
include "cl_channel.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libchannel_odl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libchannel_odl_EXPORTS
#define libchannel_odl_EXPORT __declspec(dllexport)
#else
#define libchannel_odl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_OD_CHANNEL_SERVER_CREATE_ERROR 1
#define ERROR_OD_CHANNEL_SERVER_EPOLL_ERROR 2
#define ERROR_OD_CHANNEL_SERVER_FD_EVENT_ERROR 3
#define ERROR_OD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_OD_CHANNEL_SERVER_CALLBACK_ERROR 5
#define ERROR_OD_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR 6
#define ERROR_OD_CHANNEL_MESSAGE_ERROR 7

#define ERROR_OD_CHANNEL_CLIENT_EPOLL_ERROR 1
#define ERROR_OD_CHANNEL_CLIENT_TIMER_READ_ERROR 2
#define ERROR_OD_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR 3
#define ERROR_OD_CHANNEL_CLIENT_CALLBACK_ERROR 4

typedef int (*od_channel_cbreq_t)(void *a_object,unsigned a_index,unsigned a_type,va_list a_ap);

// === constants and definitions ===============================================

enum
{/*{{{*/
  od_channel_FIRST = 1,

  od_channel_ID,

  od_channel_TYPE,
  od_channel_RESP,
  od_channel_PATH,
  od_channel_DATA,

  od_channel_SET,
  od_channel_GET,
  od_channel_WATCH,
  od_channel_IGNORE,
  od_channel_UPDATE,

  od_channel_LAST,
};/*}}}*/

enum
{/*{{{*/
  od_channel_cbreq_NEW  = -1,
  od_channel_cbreq_DROP = -2,

  od_channel_cbreq_SET = 0,
  od_channel_cbreq_GET,
  od_channel_cbreq_WATCH,
  od_channel_cbreq_IGNORE,

  od_channel_cbresp_SET,
  od_channel_cbresp_GET,
  od_channel_cbresp_WATCH,
  od_channel_cbresp_IGNORE,

  od_channel_cbevt_UPDATE,
};/*}}}*/

// === definition of generated structures ======================================

// -- pointer_tree_s --
@begin
rb_tree<pointer> pointer_tree_s;
@end

// -- od_channel_s --
@begin
struct
<
channel_server_s:server

pointer:channel_callback
pointer:cb_object
>
od_channel_s;
@end

WUR libchannel_odl_EXPORT int od_channel_s_create(od_channel_s *this,const char *a_ip,unsigned short a_port,epoll_s *a_epoll,
    od_channel_cbreq_t a_channel_callback,
    void *a_cb_object);
WUR static inline int od_channel_s_message_call(od_channel_s *this,unsigned a_index,unsigned a_type,...);
WUR static inline int od_channel_s_send_message(od_channel_s *this,unsigned a_index,bc_array_s *a_message);
WUR libchannel_odl_EXPORT int od_channel_s_send_multi_message_var(od_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var);
WUR static inline int od_channel_s_send_multi_message(od_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message);
WUR int od_channel_s_conn_new(void *a_od_channel,unsigned a_index);
WUR int od_channel_s_conn_drop(void *a_od_channel,unsigned a_index);
WUR libchannel_odl_EXPORT int od_channel_s_conn_message(void *a_od_channel,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_odl_EXPORT int od_channel_s_fd_event(void *a_od_channel,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// -- od_channel_client_s --
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
od_channel_client_s;
@end

WUR libchannel_odl_EXPORT int od_channel_client_s_create(od_channel_client_s *this,epoll_s *a_epoll,
  const char *a_server_ip,unsigned short a_server_port,
  od_channel_cbreq_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index);
WUR static inline int od_channel_client_s_message_call(od_channel_client_s *this,unsigned a_type,...);
WUR static inline int od_channel_client_s_send_message(od_channel_client_s *this,bc_array_s *a_message);
WUR libchannel_odl_EXPORT int od_channel_client_s_conn_message(void *a_od_channel_client,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_odl_EXPORT int od_channel_client_s_fd_event(void *a_od_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR libchannel_odl_EXPORT int od_channel_client_s_connect_time_event(void *a_od_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// === definition of global functions ==========================================

extern json_parser_s g_od_channel_json_parser;
extern pointer_tree_s g_od_channel_json_string_map;

void libchannel_odl_init();
void libchannel_odl_clear();

// === inline methods of generated structures ==================================

// -- pointer_tree_s --
@begin
inlines pointer_tree_s
@end

static inline int pointer_tree_s___compare_value(const pointer_tree_s *this,const pointer *a_first,const pointer *a_second)
{/*{{{*/
  (void)this;

  return *a_first < *a_second ? -1 : *a_first > *a_second ? 1 : 0;
}/*}}}*/

// -- od_channel_s --
@begin
inlines od_channel_s
@end

static inline int od_channel_s_message_call(od_channel_s *this,unsigned a_index,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((od_channel_cbreq_t)this->channel_callback)(this->cb_object,a_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int od_channel_s_send_message(od_channel_s *this,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
#if OPTION_BRUTAL_ASSERT == ENABLED
  CONT_INIT_CLEAR(var_s,msg_var);
  brutal_assert(json_parser_s_parse(&g_od_channel_json_parser,a_message,&msg_var) == 0);
#endif

  // - log message -
  log_info_2("channel server %u, --> " LOG_MSG_FORMAT,a_index,
      LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  // - schedule message to send -
  if (channel_conn_s_schedule_message(&this->server.conn_list.data[a_index].object,a_message))
  {
    throw_error(OD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int od_channel_s_send_multi_message(od_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message)
{/*{{{*/
  VAR_CLEAR(data_var,loc_s_channel_message_buffer_swap(a_message));

  if (od_channel_s_send_multi_message_var(this,a_indexes,data_var))
  {
    throw_error(OD_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR);
  }

  return 0;
}/*}}}*/

// -- od_channel_client_s --
@begin
inlines od_channel_client_s
@end

static inline int od_channel_client_s_message_call(od_channel_client_s *this,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((od_channel_cbreq_t)this->channel_callback)(this->cb_object,this->cb_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int od_channel_client_s_send_message(od_channel_client_s *this,bc_array_s *a_message)
{/*{{{*/
#if OPTION_BRUTAL_ASSERT == ENABLED
  CONT_INIT_CLEAR(var_s,msg_var);
  brutal_assert(json_parser_s_parse(&g_od_channel_json_parser,a_message,&msg_var) == 0);
#endif

  // - log message -
  log_info_2("channel client, --> " LOG_MSG_FORMAT,
      LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  if (channel_conn_s_schedule_message(&this->connection,a_message))
  {
    throw_error(OD_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

#endif

