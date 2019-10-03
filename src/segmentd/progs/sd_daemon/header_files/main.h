
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_process.h"
include "cl_channel.h"
include "sd_channel.h"
include "sd_trace.h"
@end

// - error codes -
#define ERROR_SD_DAEMON_CONFIG_READ_ERROR 1
#define ERROR_SD_DAEMON_CONFIG_DATA_ERROR 2
#define ERROR_SD_DAEMON_EPOLL_WAIT_ERROR 3
#define ERROR_SD_DAEMON_SEGMENT_CREATE_ERROR 4
#define ERROR_SD_DAEMON_SEGMENT_NOT_EXIST 5
#define ERROR_SD_DAEMON_SEGMENT_INVALID_WRITE_SIZE 6
#define ERROR_SD_DAEMON_SEGMENT_WRITE_ERROR 7
#define ERROR_SD_DAEMON_TRACE_CREATE_ERROR 8
#define ERROR_SD_DAEMON_TRACE_NOT_EXIST 9
#define ERROR_SD_DAEMON_TRACE_INVALID_WRITE_SIZE 10
#define ERROR_SD_DAEMON_TRACE_WRITE_ERROR 11

#define ERROR_SD_DAEMON_CHANNEL_CREATE_ERROR 20
#define ERROR_SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR 21

// === definition of generated structures ======================================

// -- sd_segment_tree_s --
@begin
safe_rb_tree<sd_segment_descr_s> sd_segment_tree_s;
@end

// -- sd_trace_tree_s --
@begin
safe_rb_tree<sd_trace_descr_s> sd_trace_tree_s;
@end

// -- sd_daemon_s --
@begin
struct
<
bi:config_changed_flag
sd_config_s:config
sd_config_s:last_config

sd_segment_tree_s:segments
sd_trace_tree_s:traces

sd_channel_s:channel

bc_array_s:buffer
epoll_s:epoll
>
sd_daemon_s;
@end

WUR int sd_daemon_s_create(sd_daemon_s *this);

WUR int sd_daemon_s_process_config(sd_daemon_s *this);
WUR int sd_daemon_s_update_segments(sd_daemon_s *this);
WUR int sd_daemon_s_update_traces(sd_daemon_s *this);

WUR int sd_daemon_s_run(sd_daemon_s *this);

WUR int sd_daemon_s_channel_callback(void *a_sd_daemon,unsigned a_index,unsigned a_type,va_list a_ap);

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

// -- sd_daemon_s --
@begin
inlines sd_daemon_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

