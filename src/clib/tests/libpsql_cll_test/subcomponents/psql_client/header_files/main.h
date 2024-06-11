
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_linux.h"
include "cl_psql.h"
@end

// - error codes -
#define ERROR_PSQL_CLIENT_CREATE_ERROR 1
#define ERROR_PSQL_CLIENT_EPOLL_ERROR 2

// === definition of generated structures ======================================

// -- psql_client_s --
@begin
struct
<
psql_conn_s:connection
>
psql_client_s;
@end

// === inline methods of generated structures ==================================

// -- psql_client_s --
@begin
inlines psql_client_s
@end

WUR int psql_client_s_create(psql_client_s *this,const char *a_conninfo);
WUR int psql_client_s_run(psql_client_s *this);
WUR int psql_client_s_result(void *a_psql_client,unsigned a_index,psql_result_s *a_result);
WUR int psql_client_s_notify(void *a_psql_client,unsigned a_index,psql_notify_s *a_notify);

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

