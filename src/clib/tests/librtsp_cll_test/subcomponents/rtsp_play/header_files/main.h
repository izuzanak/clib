
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_regex.h"
include "cl_rtsp.h"
@end

// - error codes -
#define ERROR_PLAYER_RTSP_SERVER_CREATE_ERROR 1
#define ERROR_PLAYER_INVALID_MEDIA_URL 2
#define ERROR_PLAYER_FILE_OPEN_ERROR 3
#define ERROR_PLAYER_FILE_READ_ERROR 4
#define ERROR_PLAYER_SERVER_FD_EVENT_ERROR 5

// === definition of generated structures ======================================

// -- rtsp_reader_s --
@begin
struct
<
file_s:stream_file
>
rtsp_reader_s;
@end

// -- rtsp_readers_s --
@begin
array<rtsp_reader_s> rtsp_readers_s;
@end

// -- rtsp_player_s --
@begin
struct
<
bc_array_s:buffer
regex_s:url_path_regex
regmatch_array_s:match_array

string_s:base_dir
string_s:ip
usi:port
rtsp_readers_s:readers
rtsp_server_s:server
>
rtsp_player_s;
@end

WUR int rtsp_player_s_create(rtsp_player_s *this,const char *a_base_dir,const char *a_ip,usi a_port);
void rtsp_player_s_run(rtsp_player_s *this);
WUR int rtsp_player_s_server_fd_event(void *a_rtsp_player,unsigned a_index,epoll_event_s *a_epoll_event);
WUR int rtsp_player_s_conn_new(void *a_rtsp_player,unsigned a_index);
WUR int rtsp_player_s_conn_drop(void *a_rtsp_player,unsigned a_index);
WUR int rtsp_player_s_conn_get_sdp(void *a_rtsp_player,unsigned a_index,rtsp_parser_s *a_parser,bc_array_s *a_trg);
WUR int rtsp_player_s_conn_check_media(void *a_rtsp_player,unsigned a_index,const char *a_url,unsigned *a_channel);
WUR int rtsp_player_s_conn_playing(void *a_rtsp_player,unsigned a_index,rtsp_play_options_s *a_options);
WUR int rtsp_player_s_conn_get_packet(void *a_rtsp_player,unsigned a_index,ulli *a_delay,bc_block_s *a_trg);

// === inline methods of generated structures ==================================

// -- rtsp_reader_s --
@begin
inlines rtsp_reader_s
@end

// -- rtsp_readers_s --
@begin
inlines rtsp_readers_s
@end

// -- rtsp_player_s --
@begin
inlines rtsp_player_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

