
#ifndef __GPSD_H
#define __GPSD_H

@begin
include "cl_json.h"
include "cl_logger.h"
include "cl_tcp.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libgpsd_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libgpsd_cll_EXPORTS
#define libgpsd_cll_EXPORT __declspec(dllexport)
#else
#define libgpsd_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_GPSD_CLIENT_EPOLL_ERROR 1
#define ERROR_GPSD_CLIENT_TIMER_READ_ERROR 2
#define ERROR_GPSD_CLIENT_SCHEDULE_MESSAGE_ERROR 3
#define ERROR_GPSD_CLIENT_CALLBACK_ERROR 4
#define ERROR_GPSD_MESSAGE_ERROR 5

typedef int (*cl_gpsd_cbreq_t)(void *a_object,unsigned a_index,unsigned a_type,va_list a_ap);

// === constants and definitions ===============================================

enum
{/*{{{*/
  cl_gpsd_first = 1,

  cl_gpsd_class,

  cl_gpsd_VERSION,
  cl_gpsd_DEVICES,
  cl_gpsd_DEVICE,
  cl_gpsd_SKY,
  cl_gpsd_WATCH,
  cl_gpsd_TPV,

  cl_gpsd_release,
  cl_gpsd_rev,
  cl_gpsd_proto_major,
  cl_gpsd_proto_minor,

  cl_gpsd_last,
};/*}}}*/

enum
{/*{{{*/
  cl_gpsd_cbreq_NEW  = -1,
  cl_gpsd_cbreq_DROP = -2,

  cl_gpsd_cbreq_VERSION = 0,
  cl_gpsd_cbreq_DEVICES,
  cl_gpsd_cbreq_DEVICE,
  cl_gpsd_cbreq_SKY,
  cl_gpsd_cbreq_WATCH,
  cl_gpsd_cbreq_TPV,
};/*}}}*/

// === definition of generated structures ======================================

// -- gpsd_conn_s --
@begin
struct
<
string_s:server_ip
usi:server_port
tcp_conn_s:connection
lli:message_id

pointer:gpsd_callback
pointer:cb_object
ui:cb_index

epoll_timer_s:epoll_timer
>
gpsd_conn_s;
@end

WUR libgpsd_cll_EXPORT int gpsd_conn_s_create(gpsd_conn_s *this,epoll_s *a_epoll,
  const char *a_server_ip,unsigned short a_server_port,
  cl_gpsd_cbreq_t a_gpsd_callback,
  void *a_cb_object,unsigned a_cb_index);
WUR unsigned gpsd_conn_s_retrieve_line(bc_array_s *a_message,unsigned *a_input_idx);
WUR static inline int gpsd_conn_s_message_call(gpsd_conn_s *this,unsigned a_type,...);
WUR static inline int gpsd_conn_s_send_message(gpsd_conn_s *this,bc_array_s *a_message);
WUR libgpsd_cll_EXPORT int gpsd_conn_s_conn_message(void *a_gpsd_conn,unsigned a_index,bc_array_s *a_message);
WUR libgpsd_cll_EXPORT int gpsd_conn_s_fd_event(void *a_gpsd_conn,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR libgpsd_cll_EXPORT int gpsd_conn_s_connect_time_event(void *a_gpsd_conn,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// === definition of global functions ==========================================

extern json_parser_s g_gpsd_json_parser;
extern pointer_tree_s g_gpsd_json_string_map;

void libgpsd_cll_init();
void libgpsd_cll_clear();

// === inline methods of generated structures ==================================

// -- gpsd_conn_s --
@begin
inlines gpsd_conn_s
@end

static inline int gpsd_conn_s_message_call(gpsd_conn_s *this,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((cl_gpsd_cbreq_t)this->gpsd_callback)(this->cb_object,this->cb_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int gpsd_conn_s_send_message(gpsd_conn_s *this,bc_array_s *a_message)
{/*{{{*/

  // - log message -
  log_info_2("gpsd client, --> " LOG_MSG_FORMAT,
      LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  if (tcp_conn_s_schedule_message(&this->connection,a_message))
  {
    throw_error(GPSD_CLIENT_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

#endif

