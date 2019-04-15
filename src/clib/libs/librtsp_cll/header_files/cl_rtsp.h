
#ifndef __CL_RTSP_H
#define __CL_RTSP_H

@begin
include "cl_linux.h"
include "cl_rtsp_parser.h"
include "cl_rtsp_sdp_parser.h"
@end

#include <netinet/tcp.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define librtsp_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef librtsp_cll_EXPORTS
#define librtsp_cll_EXPORT __declspec(dllexport)
#else
#define librtsp_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_RTSP_CLIENT_INVALID_STATE 1
#define ERROR_RTSP_CLIENT_INVALID_FD 2
#define ERROR_RTSP_CLIENT_UNKNOWN_COMMAND 3
#define ERROR_RTSP_CLIENT_CONNECT_ERROR 4
#define ERROR_RTSP_CLIENT_READ_ERROR 5
#define ERROR_RTSP_CLIENT_PARSE_ERROR 6
#define ERROR_RTSP_CLIENT_CALLBACK_ERROR 7
#define ERROR_RTSP_CLIENT_SEND_ERROR 8
#define ERROR_RTSP_CLIENT_RECEIVE_ERROR 9
#define ERROR_RTSP_CLIENT_SESSION_ERROR 10

#define ERROR_RTSP_CONN_INVALID_STATE 1
#define ERROR_RTSP_CONN_INVALID_FD 2
#define ERROR_RTSP_CONN_UNKNOWN_COMMAND 3
#define ERROR_RTSP_CONN_READ_ERROR 4
#define ERROR_RTSP_CONN_PARSE_ERROR 5
#define ERROR_RTSP_CONN_CALLBACK_ERROR 6
#define ERROR_RTSP_CONN_SEND_ERROR 7
#define ERROR_RTSP_CONN_RECEIVE_ERROR 8
#define ERROR_RTSP_CONN_GET_TIME_ERROR 9
#define ERROR_RTSP_CONN_NEXT_PACKET_ERROR 10
#define ERROR_RTSP_CONN_SEND_PACKET_ERROR 11
#define ERROR_RTSP_CONN_INVALID_PACKET_CHANNEL 12
#define ERROR_RTSP_CONN_SETSOCKOPT_ERROR 13
#define ERROR_RTSP_CONN_UDP_SETUP_ERROR 14
#define ERROR_RTSP_CONN_MISMATCH_RTSP_TRANSPORT 15

#define ERROR_RTSP_SERVER_INVALID_STATE 1
#define ERROR_RTSP_SERVER_INVALID_FD 2
#define ERROR_RTSP_SERVER_LISTEN_ERROR 3
#define ERROR_RTSP_SERVER_ACCEPT_ERROR 4
#define ERROR_RTSP_SERVER_CONN_CREATE_ERROR 5

// === constants and definitions ===============================================

#define RTP_PKT_GET_CHANNEL(PACKET) ((PACKET)[sizeof(rtsp_pkt_delay_t) + 1])
#define RTP_PKT_GET_SEQUENCE(PACKET) ntohs(*((usi *)(this->packet.data + sizeof(rtsp_pkt_delay_t) + 6)))
#define RTP_PKT_SET_SEQUENCE(PACKET,VALUE) *((usi *)((PACKET) + sizeof(rtsp_pkt_delay_t) + 6)) = htons(VALUE)

enum
{/*{{{*/
  c_rtsp_command_RESPONSE      = 0,
  c_rtsp_command_OPTIONS       = 1 << 0,
  c_rtsp_command_DESCRIBE      = 1 << 1,
  c_rtsp_command_SETUP         = 1 << 2,
  c_rtsp_command_TEARDOWN      = 1 << 3,
  c_rtsp_command_PLAY          = 1 << 4,
  c_rtsp_command_PAUSE         = 1 << 5,
  c_rtsp_command_SET_PARAMETER = 1 << 6,
  c_rtsp_command_GET_PARAMETER = 1 << 7,
};/*}}}*/

