
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_http.h"
@end

// - error codes -
#define ERROR_HTTP_COMM_SERVER_CREATE_ERROR 1
#define ERROR_HTTP_COMM_SERVER_EPOLL_ERROR 2
#define ERROR_HTTP_COMM_CONN_SERVER_FD_EVENT_ERROR 4
#define ERROR_HTTP_COMM_SERVER_INIT_SSL_ERROR 5
#define ERROR_HTTP_COMM_SCHEDULE_RESPONSE_ERROR 6

// === definition of generated structures ======================================

// -- http_comm_s --
@begin
struct
<
http_server_s:server
bc_array_s:buffer
epoll_s:epoll
>
http_comm_s;
@end

WUR int http_comm_s_create(http_comm_s *this,const char *a_ip,unsigned short a_port);
WUR int http_comm_s_run(http_comm_s *this);
WUR int http_comm_s_conn_request(void *a_http_comm,unsigned a_index,int a_complete);
WUR int http_comm_s_conn_response(void *a_http_comm,unsigned a_index);
WUR int http_comm_s_fd_event(void *a_http_comm,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll);

// === inline methods of generated structures ==================================

// -- http_comm_s --
@begin
inlines http_comm_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

