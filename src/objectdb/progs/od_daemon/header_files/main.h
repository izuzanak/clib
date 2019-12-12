
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_process.h"
include "od_conf.h"
include "od_channel.h"
include "od_odb.h"
@end

// - error codes -
#define ERROR_OD_DAEMON_CONFIG_DATA_ERROR 1
#define ERROR_OD_DAEMON_EPOLL_WAIT_ERROR 2

#define ERROR_OD_DAEMON_CHANNEL_CREATE_ERROR 1
#define ERROR_OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR 2
#define ERROR_OD_DAEMON_PROCESS_UPDATES_ERROR 3

// === definition of generated structures ======================================

// -- od_daemon_s --
@begin
struct
<
bi:config_changed_flag
od_config_s:config
od_config_s:last_config

odb_database_s:database
od_channel_s:channel
var_array_s:channel_watches

$// - temporary buffers -
bc_array_s:buffer
ui_array_s:indexes
var_array_s:nodes

epoll_s:epoll
>
od_daemon_s;
@end

WUR int od_daemon_s_create(od_daemon_s *this);
WUR int od_daemon_s_process_config(od_daemon_s *this);
WUR int od_daemon_s_run(od_daemon_s *this);

WUR int od_daemon_s_process_updates(od_daemon_s *this,const string_s *a_path);
WUR int od_daemon_s_channel_callback(void *a_sd_daemon,unsigned a_index,unsigned a_type,va_list a_ap);

// === inline methods of generated structures ==================================

// -- od_daemon_s --
@begin
inlines od_daemon_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

