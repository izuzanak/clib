
#ifndef HR_PROG_MAIN_H
#define HR_PROG_MAIN_H

@begin
include "cl_process.h"
include "cl_microhttp.h"
include "hr_conf.h"
@end

// - error codes -
#define ERROR_HR_DAEMON_URL_REGEX_CREATE_ERROR 1
#define ERROR_HR_DAEMON_CONFIG_DATA_ERROR 2
#define ERROR_HR_DAEMON_EPOLL_WAIT_ERROR 3
#define ERROR_HR_DAEMON_EPOLL_ERROR 4
#define ERROR_HR_DAEMON_TIMER_SETTIME_ERROR 5
#define ERROR_HR_DAEMON_TIMER_READ_ERROR 6

#define ERROR_RECV_HTTP_SERVER_INVALID_ADDRESS 10
#define ERROR_RECV_HTTP_SERVER_CREATE_ERROR 11
#define ERROR_RECV_HTTP_SERVER_UPDATE_FDS_ERROR 12
#define ERROR_RECV_HTTP_SERVER_CHECK_ERROR 13
#define ERROR_RECV_HTTP_SERVER_PROCESS_ERROR 14
#define ERROR_RECV_HTTP_CONN_CLIENT_IP_ERROR 15
#define ERROR_RECV_HTTP_CONN_QUEUE_RESPONSE_ERROR 16

#define ERROR_RECV_GET_TIME_ERROR 20
#define ERROR_RECV_UPLOAD_MAKE_DIR_ERROR 21
#define ERROR_RECV_UPLOAD_FILE_WRITE_ERROR 22

// === constants and definitions ===============================================

// === definition of generated structures ======================================

// -- hr_recv_data_s --
@begin
struct
<
bc_array_s:ip
bc_array_s:data
>
hr_recv_data_s;
@end

// -- hr_recv_data_list_s --
@begin
list<hr_recv_data_s> hr_recv_data_list_s;
@end

// -- hr_daemon_s --
@begin
struct
<
regex_s:url_regex
regmatch_array_s:url_match_array

bi:config_changed_flag
hr_config_s:config
hr_config_s:last_config

$// -- http --
http_epoll_s:recv_http
hr_recv_data_list_s:recv_data_list

$// -- shared --
bc_array_s:ip_buffer
bc_array_s:dir_buffer
bc_array_s:file_buffer
>
hr_daemon_s;
@end

WUR int hr_daemon_s_create(hr_daemon_s *this);
WUR int hr_daemon_s_process_config(hr_daemon_s *this);
WUR int hr_daemon_s_run(hr_daemon_s *this);

// -- http --
WUR int hr_recv_http_connection_callback(http_conn_s *a_conn);
WUR int hr_recv_http_completed_callback(http_conn_s *a_conn);

// === inline methods of generated structures ==================================

// -- hr_recv_data_s --
@begin
inlines hr_recv_data_s
@end

// -- hr_recv_data_list_s --
@begin
inlines hr_recv_data_list_s
@end

// -- hr_daemon_s --
@begin
inlines hr_daemon_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

