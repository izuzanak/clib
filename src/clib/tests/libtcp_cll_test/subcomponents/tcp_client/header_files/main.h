
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_tcp.h"
@end

// - error codes -
#define ERROR_TCP_CLIENT_CONNECT_ERROR 1
#define ERROR_TCP_CLIENT_WRITE_ERROR 2

#define ERROR_TCP_COMM_CONN_CREATE_ERROR 1
#define ERROR_TCP_COMM_CONN_EPOLL_ERROR 2
#define ERROR_TCP_COMM_EPOLL_ERROR 3
#define ERROR_TCP_COMM_TIMER_READ_ERROR 4
#define ERROR_TCP_COMM_CONN_SCHEDULE_MESSAGE_ERROR 5
#define ERROR_TCP_COMM_CONN_INIT_SSL_ERROR 6

// === definition of generated structures ======================================

// -- tcp_message_s --
@begin
struct
<
ui:type
ui:sequence
>
options( to_json )
tcp_message_s;
@end

// -- tcp_client_s --
@begin
struct
<
string_s:server_ip
usi:server_port

tcp_conn_s:connection
tcp_message_s:message
epoll_timer_s:epoll_timer
>
tcp_client_s;
@end

// -- tcp_client_list_s --
@begin
list<tcp_client_s> tcp_client_list_s;
@end

// -- tcp_comm_s --
@begin
struct
<
tcp_client_list_s:client_list
bc_array_s:buffer
epoll_s:epoll
>
tcp_comm_s;
@end

WUR int tcp_comm_s_create(tcp_comm_s *this,
  unsigned a_count,const char **a_server_ips,const usi *a_server_ports);
WUR int tcp_comm_s_run(tcp_comm_s *this);
WUR int tcp_comm_s_client_time_event(void *a_tcp_comm,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR int tcp_comm_s_client_fd_event(void *a_tcp_comm,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);
WUR int tcp_comm_s_recv(void *a_tcp_comm,unsigned a_index,bc_array_s *a_message);
WUR int tcp_comm_s_send(void *a_tcp_comm,unsigned a_index);

// === inline methods of generated structures ==================================

// -- tcp_message_s --
@begin
inlines tcp_message_s
@end

// -- tcp_client_s --
@begin
inlines tcp_client_s
@end

// -- tcp_client_list_s --
@begin
inlines tcp_client_list_s
@end

// -- tcp_comm_s --
@begin
inlines tcp_comm_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

