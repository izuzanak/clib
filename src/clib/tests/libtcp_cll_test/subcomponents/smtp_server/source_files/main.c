
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- smtp_comm_s --
@begin
methods smtp_comm_s
@end

int smtp_comm_s_create(smtp_comm_s *this,const char *a_ip,unsigned short a_port)
{/*{{{*/
  if (tcp_server_s_create(&this->server,a_ip,a_port,
        smtp_comm_s_conn_new,
        smtp_comm_s_conn_drop,
        smtp_comm_s_conn_recv,
        smtp_comm_s_conn_send,
        this))
  {
    throw_error(SMTP_COMM_SERVER_CREATE_ERROR);
  }

//#ifdef CLIB_WITH_OPENSSL
//  if (tcp_server_s_init_ssl(&this->server,
//    "tests/libchannel_cll_test/resources/mycert.pem",
//    "tests/libchannel_cll_test/resources/mycert.pem"))
//  {
//    throw_error(SMTP_COMM_SERVER_INIT_SSL_ERROR);
//  }
//#endif

  if(epoll_s_fd_callback(&this->server.epoll_fd,EPOLLIN | EPOLLPRI,smtp_comm_s_fd_event,this,0))
  {
    throw_error(SMTP_COMM_SERVER_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int smtp_comm_s_run(smtp_comm_s *this)
{/*{{{*/
  (void)this;

  while(!g_terminate)
  {
    // - wait on events -
    int err;
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(SMTP_COMM_SERVER_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

unsigned smtp_conn_s_recognize_terminal(const bc_array_s *a_source,unsigned *a_input_idx) // NOLINT
{/*{{{*/
#define SMTP_CONN_GET_NEXT_CHAR() \
{\
   if (*a_input_idx < a_source->used) {\
      in_char = (unsigned char)a_source->data[*a_input_idx];\
   }\
   else {\
      in_char = '\0';\
   }\
}

#define SMTP_CONN_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (*a_input_idx >= a_source->used) {\
      return RET_TERM_IDX;\
   }\
\
   (*a_input_idx)++;\
}

   unsigned char in_char;

// - STATE 0 -
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 68) {
      goto state_1_label;
   }
   if (in_char == 69) {
      goto state_2_label;
   }
   if (in_char == 72) {
      goto state_3_label;
   }
   if (in_char == 77) {
      goto state_4_label;
   }
   if (in_char == 78) {
      goto state_5_label;
   }
   if (in_char == 81) {
      goto state_6_label;
   }
   if (in_char == 82) {
      goto state_7_label;
   }
   if (in_char == 86) {
      goto state_8_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 65) {
      goto state_9_label;
   }
   return c_idx_not_exist;

// - STATE 2 -
state_2_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 72) {
      goto state_10_label;
   }
   return c_idx_not_exist;

// - STATE 3 -
state_3_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 69) {
      goto state_11_label;
   }
   return c_idx_not_exist;

// - STATE 4 -
state_4_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 65) {
      goto state_12_label;
   }
   return c_idx_not_exist;

// - STATE 5 -
state_5_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 79) {
      goto state_13_label;
   }
   return c_idx_not_exist;

// - STATE 6 -
state_6_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 85) {
      goto state_14_label;
   }
   return c_idx_not_exist;

// - STATE 7 -
state_7_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 67) {
      goto state_15_label;
   }
   if (in_char == 83) {
      goto state_16_label;
   }
   return c_idx_not_exist;

// - STATE 8 -
state_8_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 82) {
      goto state_17_label;
   }
   return c_idx_not_exist;

// - STATE 9 -
state_9_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_18_label;
   }
   return c_idx_not_exist;

// - STATE 10 -
state_10_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 76) {
      goto state_19_label;
   }
   return c_idx_not_exist;

// - STATE 11 -
state_11_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 76) {
      goto state_20_label;
   }
   return c_idx_not_exist;

// - STATE 12 -
state_12_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 73) {
      goto state_21_label;
   }
   return c_idx_not_exist;

// - STATE 13 -
state_13_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 79) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 14 -
state_14_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 73) {
      goto state_23_label;
   }
   return c_idx_not_exist;

// - STATE 15 -
state_15_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 80) {
      goto state_24_label;
   }
   return c_idx_not_exist;

// - STATE 16 -
state_16_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 69) {
      goto state_25_label;
   }
   return c_idx_not_exist;

// - STATE 17 -
state_17_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 70) {
      goto state_26_label;
   }
   return c_idx_not_exist;

