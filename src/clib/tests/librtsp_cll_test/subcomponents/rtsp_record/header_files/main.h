
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_rtsp.h"
@end

// - error codes -
#define ERROR_RECORDER_FILE_OPEN_ERROR 1
#define ERROR_RECORDER_FILE_WRITE_ERROR 2
#define ERROR_RECORDER_GET_TIME_ERROR 3
#define ERROR_RECORDER_EPOLL_ERROR 4

// === definition of generated structures ======================================

// -- rtsp_client_list_s --
@begin
list<rtsp_client_s> rtsp_client_list_s;
@end

// -- rtsp_record_s --
@begin
struct
<
string_s:server_ip
usi:server_port
string_s:media
string_s:file_name
ui:client_idx
ulli:last_pkt_time
file_s:stream_file
epoll_timer_s:epoll_timer
>
rtsp_record_s;
@end

// -- rtsp_records_s --
@begin
array<rtsp_record_s> rtsp_records_s;
@end

// -- rtsp_recorder_s --
@begin
struct
<
bc_array_s:buffer
string_s:base_dir
rtsp_records_s:records
rtsp_client_list_s:client_list
epoll_s:epoll
>
rtsp_recorder_s;
@end

WUR int rtsp_recorder_s_create(rtsp_recorder_s *this,const char *a_base_dir,unsigned a_count,
  const char **a_server_ips,const usi *a_server_ports,const char **a_media,const char **a_file_names);
void rtsp_recorder_s_run(rtsp_recorder_s *this);
WUR int rtsp_recorder_s_record_time_event(void *a_rtsp_recorder,unsigned a_index,unsigned a_timer,epoll_s *a_epoll);
WUR int rtsp_recorder_s_client_fd_event(void *a_rtsp_recorder,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR int rtsp_recorder_s_recv_sdp(void *a_rtsp_recorder,unsigned a_index,const bc_array_s *a_src);
WUR int rtsp_recorder_s_recv_packet(void *a_rtsp_recorder,unsigned a_index,const bc_array_s *a_src);

// === inline methods of generated structures ==================================

// -- rtsp_client_list_s --
@begin
inlines rtsp_client_list_s
@end

// -- rtsp_record_s --
@begin
inlines rtsp_record_s
@end

// -- rtsp_records_s --
@begin
inlines rtsp_records_s
@end

// -- rtsp_recorder_s --
@begin
inlines rtsp_recorder_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

