
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_channel.h"
@end

// - error codes -
#define ERROR_CHANNEL_COMM_SERVER_CREATE_ERROR 1
#define ERROR_CHANNEL_COMM_SERVER_EPOLL_ERROR 2
#define ERROR_CHANNEL_COMM_CONN_SCHEDULE_MESSAGE_ERROR 3
#define ERROR_CHANNEL_COMM_CONN_SERVER_FD_EVENT_ERROR 4
#define ERROR_CHANNEL_COMM_SERVER_INIT_SSL_ERROR 5

// === definition of generated structures ======================================

// -- channel_comm_s --
@begin
struct
<
channel_server_s:server
bc_array_s:buffer
epoll_s:epoll
>
channel_comm_s;
@end

WUR int channel_comm_s_create(channel_comm_s *this,const char *a_ip,unsigned short a_port);
WUR int channel_comm_s_run(channel_comm_s *this);
WUR int channel_comm_s_conn_new(void *a_channel_comm,unsigned a_index);
WUR int channel_comm_s_conn_drop(void *a_channel_comm,unsigned a_index);
WUR int channel_comm_s_conn_message(void *a_channel_comm,unsigned a_index,const bc_array_s *a_message);
WUR int channel_comm_s_fd_event(void *a_channel_comm,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// === inline methods of generated structures ==================================

// -- channel_comm_s --
@begin
inlines channel_comm_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