// - STATE 18 -
state_18_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 65) {
      goto state_27_label;
   }
   return c_idx_not_exist;

// - STATE 19 -
state_19_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 79) {
      goto state_28_label;
   }
   return c_idx_not_exist;

// - STATE 20 -
state_20_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 79) {
      goto state_29_label;
   }
   return c_idx_not_exist;

// - STATE 21 -
state_21_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 76) {
      goto state_30_label;
   }
   return c_idx_not_exist;

// - STATE 22 -
state_22_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 80) {
      goto state_31_label;
   }
   return c_idx_not_exist;

// - STATE 23 -
state_23_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 24 -
state_24_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_33_label;
   }
   return c_idx_not_exist;

// - STATE 25 -
state_25_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_34_label;
   }
   return c_idx_not_exist;

// - STATE 26 -
state_26_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 89) {
      goto state_35_label;
   }
   return c_idx_not_exist;

// - STATE 27 -
state_27_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_27_label;
   }
   if (in_char == 10) {
      goto state_36_label;
   }
   if (in_char < 13) {
      goto state_27_label;
   }
   if (in_char == 13) {
      goto state_36_label;
   }
   goto state_27_label;

// - STATE 28 -
state_28_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 32) {
      goto state_37_label;
   }
   return c_idx_not_exist;

// - STATE 29 -
state_29_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 32) {
      goto state_38_label;
   }
   return c_idx_not_exist;

// - STATE 30 -
state_30_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 32) {
      goto state_39_label;
   }
   return c_idx_not_exist;

// - STATE 31 -
state_31_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_31_label;
   }
   if (in_char == 10) {
      goto state_40_label;
   }
   if (in_char < 13) {
      goto state_31_label;
   }
   if (in_char == 13) {
      goto state_40_label;
   }
   goto state_31_label;

// - STATE 32 -
state_32_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_32_label;
   }
   if (in_char == 10) {
      goto state_41_label;
   }
   if (in_char < 13) {
      goto state_32_label;
   }
   if (in_char == 13) {
      goto state_41_label;
   }
   goto state_32_label;

// - STATE 33 -
state_33_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 32) {
      goto state_42_label;
   }
   return c_idx_not_exist;

// - STATE 34 -
state_34_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_34_label;
   }
   if (in_char == 10) {
      goto state_43_label;
   }
   if (in_char < 13) {
      goto state_34_label;
   }
   if (in_char == 13) {
      goto state_43_label;
   }
   goto state_34_label;

// - STATE 35 -
state_35_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 32) {
      goto state_44_label;
   }
   return c_idx_not_exist;

// - STATE 36 -
state_36_label:
   SMTP_CONN_CLOSE_CHAR(4);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_36_label;
   }
   if (in_char == 13) {
      goto state_36_label;
   }
   return 4;

// - STATE 37 -
state_37_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_37_label;
   }
   if (in_char == 10) {
      goto state_45_label;
   }
   if (in_char < 13) {
      goto state_37_label;
   }
   if (in_char == 13) {
      goto state_45_label;
   }
   goto state_37_label;

// - STATE 38 -
state_38_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_38_label;
   }
   if (in_char == 10) {
      goto state_46_label;
   }
   if (in_char < 13) {
      goto state_38_label;
   }
   if (in_char == 13) {
      goto state_46_label;
   }
   goto state_38_label;

// - STATE 39 -
state_39_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 70) {
      goto state_47_label;
   }
   return c_idx_not_exist;

// - STATE 40 -
state_40_label:
   SMTP_CONN_CLOSE_CHAR(8);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_40_label;
   }
   if (in_char == 13) {
      goto state_40_label;
   }
   return 8;

// - STATE 41 -
state_41_label:
   SMTP_CONN_CLOSE_CHAR(5);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_41_label;
   }
   if (in_char == 13) {
      goto state_41_label;
   }
   return 5;

// - STATE 42 -
state_42_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_48_label;
   }
   return c_idx_not_exist;

// - STATE 43 -
state_43_label:
   SMTP_CONN_CLOSE_CHAR(6);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_43_label;
   }
   if (in_char == 13) {
      goto state_43_label;
   }
   return 6;

// - STATE 44 -
state_44_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_44_label;
   }
   if (in_char == 10) {
      goto state_49_label;
   }
   if (in_char < 13) {
      goto state_44_label;
   }
   if (in_char == 13) {
      goto state_49_label;
   }
   goto state_44_label;

