
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_gpsd.h"
@end

// - error codes -
#define ERROR_GPSD_CONN_CREATE_ERROR 1
#define ERROR_GPSD_CONN_EPOLL_ERROR 2
#define ERROR_GPSD_CONN_SEND_MESSAGE_ERROR 3

// === definition of generated structures ======================================

// -- gpsd_client_s --
@begin
struct
<
gpsd_conn_s:connection
bc_array_s:buffer
epoll_s:epoll
>
gpsd_client_s;
@end

WUR int gpsd_client_s_create(gpsd_client_s *this,const char *a_server_ip,usi a_server_port);
WUR int gpsd_client_s_run(gpsd_client_s *this);
WUR int gpsd_client_s_gpsd_callback(void *a_gpsd_client,unsigned a_index,unsigned a_type,const va_list a_ap);

// === inline methods of generated structures ==================================

// -- gpsd_client_s --
@begin
inlines gpsd_client_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

