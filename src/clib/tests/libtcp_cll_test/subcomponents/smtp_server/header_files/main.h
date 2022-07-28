
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_tcp.h"
@end

// - error codes -
#define ERROR_SMTP_COMM_SERVER_CREATE_ERROR 1
#define ERROR_SMTP_COMM_SERVER_EPOLL_ERROR 2
#define ERROR_SMTP_COMM_SERVER_INIT_SSL_ERROR 3
#define ERROR_SMTP_COMM_CONN_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_SMTP_COMM_CONN_SERVER_FD_EVENT_ERROR 5
#define ERROR_SMTP_COMM_CONN_INVALID_SNMP_TERMINAL 6
#define ERROR_SMTP_COMM_CONN_QUIT 7

// - smtp connection states -
enum
{
  SMTP_CONN_STATE_NONE = 0,
  SMTP_CONN_STATE_INIT,
  SMTP_CONN_STATE_DATA,
  SMTP_CONN_STATE_QUIT,
};

// - smtp protocol terminals -
enum
{
  SMTP_TERM_EHLO,
  SMTP_TERM_HELO,
  SMTP_TERM_MAIL_FROM,
  SMTP_TERM_RCPT_TO,
  SMTP_TERM_DATA,
  SMTP_TERM_QUIT,
  SMTP_TERM_RSET,
  SMTP_TERM_VRFY,
  SMTP_TERM_NOOP,
};

// === definition of generated structures ======================================

// -- smtp_comm_s --
@begin
struct
<
tcp_server_s:server
ui_array_s:conn_states

bc_array_s:buffer
>
smtp_comm_s;
@end

WUR int smtp_comm_s_create(smtp_comm_s *this,const char *a_ip,unsigned short a_port);
WUR int smtp_comm_s_run(smtp_comm_s *this);
WUR unsigned smtp_conn_s_recognize_terminal(const bc_array_s *a_source,unsigned *a_input_idx);

WUR int smtp_comm_s_conn_new(void *a_smtp_comm,unsigned a_index);
WUR int smtp_comm_s_conn_drop(void *a_smtp_comm,unsigned a_index);
WUR int smtp_comm_s_conn_recv(void *a_smtp_comm,unsigned a_index,bc_array_s *a_message);
WUR int smtp_comm_s_conn_send(void *a_smtp_comm,unsigned a_index);
WUR int smtp_comm_s_fd_event(void *a_smtp_comm,unsigned a_index,epoll_event_s *a_epoll_event);

// === inline methods of generated structures ==================================

// -- smtp_comm_s --
@begin
inlines smtp_comm_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