// - STATE 45 -
state_45_label:
   SMTP_CONN_CLOSE_CHAR(0);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_45_label;
   }
   if (in_char == 13) {
      goto state_45_label;
   }
   return 0;

// - STATE 46 -
state_46_label:
   SMTP_CONN_CLOSE_CHAR(1);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_46_label;
   }
   if (in_char == 13) {
      goto state_46_label;
   }
   return 1;

// - STATE 47 -
state_47_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 82) {
      goto state_50_label;
   }
   if (in_char == 114) {
      goto state_51_label;
   }
   return c_idx_not_exist;

// - STATE 48 -
state_48_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 79) {
      goto state_52_label;
   }
   if (in_char == 111) {
      goto state_52_label;
   }
   return c_idx_not_exist;

// - STATE 49 -
state_49_label:
   SMTP_CONN_CLOSE_CHAR(7);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_49_label;
   }
   if (in_char == 13) {
      goto state_49_label;
   }
   return 7;

// - STATE 50 -
state_50_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 79) {
      goto state_53_label;
   }
   return c_idx_not_exist;

// - STATE 51 -
state_51_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 111) {
      goto state_54_label;
   }
   return c_idx_not_exist;

// - STATE 52 -
state_52_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 58) {
      goto state_55_label;
   }
   return c_idx_not_exist;

// - STATE 53 -
state_53_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 77) {
      goto state_56_label;
   }
   return c_idx_not_exist;

// - STATE 54 -
state_54_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 109) {
      goto state_56_label;
   }
   return c_idx_not_exist;

// - STATE 55 -
state_55_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_55_label;
   }
   if (in_char == 10) {
      goto state_57_label;
   }
   if (in_char < 13) {
      goto state_55_label;
   }
   if (in_char == 13) {
      goto state_57_label;
   }
   goto state_55_label;

// - STATE 56 -
state_56_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 58) {
      goto state_58_label;
   }
   return c_idx_not_exist;

// - STATE 57 -
state_57_label:
   SMTP_CONN_CLOSE_CHAR(3);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_57_label;
   }
   if (in_char == 13) {
      goto state_57_label;
   }
   return 3;

// - STATE 58 -
state_58_label:
   SMTP_CONN_CLOSE_CHAR(c_idx_not_exist);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_58_label;
   }
   if (in_char == 10) {
      goto state_59_label;
   }
   if (in_char < 13) {
      goto state_58_label;
   }
   if (in_char == 13) {
      goto state_59_label;
   }
   goto state_58_label;

// - STATE 59 -
state_59_label:
   SMTP_CONN_CLOSE_CHAR(2);
   SMTP_CONN_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_59_label;
   }
   if (in_char == 13) {
      goto state_59_label;
   }
   return 2;

}/*}}}*/

