
#ifndef __CL_CHANNEL_H
#define __CL_CHANNEL_H

@begin
include "cl_linux.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libchannel_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libchannel_cll_EXPORTS
#define libchannel_cll_EXPORT __declspec(dllexport)
#else
#define libchannel_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_CHANNEL_SERVER_LISTEN_ERROR 1
#define ERROR_CHANNEL_SERVER_INVALID_FD 2
#define ERROR_CHANNEL_SERVER_ACCEPT_ERROR 3

#define ERROR_CHANNEL_CONN_INVALID_FD 1
#define ERROR_CHANNEL_CONN_CONNECT_ERROR 2
#define ERROR_CHANNEL_CONN_SEND_ERROR 3
#define ERROR_CHANNEL_CONN_WRITE_ERROR 4
#define ERROR_CHANNEL_CONN_RECEIVE_ERROR 5
#define ERROR_CHANNEL_CONN_READ_ERROR 6
#define ERROR_CHANNEL_CONN_CALLBACK_ERROR 7
#define ERROR_CHANNEL_CONN_EPOLL_ERROR 8

typedef struct channel_server_s channel_server_s;
typedef void (*channel_conn_new_callback_t)(void *a_object,unsigned a_index);
typedef void (*channel_conn_drop_callback_t)(void *a_object,unsigned a_index);
typedef int (*channel_conn_message_callback_t)(void *a_object,unsigned a_index,const bc_array_s *a_message);

// === definition of generated structures ======================================

// -- bc_array_queue_s --
@begin
queue<bc_array_s> bc_array_queue_s;
@end

// -- channel_conn_s --
@begin
struct
<
ui:index
epoll_fd_s:epoll_fd

pointer:conn_message_callback
pointer:cb_object

bc_array_s:in_msg
ui:in_msg_length

bc_array_queue_s:out_msg_queue
ui:out_msg_offset
>
channel_conn_s;
@end

libchannel_cll_EXPORT void channel_conn_s_create(channel_conn_s *this,unsigned a_index,epoll_fd_s *a_epoll_fd,
    channel_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object);
WUR libchannel_cll_EXPORT int channel_conn_s_create_client(channel_conn_s *this,
    const char *a_server_ip,unsigned short a_server_port,unsigned a_index,
    channel_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object);
WUR int channel_conn_s_recv_msg(channel_conn_s *this);
WUR int channel_conn_s_send_msg(channel_conn_s *this);
WUR libchannel_cll_EXPORT int channel_conn_s_fd_event(channel_conn_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR libchannel_cll_EXPORT int channel_conn_s_schedule_message(channel_conn_s *this,bc_array_s *a_message);

// -- channel_conn_list_s --
@begin
list<channel_conn_s> channel_conn_list_s;
@end

// -- channel_server_s --
@begin
struct
<
string_s:ip
usi:port

pointer:conn_new_callback
pointer:conn_drop_callback
pointer:conn_message_callback
pointer:cb_object

epoll_fd_s:epoll_fd
channel_conn_list_s:conn_list
>
channel_server_s;
@end

WUR libchannel_cll_EXPORT int channel_server_s_create(channel_server_s *this,
    const string_s *a_ip,unsigned short a_port,
    channel_conn_new_callback_t a_conn_new_callback,
    channel_conn_drop_callback_t a_conn_drop_callback,
    channel_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object);
WUR libchannel_cll_EXPORT int channel_server_s_fd_event(channel_server_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
libchannel_cll_EXPORT void channel_server_s_conn_fd_event(void *a_channel_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// === inline methods of generated structures ==================================

// -- bc_array_queue_s --
@begin
inlines bc_array_queue_s
@end

// -- channel_conn_s --
@begin
inlines channel_conn_s
@end

// -- channel_conn_list_s --
@begin
inlines channel_conn_list_s
@end

// -- channel_server_s --
@begin
inlines channel_server_s
@end

#endif

