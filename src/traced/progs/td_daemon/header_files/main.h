
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_process.h"
include "cl_channel.h"
include "cl_trace.h"
include "td_conf.h"
include "td_channel.h"
@end

// - error codes -
#define ERROR_TD_DAEMON_CONFIG_READ_ERROR 1
#define ERROR_TD_DAEMON_CONFIG_DATA_ERROR 2
#define ERROR_TD_DAEMON_EPOLL_WAIT_ERROR 3
#define ERROR_TD_DAEMON_TRACE_FILE_OPEN_ERROR 4
#define ERROR_TD_DAEMON_TRACE_FILE_MMAP_ERROR 5
#define ERROR_TD_DAEMON_TRACE_CREATE_ERROR 6

#define ERROR_TD_TRACE_MMAP_FILE_ERROR 1

// === definition of generated structures ======================================

// -- td_trace_s --
@begin
struct
<
td_conf_trace_s:config
trace_mmap_s:mmap_header
trace_mmap_s:mmap_trace
trace_mmap_s:mmap_timestamp
trace_s:trace
>
td_trace_s;
@end

WUR static inline int td_trace_s_mmap_file(trace_mmap_s *a_trace_mmap,td_conf_trace_data_s *a_trace_data);

// -- td_trace_tree_s --
@begin
safe_rb_tree<td_trace_s> td_trace_tree_s;
@end

// -- td_daemon_s --
@begin
struct
<
bi:config_changed_flag
td_config_s:config
td_config_s:last_config

td_trace_tree_s:traces

epoll_s:epoll
>
td_daemon_s;
@end

WUR int td_daemon_s_create(td_daemon_s *this);

WUR int td_daemon_s_process_config(td_daemon_s *this);
WUR int td_daemon_s_update_traces(td_daemon_s *this);

WUR int td_daemon_s_run(td_daemon_s *this);

// === inline methods of generated structures ==================================

// -- td_trace_s --
@begin
inlines td_trace_s
@end

static inline int td_trace_s_mmap_file(trace_mmap_s *a_trace_mmap,td_conf_trace_data_s *a_trace_data)
{/*{{{*/
  CONT_INIT_CLEAR(fd_s,fd);
  fd = open(a_trace_data->path.data,O_RDWR,0);

  if (fd == -1)
  {
    throw_error(TD_DAEMON_TRACE_FILE_OPEN_ERROR);
  }

  if (trace_mmap_s_create(a_trace_mmap,NULL,a_trace_data->size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,a_trace_data->offset))
  {
    throw_error(TD_TRACE_MMAP_FILE_ERROR);
  }

  return 0;
}/*}}}*/

// -- td_trace_tree_s --
@begin
inlines td_trace_tree_s
@end

static inline int td_trace_tree_s___compare_value(const td_trace_tree_s *this,const td_trace_s *a_first,const td_trace_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->config.trace_id;
  const string_s *second = &a_second->config.trace_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- td_daemon_s --
@begin
inlines td_daemon_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