int smtp_comm_s_conn_new(void *a_smtp_comm,unsigned a_index)
{/*{{{*/
  (void)a_smtp_comm;

  debug_message_5(fprintf(stderr,"smtp_comm_s_conn_new: %u\n",a_index));

  smtp_comm_s *this = (smtp_comm_s *)a_smtp_comm;
  tcp_conn_s *conn = &this->server.conn_list.data[a_index].object;

  // - set connection state -
  while (this->conn_states.used <= a_index)
  {
    ui_array_s_push(&this->conn_states,SMTP_CONN_STATE_NONE);
  }

  *ui_array_s_at(&this->conn_states,a_index) = SMTP_CONN_STATE_INIT;

  // - send initial greeting message -
  this->buffer.used = 0;
  bc_array_s_append_ptr(&this->buffer,"220 smtp.example.com ESMTP Postfix\r\n");

  if (tcp_conn_s_schedule_message(conn,&this->buffer))
  {
    throw_error(SMTP_COMM_CONN_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int smtp_comm_s_conn_drop(void *a_smtp_comm,unsigned a_index)
{/*{{{*/
  (void)a_smtp_comm;

  debug_message_5(fprintf(stderr,"smtp_comm_s_conn_drop: %u\n",a_index));

  smtp_comm_s *this = (smtp_comm_s *)a_smtp_comm;

  // - reset connection state -
  *ui_array_s_at(&this->conn_states,a_index) = SMTP_CONN_STATE_NONE;

  return 0;
}/*}}}*/

int smtp_comm_s_conn_recv(void *a_smtp_comm,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
  debug_message_5(fprintf(stderr,"smtp_comm_s_conn_recv: %u - %u\n",a_index,a_message->used));

  smtp_comm_s *this = (smtp_comm_s *)a_smtp_comm;
  tcp_conn_s *conn = &this->server.conn_list.data[a_index].object;

  this->buffer.used = 0;

  switch (*ui_array_s_at(&this->conn_states,a_index))
  {
  case SMTP_CONN_STATE_INIT:
    {
      unsigned input_idx = 0;
      unsigned old_input_idx = input_idx;
      unsigned terminal = smtp_conn_s_recognize_terminal(a_message,&input_idx);

      switch (terminal)
      {
      case SMTP_TERM_EHLO:
        {
          bc_array_s_append_ptr(&this->buffer,"500 Error\r\n");
        }
        break;
      case SMTP_TERM_HELO:
        {
          bc_array_s_append_format(&this->buffer,"250 Hello %.*s\r\n",
              input_idx - old_input_idx - 5 - 2,a_message->data + old_input_idx + 5);
        }
        break;
      case SMTP_TERM_MAIL_FROM:
      case SMTP_TERM_RCPT_TO:
      case SMTP_TERM_RSET:
      case SMTP_TERM_NOOP:
        {
          bc_array_s_append_ptr(&this->buffer,"250 Ok\r\n");
        }
        break;
      case SMTP_TERM_DATA:
        {
          bc_array_s_append_ptr(&this->buffer,"354 End data with <CR><LF>.<CR><LF>\r\n");

          // - update connection state -
          *ui_array_s_at(&this->conn_states,a_index) = SMTP_CONN_STATE_DATA;
        }
        break;
      case SMTP_TERM_QUIT:
        {
          bc_array_s_append_ptr(&this->buffer,"221 Bye\r\n");

          // - update connection state -
          *ui_array_s_at(&this->conn_states,a_index) = SMTP_CONN_STATE_QUIT;
        }
        break;
      case SMTP_TERM_VRFY:
        break;
      default:
        throw_error(SMTP_COMM_CONN_INVALID_SNMP_TERMINAL);
        break;
      }

      // - clear message buffer -
      a_message->used = 0;
    }
    break;
  case SMTP_CONN_STATE_DATA:
    {
      if (a_message->used >= 5)
      {
        // - check message tail for end of data -
        char *msg_tail = a_message->data + a_message->used - 5;
        if (memcmp(msg_tail,"\r\n.\r\n",5) == 0)
        {
          bc_array_s_append_ptr(&this->buffer,"250 Ok\r\n");

          // - update connection state -
          *ui_array_s_at(&this->conn_states,a_index) = SMTP_CONN_STATE_INIT;
        }
      }

      // - clear message buffer -
      a_message->used = 0;
    }
    break;
  default:
    cassert(0);
  }

  // - send message if any -
  if (this->buffer.used != 0)
  {
    if (tcp_conn_s_schedule_message(conn,&this->buffer))
    {
      throw_error(SMTP_COMM_CONN_SCHEDULE_MESSAGE_ERROR);
    }
  }

  return 0;
}/*}}}*/

int smtp_comm_s_conn_send(void *a_smtp_comm,unsigned a_index)
{/*{{{*/
  (void)a_smtp_comm;

  debug_message_5(fprintf(stderr,"smtp_comm_s_conn_send: %u\n",a_index));

  smtp_comm_s *this = (smtp_comm_s *)a_smtp_comm;

  if (*ui_array_s_at(&this->conn_states,a_index) == SMTP_CONN_STATE_QUIT)
  {
    throw_error(SMTP_COMM_CONN_QUIT);
  }

  return 0;
}/*}}}*/

int smtp_comm_s_fd_event(void *a_smtp_comm,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  smtp_comm_s *this = (smtp_comm_s *)a_smtp_comm;

  if (tcp_server_s_fd_event(&this->server,a_index,a_epoll_event))
  {
    throw_error(SMTP_COMM_CONN_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  __sync_add_and_fetch(&g_terminate,1);
}/*}}}*/

int epoll_fd_update(int a_fd,unsigned a_evts,int a_update_cb,const epoll_callback_s *a_callback)
{/*{{{*/
  return epoll_s_fd_update(g_epoll,a_fd,a_evts,a_update_cb,a_callback);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();
  libtcp_cll_init();
  libopenssl_cll_init();

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    const char *address = "0.0.0.0";
    const unsigned short port = 2525;

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(smtp_comm_s,comm);
    cassert(smtp_comm_s_create(&comm,address,port) == 0);
    cassert(smtp_comm_s_run(&comm) == 0);
  }

  libopenssl_cll_clear();
  libtcp_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

