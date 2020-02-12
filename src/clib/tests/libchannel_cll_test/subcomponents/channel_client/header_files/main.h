
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_channel.h"
@end

// - error codes -
#define ERROR_CHANNEL_CLIENT_CONNECT_ERROR 1
#define ERROR_CHANNEL_CLIENT_WRITE_ERROR 2

#define ERROR_CHANNEL_COMM_CONN_CREATE_ERROR 1
#define ERROR_CHANNEL_COMM_CONN_EPOLL_ERROR 2
#define ERROR_CHANNEL_COMM_EPOLL_ERROR 3
#define ERROR_CHANNEL_COMM_TIMER_READ_ERROR 4
#define ERROR_CHANNEL_COMM_CONN_SCHEDULE_MESSAGE_ERROR 5
#define ERROR_CHANNEL_COMM_CONN_INIT_SSL_ERROR 6

// === definition of generated structures ======================================

// -- channel_message_s --
@begin
struct
<
ui:type
ui:sequence
>
options( to_json )
channel_message_s;
@end

// -- channel_client_s --
@begin
struct
<
string_s:server_ip
usi:server_port

channel_conn_s:connection
channel_message_s:message
epoll_timer_s:epoll_timer
>
channel_client_s;
@end

// -- channel_client_list_s --
@begin
list<channel_client_s> channel_client_list_s;
@end

// -- channel_comm_s --
@begin
struct
<
channel_client_list_s:client_list
bc_array_s:buffer
epoll_s:epoll
>
channel_comm_s;
@end

WUR int channel_comm_s_create(channel_comm_s *this,
  unsigned a_count,const char **a_server_ips,const usi *a_server_ports);
WUR int channel_comm_s_run(channel_comm_s *this);
WUR int channel_comm_s_client_time_event(void *a_channel_comm,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR int channel_comm_s_client_fd_event(void *a_channel_comm,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR int channel_comm_s_message(void *a_channel_comm,unsigned a_index,const bc_array_s *a_message);

// === inline methods of generated structures ==================================

// -- channel_message_s --
@begin
inlines channel_message_s
@end

// -- channel_client_s --
@begin
inlines channel_client_s
@end

// -- channel_client_list_s --
@begin
inlines channel_client_list_s
@end

// -- channel_comm_s --
@begin
inlines channel_comm_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

