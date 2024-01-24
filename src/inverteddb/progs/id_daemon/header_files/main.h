
#ifndef ID_PROG_MAIN_H
#define ID_PROG_MAIN_H

@begin
include "cl_crc.h"
include "cl_process.h"
include "cl_zlib.h"
include "cl_crypto.h"
include "wd_channel.h"
include "id_conf.h"
include "id_channel.h"
include "id_idb.h"
@end

// - error codes -
#define ERROR_ID_DAEMON_CONFIG_DATA_ERROR 1
#define ERROR_ID_DAEMON_EPOLL_WAIT_ERROR 2
#define ERROR_ID_DAEMON_EPOLL_ERROR 3
#define ERROR_ID_DAEMON_TIMER_SETTIME_ERROR 4
#define ERROR_ID_DAEMON_TIMER_READ_ERROR 5

#define ERROR_ID_DAEMON_CHANNEL_CREATE_ERROR 1
#define ERROR_ID_DAEMON_WATCHDOG_CHANNEL_CREATE_ERROR 2
#define ERROR_ID_DAEMON_STORAGE_FILE_OPEN_ERROR 3
#define ERROR_ID_DAEMON_STORAGE_READ_ERROR 4
#define ERROR_ID_DAEMON_STORAGE_WRITE_ERROR 5
#define ERROR_ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR 6
#define ERROR_ID_DAEMON_PROCESS_UPDATES_ERROR 7

#define ERROR_ID_DAEMON_DATABASE_CREATE_ERROR 8
#define ERROR_ID_DAEMON_DATABASE_NOT_EXIST 9
#define ERROR_ID_DAEMON_DATABASE_UPDATE_EXTRACTOR_ERROR 10
#define ERROR_ID_DAEMON_DATABASE_UPDATE_INDEX_ERROR 11
#define ERROR_ID_DAEMON_DATABASE_REMOVE_DOCS_ERROR 12
#define ERROR_ID_DAEMON_DATABASE_QUERY_ERROR 13

#define ERROR_ID_DAEMON_GZIP_COMPRESSION_ERROR 14

// === definition of generated structures ======================================

// -- id_daemon_s --
@begin
struct
<
bi:config_changed_flag
id_config_s:config
id_config_s:last_config

idb_database_tree_s:databases

id_channel_s:channel
var_array_s:channel_watches

wd_channel_client_s:watchdog_channel
epoll_timer_s:watchdog_timer

$// - temporary buffers -
bc_array_s:compressed
bc_array_s:buffer
>
id_daemon_s;
@end

WUR int id_daemon_s_create(id_daemon_s *this);
WUR int id_daemon_s_process_config(id_daemon_s *this);
WUR int id_daemon_s_update_databases(id_daemon_s *this);
WUR int id_daemon_s_run(id_daemon_s *this);

void id_daemon_bits_result_to_range_offsets(idb_bits_tree_s *a_bits_res,ui_array_s *a_ranges);

WUR int id_daemon_s_channel_callback(void *a_id_daemon,unsigned a_index,unsigned a_type,va_list a_ap);
WUR int id_daemon_s_watchdog_channel_callback(void *a_id_daemon,unsigned a_index,unsigned a_type,va_list a_ap);
WUR int id_daemon_s_watchdog_time_event(void *a_id_daemon,unsigned a_index,epoll_event_s *a_epoll_event);

// === inline methods of generated structures ==================================

// -- id_daemon_s --
@begin
inlines id_daemon_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

