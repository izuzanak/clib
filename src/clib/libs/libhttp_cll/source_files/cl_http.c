
@begin
include "cl_http.h"
@end

// === methods of generated structures =========================================

// -- bc_buffer_s --
@begin
methods bc_buffer_s
@end

// -- bc_buffers_s --
@begin
methods bc_buffers_s
@end

// -- http_conn_s --
@begin
methods http_conn_s
@end

unsigned http_conn_s_recognize_request_terminal(http_conn_s *this,const bc_array_s *a_source)
{/*{{{*/
#define HTTP_REQUEST_GET_NEXT_CHAR() \
{\
   if (this->input_idx < a_source->used) {\
      in_char = (unsigned char)a_source->data[this->input_idx];\
   }\
   else {\
      in_char = '\0';\
   }\
}

#define HTTP_REQUEST_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (this->input_idx >= a_source->used) {\
      return RET_TERM_IDX;\
   }\
\
   this->input_idx++;\
}

   unsigned char in_char;

// - STATE 0 -
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 8 && in_char < 14) {
      goto state_5_label;
   }
   if (in_char == 32) {
      goto state_5_label;
   }
   if (in_char == 47) {
      goto state_1_label;
   }
   if (in_char == 71) {
      goto state_2_label;
   }
   if (in_char == 72) {
      goto state_3_label;
   }
   if (in_char == 80) {
      goto state_4_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 47) {
      goto state_6_label;
   }
   if (in_char >= 48) {
      goto state_6_label;
   }
   return c_idx_not_exist;

// - STATE 2 -
state_2_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 69) {
      goto state_7_label;
   }
   return c_idx_not_exist;

// - STATE 3 -
state_3_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 69) {
      goto state_8_label;
   }
   if (in_char == 84) {
      goto state_9_label;
   }
   return c_idx_not_exist;

// - STATE 4 -
state_4_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 79) {
      goto state_10_label;
   }
   if (in_char == 85) {
      goto state_11_label;
   }
   return c_idx_not_exist;

// - STATE 5 -
state_5_label:
   HTTP_REQUEST_CLOSE_CHAR(6);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 8 && in_char < 14) {
      goto state_5_label;
   }
   if (in_char == 32) {
      goto state_5_label;
   }
   return 6;

// - STATE 6 -
state_6_label:
   HTTP_REQUEST_CLOSE_CHAR(4);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 32) {
      goto state_6_label;
   }
   if (in_char >= 33 && in_char < 47) {
      goto state_6_label;
   }
   if (in_char >= 48) {
      goto state_6_label;
   }
   return 4;

// - STATE 7 -
state_7_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_12_label;
   }
   return c_idx_not_exist;

// - STATE 8 -
state_8_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 65) {
      goto state_13_label;
   }
   return c_idx_not_exist;

// - STATE 9 -
state_9_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_14_label;
   }
   return c_idx_not_exist;

// - STATE 10 -
state_10_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 83) {
      goto state_15_label;
   }
   return c_idx_not_exist;

// - STATE 11 -
state_11_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_16_label;
   }
   return c_idx_not_exist;

// - STATE 12 -
state_12_label:
   HTTP_REQUEST_CLOSE_CHAR(0);
   return 0;

// - STATE 13 -
state_13_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 68) {
      goto state_17_label;
   }
   return c_idx_not_exist;

// - STATE 14 -
state_14_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 80) {
      goto state_18_label;
   }
   return c_idx_not_exist;

// - STATE 15 -
state_15_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_19_label;
   }
   return c_idx_not_exist;

// - STATE 16 -
state_16_label:
   HTTP_REQUEST_CLOSE_CHAR(3);
   return 3;

// - STATE 17 -
state_17_label:
   HTTP_REQUEST_CLOSE_CHAR(1);
   return 1;

// - STATE 18 -
state_18_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 47) {
      goto state_20_label;
   }
   return c_idx_not_exist;

// - STATE 19 -
state_19_label:
   HTTP_REQUEST_CLOSE_CHAR(2);
   return 2;

// - STATE 20 -
state_20_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 49) {
      goto state_21_label;
   }
   return c_idx_not_exist;

// - STATE 21 -
state_21_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 46) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 22 -
state_22_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 50) {
      goto state_23_label;
   }
   return c_idx_not_exist;

// - STATE 23 -
state_23_label:
   HTTP_REQUEST_CLOSE_CHAR(5);
   return 5;

}/*}}}*/

