
#ifndef WD_PROG_MAIN_H
#define WD_PROG_MAIN_H

@begin
include "cl_crc.h"
include "cl_process.h"
include "wd_conf.h"
include "wd_channel.h"
@end

// - error codes -
#define ERROR_WD_DAEMON_CONFIG_DATA_ERROR 1
#define ERROR_WD_DAEMON_EPOLL_WAIT_ERROR 2
#define ERROR_WD_DAEMON_EPOLL_ERROR 3
#define ERROR_WD_DAEMON_TIMER_READ_ERROR 4
#define ERROR_WD_DAEMON_INVALID_MONITOR_NAME 5
#define ERROR_WD_DAEMON_GET_TIME_ERROR 6

#define ERROR_WD_DAEMON_CHANNEL_CREATE_ERROR 1
#define ERROR_WD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR 2

#define ERROR_WD_DAEMON_WATCHDOG_OPEN_ERROR 1
#define ERROR_WD_DAEMON_WATCHDOG_WRITE_ERROR 2

// === definition of generated structures ======================================

// -- wd_monitor_s --
@begin
struct
<
string_s:name
lli:timeout

time_s:enable_time
time_s:keepalive_time

epoll_timer_s:timer
>
wd_monitor_s;
@end

// -- wd_monitor_tree_s --
@begin
safe_rb_tree<wd_monitor_s> wd_monitor_tree_s;
@end

// -- wd_daemon_s --
@begin
struct
<
bi:config_changed_flag
wd_config_s:config
wd_config_s:last_config

wd_channel_s:channel
ui_array_s:channel_watches
wd_monitor_tree_s:monitor_tree
bi:monitors_ok

fd_s:wdg_fd
epoll_timer_s:wdg_timer

$// - temporary buffers -
bc_array_s:buffer
ui_array_s:indexes
var_array_s:nodes
>
wd_daemon_s;
@end

WUR int wd_daemon_s_create(wd_daemon_s *this);
WUR int wd_daemon_s_process_config(wd_daemon_s *this);
WUR int wd_daemon_s_run(wd_daemon_s *this);

WUR int wd_daemon_s_channel_callback(void *a_wd_daemon,unsigned a_index,unsigned a_type,va_list a_ap);
WUR int wd_daemon_s_watchdog_time_event(void *a_wd_daemon,unsigned a_index,epoll_event_s *a_epoll_event);
WUR int wd_daemon_s_monitor_time_event(void *a_wd_daemon,unsigned a_index,epoll_event_s *a_epoll_event);

// === inline methods of generated structures ==================================

// -- wd_monitor_s --
@begin
inlines wd_monitor_s
@end

// -- wd_monitor_tree_s --
@begin
inlines wd_monitor_tree_s
@end

static inline int wd_monitor_tree_s___compare_value(const wd_monitor_tree_s *this,const wd_monitor_s *a_first,const wd_monitor_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->name;
  const string_s *second = &a_second->name;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }
  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- wd_daemon_s --
@begin
inlines wd_daemon_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