typedef usi rtsp_pkt_delay_t;

typedef struct rtsp_client_s rtsp_client_s;
typedef int (*rtsp_recv_sdp_callback_t)(void *a_object,unsigned a_index,const bc_array_s *a_src);
typedef int (*rtsp_recv_packet_callback_t)(void *a_object,unsigned a_index,const bc_array_s *a_src);

typedef struct rtsp_server_s rtsp_server_s;
typedef int (*rtsp_conn_new_callback_t)(void *a_object,unsigned a_index);
typedef int (*rtsp_conn_drop_callback_t)(void *a_object,unsigned a_index);
typedef int (*rtsp_conn_get_sdp_callback_t)(void *a_object,unsigned a_index,const char *a_url,bc_array_s *a_trg);
typedef int (*rtsp_conn_check_media_callback_t)(void *a_object,unsigned a_index,const char *a_url);
typedef int (*rtsp_conn_playing_callback_t)(void *a_object,unsigned a_index,ulli a_session);
typedef int (*rtsp_conn_get_packet_callback_t)(void *a_object,unsigned a_index,ulli *a_delay,bc_array_s *a_trg);

// === definition of generated structures ======================================

// -- rtsp_client_s --

enum
{/*{{{*/
  c_rtsp_client_state_UNKNOWN = 0,
  c_rtsp_client_state_ERROR,
  c_rtsp_client_state_RECV_OPTIONS,
  c_rtsp_client_state_RECV_DESCRIBE,
  c_rtsp_client_state_RECV_SDP,
  c_rtsp_client_state_RECV_SETUP_VIDEO,
  c_rtsp_client_state_RECV_SETUP_AUDIO,
  c_rtsp_client_state_RECV_PLAY_OR_DATA,
};/*}}}*/

@begin
struct
<
string_s:server_ip
usi:server_port
string_s:media_url

pointer:recv_sdp_callback
pointer:recv_packet_callback
pointer:cb_object
ui:cb_index

epoll_fd_s:epoll_fd
bc_array_s:in_msg
bc_array_s:out_msg
rtsp_parser_s:parser

string_s:video_control
string_s:audio_control
ui:state
ui:sequence
string_s:session
>
rtsp_client_s;
@end

WUR librtsp_cll_EXPORT int rtsp_client_s_create(rtsp_client_s *this,
    const string_s *a_server_ip,unsigned short a_server_port,const string_s *a_media,
    rtsp_recv_sdp_callback_t a_recv_sdp_callback,
    rtsp_recv_packet_callback_t a_recv_packet_callback,
    void *a_cb_object,unsigned a_cb_index);