unsigned http_conn_s_recognize_header_terminal(http_conn_s *this,const bc_array_s *a_source)
{/*{{{*/
#define HTTP_HEADER_GET_NEXT_CHAR() \
{\
   if (this->input_idx < a_source->used) {\
      in_char = (unsigned char)a_source->data[this->input_idx];\
   }\
   else {\
      in_char = '\0';\
   }\
}

#define HTTP_HEADER_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (this->input_idx >= a_source->used) {\
      return RET_TERM_IDX;\
   }\
\
   this->input_idx++;\
}

   unsigned char in_char;

// - STATE 0 -
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char == 13) {
      goto state_2_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char == 58) {
      goto state_3_label;
   }
   if (in_char >= 59 && in_char < 67) {
      goto state_1_label;
   }
   if (in_char == 67) {
      goto state_4_label;
   }
   if (in_char >= 68 && in_char < 99) {
      goto state_1_label;
   }
   if (in_char == 99) {
      goto state_4_label;
   }
   if (in_char >= 100) {
      goto state_1_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59) {
      goto state_1_label;
   }
   return 2;

// - STATE 2 -
state_2_label:
   HTTP_HEADER_CLOSE_CHAR(c_idx_not_exist);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_5_label;
   }
   return c_idx_not_exist;

// - STATE 3 -
state_3_label:
   HTTP_HEADER_CLOSE_CHAR(0);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char == 9) {
      goto state_3_label;
   }
   if (in_char == 32) {
      goto state_3_label;
   }
   return 0;

// - STATE 4 -
state_4_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 79) {
      goto state_1_label;
   }
   if (in_char == 79) {
      goto state_6_label;
   }
   if (in_char >= 80 && in_char < 111) {
      goto state_1_label;
   }
   if (in_char == 111) {
      goto state_6_label;
   }
   if (in_char >= 112) {
      goto state_1_label;
   }
   return 2;

// - STATE 5 -
state_5_label:
   HTTP_HEADER_CLOSE_CHAR(1);
   return 1;

// - STATE 6 -
state_6_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 78) {
      goto state_1_label;
   }
   if (in_char == 78) {
      goto state_7_label;
   }
   if (in_char >= 79 && in_char < 110) {
      goto state_1_label;
   }
   if (in_char == 110) {
      goto state_7_label;
   }
   if (in_char >= 111) {
      goto state_1_label;
   }
   return 2;

// - STATE 7 -
state_7_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 84) {
      goto state_1_label;
   }
   if (in_char == 84) {
      goto state_8_label;
   }
   if (in_char >= 85 && in_char < 116) {
      goto state_1_label;
   }
   if (in_char == 116) {
      goto state_8_label;
   }
   if (in_char >= 117) {
      goto state_1_label;
   }
   return 2;

// - STATE 8 -
state_8_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 69) {
      goto state_1_label;
   }
   if (in_char == 69) {
      goto state_9_label;
   }
   if (in_char >= 70 && in_char < 101) {
      goto state_1_label;
   }
   if (in_char == 101) {
      goto state_9_label;
   }
   if (in_char >= 102) {
      goto state_1_label;
   }
   return 2;

// - STATE 9 -
state_9_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 78) {
      goto state_1_label;
   }
   if (in_char == 78) {
      goto state_10_label;
   }
   if (in_char >= 79 && in_char < 110) {
      goto state_1_label;
   }
   if (in_char == 110) {
      goto state_10_label;
   }
   if (in_char >= 111) {
      goto state_1_label;
   }
   return 2;

// - STATE 10 -
state_10_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 84) {
      goto state_1_label;
   }
   if (in_char == 84) {
      goto state_11_label;
   }
   if (in_char >= 85 && in_char < 116) {
      goto state_1_label;
   }
   if (in_char == 116) {
      goto state_11_label;
   }
   if (in_char >= 117) {
      goto state_1_label;
   }
   return 2;

// - STATE 11 -
state_11_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 45) {
      goto state_1_label;
   }
   if (in_char == 45) {
      goto state_12_label;
   }
   if (in_char >= 46 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59) {
      goto state_1_label;
   }
   return 2;

