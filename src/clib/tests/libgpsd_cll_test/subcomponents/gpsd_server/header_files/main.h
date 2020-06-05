
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_gpsd.h"
@end

// - error codes -
#define ERROR_GPSD_SERVER_CREATE_ERROR 1
#define ERROR_GPSD_SERVER_EPOLL_ERROR 2
#define ERROR_GPSD_SERVER_TIMER_READ_ERROR 3
#define ERROR_GPSD_CONN_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_GPSD_CONN_SERVER_FD_EVENT_ERROR 5

// === definition of generated structures ======================================

// -- message_queue_s --
@begin
queue<bc_array_s> message_queue_s;
@end

// -- message_queues_s --
@begin
array<message_queue_s> message_queues_s;
@end

// -- gpsd_server_s --
@begin
struct
<
tcp_server_s:server

bc_array_s:buffer
message_queues_s:message_queues

epoll_timer_s:message_timer
>
gpsd_server_s;
@end

WUR int gpsd_server_s_create(gpsd_server_s *this,const char *a_ip,unsigned short a_port);
void gpsd_server_s_schedule_message(gpsd_server_s *this,unsigned a_index,bc_array_s *a_buffer);
WUR int gpsd_server_s_run(gpsd_server_s *this);
WUR int gpsd_server_s_message_time_event(void *a_gpsd_server,unsigned a_index,epoll_event_s *a_epoll_event);
WUR int gpsd_server_s_conn_new(void *a_gpsd_server,unsigned a_index);
WUR int gpsd_server_s_conn_drop(void *a_gpsd_server,unsigned a_index);
WUR int gpsd_server_s_conn_recv(void *a_gpsd_server,unsigned a_index,bc_array_s *a_message);
WUR int gpsd_server_s_fd_event(void *a_gpsd_server,unsigned a_index,epoll_event_s *a_epoll_event);

// === inline methods of generated structures ==================================

// -- message_queue_s --
@begin
inlines message_queue_s
@end

// -- message_queues_s --
@begin
inlines message_queues_s
@end

// -- gpsd_server_s --
@begin
inlines gpsd_server_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

