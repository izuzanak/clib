
#ifndef SD_LIB_CHANNEL_H
#define SD_LIB_CHANNEL_H

@begin
include "cl_logger.h"
include "cl_channel.h"
@end

#include "endian.h"

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libchannel_sdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libchannel_sdl_EXPORTS
#define libchannel_sdl_EXPORT __declspec(dllexport)
#else
#define libchannel_sdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SD_CHANNEL_SERVER_CREATE_ERROR 1
#define ERROR_SD_CHANNEL_SERVER_EPOLL_ERROR 2
#define ERROR_SD_CHANNEL_SERVER_FD_EVENT_ERROR 3
#define ERROR_SD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_SD_CHANNEL_SERVER_SEND_MESSAGE_ERROR 5
#define ERROR_SD_CHANNEL_SERVER_CALLBACK_ERROR 6
#define ERROR_SD_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR 7

#define ERROR_SD_CHANNEL_CLIENT_EPOLL_ERROR 1
#define ERROR_SD_CHANNEL_CLIENT_TIMER_READ_ERROR 2
#define ERROR_SD_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR 3
#define ERROR_SD_CHANNEL_CLIENT_CALLBACK_ERROR 4
#define ERROR_SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR 5

typedef int (*sd_channel_cbreq_t)(void *a_object,unsigned a_index,unsigned a_type,va_list a_ap);

// === constants and definitions ===============================================

enum
{/*{{{*/
  sd_channel_msg_type_REQUEST = 0,
  sd_channel_msg_type_RESPONSE,
  sd_channel_msg_type_EVENT,
};/*}}}*/

enum
{/*{{{*/
  sd_channel_cbreq_NEW  = -1,
  sd_channel_cbreq_DROP = -2,

  sd_channel_cbreq_SEGMENT_INFO = 1,
  sd_channel_cbreq_SEGMENT_WRITE,
  sd_channel_cbreq_SEGMENT_READ,
  sd_channel_cbreq_SEGMENT_WATCH,
  sd_channel_cbreq_SEGMENT_IGNORE,
  sd_channel_cbreq_SEGMENT_UPDATE,

  sd_channel_cbreq_TRACE_INFO = 11,
  sd_channel_cbreq_TRACE_WRITE,
  sd_channel_cbreq_TRACE_READ,
  sd_channel_cbreq_TRACE_HEAD,
  sd_channel_cbreq_TRACE_TAIL,
  sd_channel_cbreq_TRACE_RANGE,
  sd_channel_cbreq_TRACE_STEP_RANGE,
  sd_channel_cbreq_TRACE_TIME_RANGE,
  sd_channel_cbreq_TRACE_LEE_TIME,
  sd_channel_cbreq_TRACE_GRE_TIME,
  sd_channel_cbreq_TRACE_WATCH,
  sd_channel_cbreq_TRACE_IGNORE,
  sd_channel_cbreq_TRACE_UPDATE,

  sd_channel_cbreq_CONFIG = 30,
  sd_channel_cbreq_PING = 40,
};/*}}}*/

// === definition of generated structures ======================================

// -- bc_array_s --

static inline void bc_array_s_append_sd_segmentd_msg_header(bc_array_s *this,
    ulli a_id,usi a_type,usi a_req_id,const string_s *a_segment_id);

// -- sd_range_record_s --
@begin
struct
<
lli:record_id
ulli:time
bc_array_s:record
>
sd_range_record_s;
@end

// -- sd_range_records_s --
@begin
array<sd_range_record_s> sd_range_records_s;
@end

// -- sd_channel_s --
@begin
struct
<
channel_server_s:server

pointer:channel_callback
pointer:cb_object

bc_array_s:buffer
>
sd_channel_s;
@end

WUR libchannel_sdl_EXPORT int sd_channel_s_create(sd_channel_s *this,const char *a_ip,unsigned short a_port,
    sd_channel_cbreq_t a_channel_callback,
    void *a_cb_object);