// - STATE 12 -
state_12_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 76) {
      goto state_1_label;
   }
   if (in_char == 76) {
      goto state_13_label;
   }
   if (in_char >= 77 && in_char < 84) {
      goto state_1_label;
   }
   if (in_char == 84) {
      goto state_14_label;
   }
   if (in_char >= 85 && in_char < 108) {
      goto state_1_label;
   }
   if (in_char == 108) {
      goto state_13_label;
   }
   if (in_char >= 109 && in_char < 116) {
      goto state_1_label;
   }
   if (in_char == 116) {
      goto state_14_label;
   }
   if (in_char >= 117) {
      goto state_1_label;
   }
   return 2;

// - STATE 13 -
state_13_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 69) {
      goto state_1_label;
   }
   if (in_char == 69) {
      goto state_15_label;
   }
   if (in_char >= 70 && in_char < 101) {
      goto state_1_label;
   }
   if (in_char == 101) {
      goto state_15_label;
   }
   if (in_char >= 102) {
      goto state_1_label;
   }
   return 2;

// - STATE 14 -
state_14_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 89) {
      goto state_1_label;
   }
   if (in_char == 89) {
      goto state_16_label;
   }
   if (in_char >= 90 && in_char < 121) {
      goto state_1_label;
   }
   if (in_char == 121) {
      goto state_16_label;
   }
   if (in_char >= 122) {
      goto state_1_label;
   }
   return 2;

// - STATE 15 -
state_15_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 78) {
      goto state_1_label;
   }
   if (in_char == 78) {
      goto state_17_label;
   }
   if (in_char >= 79 && in_char < 110) {
      goto state_1_label;
   }
   if (in_char == 110) {
      goto state_17_label;
   }
   if (in_char >= 111) {
      goto state_1_label;
   }
   return 2;

// - STATE 16 -
state_16_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 80) {
      goto state_1_label;
   }
   if (in_char == 80) {
      goto state_18_label;
   }
   if (in_char >= 81 && in_char < 112) {
      goto state_1_label;
   }
   if (in_char == 112) {
      goto state_18_label;
   }
   if (in_char >= 113) {
      goto state_1_label;
   }
   return 2;

// - STATE 17 -
state_17_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 71) {
      goto state_1_label;
   }
   if (in_char == 71) {
      goto state_19_label;
   }
   if (in_char >= 72 && in_char < 103) {
      goto state_1_label;
   }
   if (in_char == 103) {
      goto state_19_label;
   }
   if (in_char >= 104) {
      goto state_1_label;
   }
   return 2;

// - STATE 18 -
state_18_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 69) {
      goto state_1_label;
   }
   if (in_char == 69) {
      goto state_20_label;
   }
   if (in_char >= 70 && in_char < 101) {
      goto state_1_label;
   }
   if (in_char == 101) {
      goto state_20_label;
   }
   if (in_char >= 102) {
      goto state_1_label;
   }
   return 2;

// - STATE 19 -
state_19_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 84) {
      goto state_1_label;
   }
   if (in_char == 84) {
      goto state_21_label;
   }
   if (in_char >= 85 && in_char < 116) {
      goto state_1_label;
   }
   if (in_char == 116) {
      goto state_21_label;
   }
   if (in_char >= 117) {
      goto state_1_label;
   }
   return 2;

// - STATE 20 -
state_20_label:
   HTTP_HEADER_CLOSE_CHAR(3);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59) {
      goto state_1_label;
   }
   return 3;

// - STATE 21 -
state_21_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 72) {
      goto state_1_label;
   }
   if (in_char == 72) {
      goto state_22_label;
   }
   if (in_char >= 73 && in_char < 104) {
      goto state_1_label;
   }
   if (in_char == 104) {
      goto state_22_label;
   }
   if (in_char >= 105) {
      goto state_1_label;
   }
   return 2;

// - STATE 22 -
state_22_label:
   HTTP_HEADER_CLOSE_CHAR(4);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59) {
      goto state_1_label;
   }
   return 4;

}/*}}}*/

// -- http_conns_s --
@begin
methods http_conns_s
@end

// -- http_server_s --
@begin
methods http_server_s
@end

