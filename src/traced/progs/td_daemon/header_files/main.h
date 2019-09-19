
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_process.h"
include "cl_channel.h"
include "cl_trace.h"
include "td_conf.h"
@end

// - error codes -
#define ERROR_TD_DAEMON_CONFIG_READ_ERROR 1
#define ERROR_TD_DAEMON_CONFIG_DATA_ERROR 2
#define ERROR_TD_DAEMON_EPOLL_WAIT_ERROR 3

// === definition of generated structures ======================================

// -- td_daemon_s --
@begin
struct
<
bi:config_changed_flag
td_config_s:config
td_config_s:last_config

epoll_s:epoll
>
td_daemon_s;
@end

WUR int td_daemon_s_create(td_daemon_s *this);
WUR int td_daemon_s_process_config(td_daemon_s *this);
WUR int td_daemon_s_run(td_daemon_s *this);

// === inline methods of generated structures ==================================

// -- td_daemon_s --
@begin
inlines td_daemon_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

