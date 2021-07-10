
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_crc.h"
include "cl_process.h"
include "wd_channel.h"
include "od_conf.h"
include "od_channel.h"
include "od_odb.h"
@end

// - error codes -
#define ERROR_OD_DAEMON_CONFIG_DATA_ERROR 1
#define ERROR_OD_DAEMON_EPOLL_WAIT_ERROR 2
#define ERROR_OD_DAEMON_EPOLL_ERROR 3
#define ERROR_OD_DAEMON_TIMER_SETTIME_ERROR 4
#define ERROR_OD_DAEMON_TIMER_READ_ERROR 5

#define ERROR_OD_DAEMON_CHANNEL_CREATE_ERROR 1
#define ERROR_OD_DAEMON_WATCHDOG_CHANNEL_CREATE_ERROR 2
#define ERROR_OD_DAEMON_STORAGE_FILE_OPEN_ERROR 3
#define ERROR_OD_DAEMON_STORAGE_READ_ERROR 4
#define ERROR_OD_DAEMON_STORAGE_WRITE_ERROR 5
#define ERROR_OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR 6
#define ERROR_OD_DAEMON_PROCESS_UPDATES_ERROR 7

#define ERROR_OD_STORAGE_FILE_READ_ERROR 1
#define ERROR_OD_STORAGE_FILE_WRITE_ERROR 2

// === constants and definitions ===============================================

#define ODB_RECORD_HEADER_SIZE (sizeof(unsigned) + sizeof(usi))

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

file_s:storage

wd_channel_client_s:watchdog_channel
epoll_timer_s:watchdog_timer

$// - temporary buffers -
bc_array_s:buffer
ui_array_s:indexes
var_array_s:nodes
>
od_daemon_s;
@end

WUR int od_daemon_s_create(od_daemon_s *this);
WUR int od_daemon_s_process_config(od_daemon_s *this);
WUR int od_daemon_s_run(od_daemon_s *this);

void od_daemon_s_storage_record(od_daemon_s *this,const string_s *a_path,var_s a_data_var);
WUR int od_daemon_s_storage_read(od_daemon_s *this);
WUR int od_daemon_s_storage_write(od_daemon_s *this,const string_s *a_path,var_s a_data_var);

WUR int od_daemon_s_process_updates(od_daemon_s *this,const string_s *a_path,var_s a_data_var);
WUR int od_daemon_s_channel_callback(void *a_od_daemon,unsigned a_index,unsigned a_type,va_list a_ap);
WUR int od_daemon_s_watchdog_channel_callback(void *a_od_daemon,unsigned a_index,unsigned a_type,va_list a_ap);
WUR int od_daemon_s_watchdog_time_event(void *a_od_daemon,unsigned a_index,epoll_event_s *a_epoll_event);

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

