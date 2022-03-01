
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_tcp.h"
@end

// - error codes -
#define ERROR_TCP_COMM_SERVER_CREATE_ERROR 1
#define ERROR_TCP_COMM_SERVER_EPOLL_ERROR 2
#define ERROR_TCP_COMM_CONN_SCHEDULE_MESSAGE_ERROR 3
#define ERROR_TCP_COMM_CONN_SERVER_FD_EVENT_ERROR 4
#define ERROR_TCP_COMM_SERVER_INIT_SSL_ERROR 5

// === definition of generated structures ======================================

// -- tcp_comm_s --
@begin
struct
<
tcp_server_s:server
bc_array_s:buffer
>
tcp_comm_s;
@end

WUR int tcp_comm_s_create(tcp_comm_s *this,const char *a_ip,unsigned short a_port);
WUR int tcp_comm_s_run(tcp_comm_s *this);
WUR int tcp_comm_s_conn_new(void *a_tcp_comm,unsigned a_index);
WUR int tcp_comm_s_conn_drop(void *a_tcp_comm,unsigned a_index);
WUR int tcp_comm_s_conn_recv(void *a_tcp_comm,unsigned a_index,bc_array_s *a_message);
WUR int tcp_comm_s_conn_send(void *a_tcp_comm,unsigned a_index);
WUR int tcp_comm_s_fd_event(void *a_tcp_comm,unsigned a_index,epoll_event_s *a_epoll_event);

// === inline methods of generated structures ==================================

// -- tcp_comm_s --
@begin
inlines tcp_comm_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

