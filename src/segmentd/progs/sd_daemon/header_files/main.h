
#ifndef SD_PROG_MAIN_H
#define SD_PROG_MAIN_H

@begin
include "cl_process.h"
include "wd_channel.h"
include "sd_channel.h"
include "sd_trace.h"
@end

// - error codes -
#define ERROR_SD_DAEMON_CONFIG_DATA_ERROR 1
#define ERROR_SD_DAEMON_EPOLL_WAIT_ERROR 2
#define ERROR_SD_DAEMON_EPOLL_ERROR 3
#define ERROR_SD_DAEMON_TIMER_SETTIME_ERROR 4
#define ERROR_SD_DAEMON_TIMER_READ_ERROR 5
#define ERROR_SD_DAEMON_SEGMENT_CREATE_ERROR 6
#define ERROR_SD_DAEMON_SEGMENT_NOT_EXIST 7
#define ERROR_SD_DAEMON_SEGMENT_INVALID_WRITE_SIZE 8
#define ERROR_SD_DAEMON_SEGMENT_WRITE_ERROR 9
#define ERROR_SD_DAEMON_TRACE_CREATE_ERROR 10
#define ERROR_SD_DAEMON_TRACE_NOT_EXIST 11
#define ERROR_SD_DAEMON_TRACE_INVALID_WRITE_SIZE 12
#define ERROR_SD_DAEMON_TRACE_WRITE_ERROR 13
#define ERROR_SD_DAEMON_TRACE_INIT_TIMESTAMP_ERROR 14

#define ERROR_SD_DAEMON_CHANNEL_CREATE_ERROR 20
#define ERROR_SD_DAEMON_WATCHDOG_CHANNEL_CREATE_ERROR 21
#define ERROR_SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR 22

// === definition of generated structures ======================================

// -- sd_segment_tree_s --
@begin
safe_rb_tree<sd_segment_descr_s> sd_segment_tree_s;
@end

// -- sd_trace_tree_s --
@begin
safe_rb_tree<sd_trace_descr_s> sd_trace_tree_s;
@end

// -- id_tree_s --
@begin
safe_rb_tree<string_s> id_tree_s;
@end

// -- sd_channel_watch_s --
@begin
struct
<
id_tree_s:segment_ids
id_tree_s:trace_ids
>
sd_channel_watch_s;
@end

// -- sd_channel_watches_s --
@begin
array<sd_channel_watch_s> sd_channel_watches_s;
@end

// -- sd_daemon_s --
@begin
struct
<
sd_config_s:config
sd_config_s:last_config

sd_segment_tree_s:segments
sd_trace_tree_s:traces

sd_channel_s:channel
sd_channel_watches_s:channel_watches
bi:update_watches

wd_channel_client_s:watchdog_channel
epoll_timer_s:watchdog_timer

bc_array_s:buffer
>
sd_daemon_s;
@end

WUR int sd_daemon_s_create(sd_daemon_s *this);

WUR int sd_daemon_s_process_config(sd_daemon_s *this);
WUR int sd_daemon_s_update_segments(sd_daemon_s *this,ui_array_s *a_updated);
WUR int sd_daemon_s_update_traces(sd_daemon_s *this);

void sd_daemon_s_do_update_watches(sd_daemon_s *this);
static inline void sd_daemon_s_update_watches(sd_daemon_s *this);

WUR int sd_daemon_s_run(sd_daemon_s *this);

WUR int sd_daemon_s_channel_callback(void *a_sd_daemon,unsigned a_index,unsigned a_type,va_list a_ap);
WUR int sd_daemon_s_watchdog_channel_callback(void *a_sd_daemon,unsigned a_index,unsigned a_type,va_list a_ap);
WUR int sd_daemon_s_watchdog_time_event(void *a_sd_daemon,unsigned a_index,epoll_event_s *a_epoll_event);

// === inline methods of generated structures ==================================

// -- sd_segment_tree_s --
@begin
inlines sd_segment_tree_s
@end

static inline int sd_segment_tree_s___compare_value(const sd_segment_tree_s *this,const sd_segment_descr_s *a_first,const sd_segment_descr_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->config.segment_id;
  const string_s *second = &a_second->config.segment_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- sd_trace_tree_s --
@begin
inlines sd_trace_tree_s
@end

static inline int sd_trace_tree_s___compare_value(const sd_trace_tree_s *this,const sd_trace_descr_s *a_first,const sd_trace_descr_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->config.trace_id;
  const string_s *second = &a_second->config.trace_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- id_tree_s --
@begin
inlines id_tree_s
@end

static inline int id_tree_s___compare_value(const id_tree_s *this,const string_s *a_first,const string_s *a_second)
{/*{{{*/
  (void)this;

  if (a_first->size < a_second->size) { return -1; }
  if (a_first->size > a_second->size) { return 1; }
  return memcmp(a_first->data,a_second->data,a_first->size - 1);
}/*}}}*/

// -- sd_channel_watch_s --
@begin
inlines sd_channel_watch_s
@end

// -- sd_channel_watches_s --
@begin
inlines sd_channel_watches_s
@end

// -- sd_daemon_s --
@begin
inlines sd_daemon_s
@end

static inline void sd_daemon_s_update_watches(sd_daemon_s *this)
{/*{{{*/
  if (this->update_watches)
  {
    sd_daemon_s_do_update_watches(this);
  }
}/*}}}*/

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