int http_server_s_create(http_server_s *this,
    const char *a_ip,unsigned short a_port,
    http_conn_request_callback_t a_conn_request_callback,
    http_conn_response_callback_t a_conn_response_callback,
    void *a_cb_object)
{/*{{{*/
  debug_message_3(fprintf(stderr,"http_server_s_create\n"));

  http_server_s_clear(this);

  this->conn_request_callback = a_conn_request_callback;
  this->conn_response_callback = a_conn_response_callback;
  this->cb_object = a_cb_object;

  // - create tcp server -
  if (tcp_server_s_create(&this->server,a_ip,a_port,
        http_server_s_tcp_conn_new,
        http_server_s_tcp_conn_drop,
        http_server_s_tcp_conn_recv,
        http_server_s_tcp_conn_send,
        this))
  {
    throw_error(HTTP_SERVER_TCP_SERVER_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

int http_server_s_tcp_conn_new(void *a_http_server,unsigned a_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"http_server_s_tcp_conn_new: %u\n",a_index));

  http_server_s *this = (http_server_s *)a_http_server;

  while (a_index >= this->conns.used)
  {
    http_conns_s_push_blank(&this->conns);
  }

  http_conn_s *conn = http_conns_s_at(&this->conns,a_index);

  // - initialize connection -
  conn->state = c_http_conn_state_REQUEST;
  conn->input_idx = 0;
  conn->headers_size = 0;
  conn->message_size = 0;
  conn->receive_size = 0;

  return 0;
}/*}}}*/

int http_server_s_tcp_conn_drop(void *a_http_server,unsigned a_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"http_server_s_tcp_conn_drop: %u\n",a_index));

  http_server_s *this = (http_server_s *)a_http_server;
  http_conn_s *conn = http_conns_s_at(&this->conns,a_index);

  http_conn_s_clear(conn);

  return 0;
}/*}}}*/