WUR static inline int sd_channel_s_message_call(sd_channel_s *this,unsigned a_index,unsigned a_type,...);
WUR static inline int sd_channel_s_send_message(sd_channel_s *this,unsigned a_index,bc_array_s *a_message);
WUR libchannel_sdl_EXPORT int sd_channel_s_send_multi_message_var(sd_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var);
WUR static inline int sd_channel_s_send_multi_message(sd_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message);
WUR int sd_channel_s_conn_new(void *a_sd_channel,unsigned a_index);
WUR int sd_channel_s_conn_drop(void *a_sd_channel,unsigned a_index);
WUR libchannel_sdl_EXPORT int sd_channel_s_conn_message(void *a_sd_channel,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_sdl_EXPORT int sd_channel_s_fd_event(void *a_sd_channel,unsigned a_index,epoll_event_s *a_epoll_event);

// -- sd_channel_client_s --
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

bc_array_s:buffer

epoll_timer_s:epoll_timer
>
sd_channel_client_s;
@end

WUR libchannel_sdl_EXPORT int sd_channel_client_s_create(sd_channel_client_s *this,
  const char *a_server_ip,unsigned short a_server_port,
  sd_channel_cbreq_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index);
WUR static inline int sd_channel_client_s_message_call(sd_channel_client_s *this,unsigned a_type,...);
WUR static inline int sd_channel_client_s_send_message(sd_channel_client_s *this,bc_array_s *a_message);
WUR static inline int sd_channel_client_s_trace_info(sd_channel_client_s *this,const string_s *a_trace_id);
WUR static inline int sd_channel_client_s_trace_write(sd_channel_client_s *this,const string_s *a_trace_id,ulli a_time,const bc_array_s *a_data);
WUR static inline int sd_channel_client_s_trace_read(sd_channel_client_s *this,const string_s *a_trace_id,lli a_record_id);
WUR static inline int sd_channel_client_s_trace_head(sd_channel_client_s *this,const string_s *a_trace_id);
WUR static inline int sd_channel_client_s_trace_tail(sd_channel_client_s *this,const string_s *a_trace_id);
WUR static inline int sd_channel_client_s_trace_range(sd_channel_client_s *this,const string_s *a_trace_id,lli a_first_id,lli a_last_id,lli a_count);
WUR static inline int sd_channel_client_s_trace_step_range(sd_channel_client_s *this,const string_s *a_trace_id,lli a_first_id,lli a_last_id,ulli a_step);
WUR static inline int sd_channel_client_s_trace_time_range(sd_channel_client_s *this,const string_s *a_trace_id,ulli a_first_time,ulli a_last_time,lli a_count);
WUR static inline int sd_channel_client_s_trace_lee_time(sd_channel_client_s *this,const string_s *a_trace_id,ulli a_time);
WUR static inline int sd_channel_client_s_trace_gre_time(sd_channel_client_s *this,const string_s *a_trace_id,ulli a_time);
WUR static inline int sd_channel_client_s_trace_watch(sd_channel_client_s *this,const string_s *a_trace_id);
WUR static inline int sd_channel_client_s_trace_ignore(sd_channel_client_s *this,const string_s *a_trace_id);
WUR libchannel_sdl_EXPORT int sd_channel_client_s_conn_message(void *a_sd_channel_client,unsigned a_index,const bc_array_s *a_message);
WUR libchannel_sdl_EXPORT int sd_channel_client_s_fd_event(void *a_sd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event);
WUR libchannel_sdl_EXPORT int sd_channel_client_s_connect_time_event(void *a_sd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event);

// === inline methods of generated structures ==================================

// -- bc_array_s --

static inline void bc_array_s_append_sd_segmentd_msg_header(bc_array_s *this,
    ulli a_id,usi a_type,usi a_req_id,const string_s *a_segment_id)
{/*{{{*/
  bc_array_s_append_be_ulli(this,a_id);
  bc_array_s_append_be_usi(this,a_type);
  bc_array_s_append_be_usi(this,a_req_id);
  bc_array_s_append(this,a_segment_id->size,a_segment_id->data);
}/*}}}*/

// -- sd_range_record_s --
@begin
inlines sd_range_record_s
@end

// -- sd_range_records_s --
@begin
inlines sd_range_records_s
@end

// -- sd_channel_s --
@begin
inlines sd_channel_s
@end

static inline int sd_channel_s_message_call(sd_channel_s *this,unsigned a_index,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((sd_channel_cbreq_t)this->channel_callback)(this->cb_object,a_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int sd_channel_s_send_message(sd_channel_s *this,unsigned a_index,bc_array_s *a_message)
{/*{{{*/

  // - log message -
  log_info_2("channel server %u, --> %u bytes",a_index,a_message->used);

  // - schedule message to send -
  if (channel_conn_s_schedule_message(&this->server.conn_list.data[a_index].object,a_message))
  {
    throw_error(SD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_s_send_multi_message(sd_channel_s *this,const ui_array_s *a_indexes,bc_array_s *a_message)
{/*{{{*/
  VAR_CLEAR(data_var,loc_s_channel_message_buffer_swap(a_message));

  if (sd_channel_s_send_multi_message_var(this,a_indexes,data_var))
  {
    throw_error(SD_CHANNEL_SERVER_SEND_MULTI_MESSAGE_VAR_ERROR);
  }

  return 0;
}/*}}}*/

// -- sd_channel_client_s --
@begin
inlines sd_channel_client_s
@end

static inline int sd_channel_client_s_message_call(sd_channel_client_s *this,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((sd_channel_cbreq_t)this->channel_callback)(this->cb_object,this->cb_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int sd_channel_client_s_send_message(sd_channel_client_s *this,bc_array_s *a_message)
{/*{{{*/

  // - log message -
  log_info_2("channel client, --> %u bytes",a_message->used);

  if (channel_conn_s_schedule_message(&this->connection,a_message))
  {
    throw_error(SD_CHANNEL_CLIENT_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_info(sd_channel_client_s *this,const string_s *a_trace_id)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_INFO,a_trace_id);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_write(sd_channel_client_s *this,
    const string_s *a_trace_id,ulli a_time,const bc_array_s *a_data)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_WRITE,a_trace_id);
  bc_array_s_append_be_ulli(&this->buffer,a_time);
  bc_array_s_append(&this->buffer,a_data->used,a_data->data);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_read(sd_channel_client_s *this,const string_s *a_trace_id,lli a_record_id)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_READ,a_trace_id);
  bc_array_s_append_be_lli(&this->buffer,a_record_id);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_head(sd_channel_client_s *this,const string_s *a_trace_id)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_HEAD,a_trace_id);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_tail(sd_channel_client_s *this,const string_s *a_trace_id)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_TAIL,a_trace_id);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_range(sd_channel_client_s *this,const string_s *a_trace_id,lli a_first_id,lli a_last_id,lli a_count)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_RANGE,a_trace_id);
  bc_array_s_append_be_lli(&this->buffer,a_first_id);
  bc_array_s_append_be_lli(&this->buffer,a_last_id);
  bc_array_s_append_be_lli(&this->buffer,a_count);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_step_range(sd_channel_client_s *this,const string_s *a_trace_id,lli a_first_id,lli a_last_id,ulli a_step)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_STEP_RANGE,a_trace_id);
  bc_array_s_append_be_lli(&this->buffer,a_first_id);
  bc_array_s_append_be_lli(&this->buffer,a_last_id);
  bc_array_s_append_be_ulli(&this->buffer,a_step);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_time_range(sd_channel_client_s *this,const string_s *a_trace_id,ulli a_first_time,ulli a_last_time,lli a_count)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_TIME_RANGE,a_trace_id);
  bc_array_s_append_be_ulli(&this->buffer,a_first_time);
  bc_array_s_append_be_ulli(&this->buffer,a_last_time);
  bc_array_s_append_be_lli(&this->buffer,a_count);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_lee_time(sd_channel_client_s *this,const string_s *a_trace_id,ulli a_time)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_LEE_TIME,a_trace_id);
  bc_array_s_append_be_ulli(&this->buffer,a_time);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_gre_time(sd_channel_client_s *this,const string_s *a_trace_id,ulli a_time)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_GRE_TIME,a_trace_id);
  bc_array_s_append_be_ulli(&this->buffer,a_time);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_watch(sd_channel_client_s *this,const string_s *a_trace_id)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_WATCH,a_trace_id);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sd_channel_client_s_trace_ignore(sd_channel_client_s *this,const string_s *a_trace_id)
{/*{{{*/
  this->buffer.used = 0;
  bc_array_s_append_sd_segmentd_msg_header(&this->buffer,
      ++this->message_id,sd_channel_msg_type_REQUEST,sd_channel_cbreq_TRACE_IGNORE,a_trace_id);

  if (sd_channel_client_s_send_message(this,&this->buffer))
  {
    throw_error(SD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

#endif