WUR int rtsp_client_s_send_cmd(rtsp_client_s *this);
WUR int rtsp_client_s_recv_resp(rtsp_client_s *this);
WUR int rtsp_client_s_recv_cmd_resp_or_data(rtsp_client_s *this);
WUR int rtsp_client_s_recv_sdp(rtsp_client_s *this);
WUR librtsp_cll_EXPORT int rtsp_client_s_fd_event(rtsp_client_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// -- rtsp_setup_s --
@begin
struct
<
string_s:media_url
ui:inter_port_begin
ui:inter_port_end
socket_address_s:udp_data_addr
socket_address_s:udp_ctrl_addr
epoll_fd_s:udp_data
epoll_fd_s:udp_ctrl
>
rtsp_setup_s;
@end

// -- rtsp_setups_s --
@begin
array<rtsp_setup_s> rtsp_setups_s;
@end

// -- rtsp_conn_s --

enum
{/*{{{*/
  c_rtsp_conn_state_UNKNOWN = 0,
  c_rtsp_conn_state_ERROR,
  c_rtsp_conn_state_RECV_COMMAND,
};/*}}}*/

@begin
struct
<
pointer:server
socket_address_s:client_addr
ui:index
epoll_fd_s:epoll_fd
epoll_timer_s:epoll_send_timer

bc_array_s:buffer
bc_array_s:in_msg
bc_array_s:out_msg
rtsp_parser_s:parser

usi:packet_seq_0
usi:packet_seq_2
uc:pkt_channel
bc_array_s:packet
ulli:packet_time

ui:state
ui:sequence
ulli:session
rtsp_setups_s:rtsp_setups
bi:tcp
>
rtsp_conn_s;
@end

librtsp_cll_EXPORT WUR int rtsp_conn_s_create(rtsp_conn_s *this,rtsp_server_s *a_server,
    unsigned a_index,epoll_fd_s *a_epoll_fd,socket_address_s *a_client_addr);
void rtsp_conn_s_append_time(bc_array_s *a_trg);
WUR int rtsp_conn_s_send_resp(rtsp_conn_s *this,bc_array_s *a_msg);
WUR int rtsp_conn_s_recv_cmd(rtsp_conn_s *this,epoll_s *a_epoll);
WUR int rtsp_conn_s_next_packet(rtsp_conn_s *this,epoll_s *a_epoll);
WUR int rtsp_conn_s_send_packet(rtsp_conn_s *this);
WUR librtsp_cll_EXPORT int rtsp_conn_s_time_event(rtsp_conn_s *this,unsigned a_index,unsigned a_timer,epoll_s *a_epoll);
WUR librtsp_cll_EXPORT int rtsp_conn_s_fd_event(rtsp_conn_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// -- rtsp_conn_list_s --
@begin
list<rtsp_conn_s> rtsp_conn_list_s;
@end

// -- rtsp_server_s --
enum
{/*{{{*/
  c_rtsp_server_state_UNKNOWN = 0,
  c_rtsp_server_state_ERROR,
  c_rtsp_server_state_LISTEN,
};/*}}}*/

@begin
struct
<
string_s:ip
usi:port

pointer:conn_new_callback
pointer:conn_drop_callback
pointer:conn_get_sdp_callback
pointer:conn_check_media_callback
pointer:conn_playing_callback
pointer:conn_get_packet_callback
pointer:cb_object

epoll_fd_s:epoll_fd
rtsp_conn_list_s:conn_list

ui:state
>
rtsp_server_s;
@end

WUR librtsp_cll_EXPORT int rtsp_server_s_create(rtsp_server_s *this,
  const string_s *a_ip,unsigned short a_port,
  rtsp_conn_new_callback_t a_conn_new_callback,
  rtsp_conn_drop_callback_t a_conn_drop_callback,
  rtsp_conn_get_sdp_callback_t a_conn_get_sdp_callback,
  rtsp_conn_check_media_callback_t a_conn_check_media_callback,
  rtsp_conn_playing_callback_t a_conn_playing_callback,
  rtsp_conn_get_packet_callback_t a_conn_get_packet_callback,
  void *a_cb_object);
WUR librtsp_cll_EXPORT int rtsp_server_s_fd_event(rtsp_server_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR librtsp_cll_EXPORT int rtsp_server_s_conn_time_event(void *a_rtsp_server,unsigned a_index,unsigned a_timer,epoll_s *a_epoll);
WUR librtsp_cll_EXPORT int rtsp_server_s_conn_fd_event(void *a_rtsp_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// === inline methods of generated structures ==================================

// -- rtsp_client_s --
@begin
inlines rtsp_client_s
@end

// -- rtsp_setup_s --
@begin
inlines rtsp_setup_s
@end

// -- rtsp_setups_s --
@begin
inlines rtsp_setups_s
@end

// -- rtsp_conn_s --
@begin
inlines rtsp_conn_s
@end

// -- rtsp_conn_list_s --
@begin
inlines rtsp_conn_list_s
@end

// -- rtsp_server_s --
@begin
inlines rtsp_server_s
@end

#endif