int http_server_s_tcp_conn_recv(void *a_http_server,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
  http_server_s *this = (http_server_s *)a_http_server;
  http_conn_s *conn = http_conns_s_at(&this->conns,a_index);

  switch (conn->state)
  {
  case c_http_conn_state_REQUEST:
    {/*{{{*/

      // - retrieve method -
      conn->method = http_conn_s_recognize_request_terminal(conn,a_message);

      switch (conn->method)
      {
      case c_http_req_term_GET:
      case c_http_req_term_HEAD:
      case c_http_req_term_POST:
      case c_http_req_term_PUT:
        break;
      default:
        throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
      }

      // - skip whitespace -
      if (http_conn_s_recognize_request_terminal(conn,a_message) != c_http_req_term_WHITESPACE)
      {
        throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
      }

      // - retrieve uri components -
      unsigned uri_input_idx = conn->input_idx;
      unsigned old_input_idx = conn->input_idx;
      unsigned term = http_conn_s_recognize_request_terminal(conn,a_message);

      do {

        // - next term is not uri component -
        if (term != c_http_req_term_URI_COMPONENT)
        {
          throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
        }

        // - set uri component buffer -
        bc_buffers_s_push_blank(&conn->uri_components);
        bc_buffer_s_set(bc_buffers_s_last(&conn->uri_components),
          old_input_idx,conn->input_idx - old_input_idx);

        old_input_idx = conn->input_idx;
        term = http_conn_s_recognize_request_terminal(conn,a_message);
      } while(term != c_http_req_term_WHITESPACE);

      // - set uri buffer -
      bc_buffer_s_set(&conn->uri,uri_input_idx,old_input_idx - uri_input_idx);

      // - retrieve http version -
      old_input_idx = conn->input_idx;
      term = http_conn_s_recognize_request_terminal(conn,a_message);
      if (term != c_http_req_term_HTTP)
      {
        throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
      }

      // - set http buffer -
      bc_buffer_s_set(&conn->http,old_input_idx,conn->input_idx - old_input_idx);

      // - skip end of line -
      conn->input_idx += 2;

      // - update connection state -
      conn->state = c_http_conn_state_HEADERS;
    }/*}}}*/

  case c_http_conn_state_HEADERS:
    {/*{{{*/
      do {
        unsigned header_idx = conn->input_idx;
        unsigned term = http_conn_s_recognize_header_terminal(conn,a_message);

        if (term == c_idx_not_exist)
        {
          // - header not recognized, wait on data -
          conn->input_idx = header_idx;
          break;
        }

        if (term == c_http_head_term_END)
        {
          // - update message length by header length -
          conn->headers_size = conn->input_idx;
          conn->message_size += conn->input_idx;

          // - update connection state -
          conn->state = c_http_conn_state_BODY;
          break;
        }

        // - invalid terminal recognized -
        if (term < c_http_head_term_HEADER)
        {
          throw_error(HTTP_SERVER_HTTP_HEADERS_ERROR);
        }

        unsigned header_length = conn->input_idx - header_idx;

        // - skip separator -
        if (http_conn_s_recognize_header_terminal(conn,a_message) != c_http_head_term_SEPARATOR)
        {
          throw_error(HTTP_SERVER_HTTP_HEADERS_ERROR);
        }

        unsigned value_idx = conn->input_idx;

        // - find end of line -
        char *ptr = a_message->data + value_idx;
        char *ptr_end = a_message->data + a_message->used;
        do {
          if (*ptr == '\r')
          {
            break;
          }
        } while(++ptr < ptr_end);

        // - end of line was found -
        if (ptr < ptr_end)
        {
          unsigned value_length = (ptr - a_message->data) - value_idx;

          bc_buffers_s_push_blank(&conn->headers);
          bc_buffer_s_set(bc_buffers_s_last(&conn->headers),header_idx,header_length);

          bc_buffers_s_push_blank(&conn->values);
          bc_buffer_s_set(bc_buffers_s_last(&conn->values),value_idx,value_length);

          switch (term)
          {
          case c_http_head_term_CONTENT_LENGTH:
            {
              char *endptr;
              conn->message_size = strtoll(a_message->data + value_idx,&endptr,10);

              // - invalid value -
              if (endptr != a_message->data + value_idx + value_length)
              {
                throw_error(HTTP_SERVER_HTTP_HEADERS_ERROR);
              }
            }
            break;
          }

          // - update input index -
          conn->input_idx = (ptr - a_message->data) + 2;
        }
        else
        {
          // - value not complete, wait on data -
          conn->input_idx = header_idx;
          break;
        }

      } while(1);
    }/*}}}*/

    if (conn->state != c_http_conn_state_BODY)
    {
      break;
    }

  case c_http_conn_state_BODY:
    {/*{{{*/

      //// FIXME debug print headers
      //if (conn->headers.used != 0)
      //{
      //  bc_buffer_s *h_ptr = conn->headers.data;
      //  bc_buffer_s *h_ptr_end = h_ptr + conn->headers.used;
      //  bc_buffer_s *v_ptr = conn->values.data;
      //  do {
      //    fprintf(stderr,"%.*s --> %.*s\n",h_ptr->length,a_message->data + h_ptr->begin,v_ptr->length,a_message->data + v_ptr->begin);
      //  } while(++v_ptr,++h_ptr < h_ptr_end);
      //}

      // - receiving small message or all data are already available -
      if (conn->message_size < HTTP_MAX_CHUNK_SIZE || a_message->used >= conn->message_size)
      {
        if (a_message->used >= conn->message_size)
        {
          // - call conn_request_callback -
          if (((http_conn_request_callback_t)this->conn_request_callback)(this->cb_object,a_index,1))
          {
            throw_error(HTTP_CONN_CALLBACK_ERROR);
          }
        }
      }

      // - receiving large message -
      else
      {
        // - copy headers data -
        if (conn->headers_data.used == 0)
        {
          bc_array_s_set(&conn->headers_data,conn->headers_size,a_message->data);
          bc_array_s_tail(a_message,a_message->used - conn->headers_size);

          // - update receive size -
          conn->receive_size += conn->headers_size;
        }

        // - message is complete or chunk is big enought -
        int complete = conn->receive_size + a_message->used >= conn->message_size;
        if (complete || a_message->used >= HTTP_MAX_CHUNK_SIZE)
        {
          // - call conn_request_callback -
          if (((http_conn_request_callback_t)this->conn_request_callback)(this->cb_object,a_index,complete))
          {
            throw_error(HTTP_CONN_CALLBACK_ERROR);
          }
          
          // - update receive size -
          conn->receive_size += a_message->used;

          a_message->used = 0;
        }
      }
    }/*}}}*/
    break;
  }

  return 0;
}/*}}}*/

int http_server_s_tcp_conn_send(void *a_http_server,unsigned a_index)
{/*{{{*/
  http_server_s *this = (http_server_s *)a_http_server;

  // - conn response callback is set -
  if (this->conn_response_callback)
  {
    tcp_conn_s *tcp_conn = tcp_conn_list_s_at(&this->server.conn_list,a_index);

    // - empty output queue -
    if (tcp_conn->out_msg_queue.used == 0)
    {
      // - call conn_response_callback -
      if (((http_conn_response_callback_t)this->conn_response_callback)(this->cb_object,a_index))
      {
        throw_error(HTTP_CONN_CALLBACK_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

