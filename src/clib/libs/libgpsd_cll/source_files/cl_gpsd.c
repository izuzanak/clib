
@begin
include "cl_gpsd.h"
@end

// === constants and definitions ===============================================

const char g_gpsd_json_parser_init[] =
/*{{{*/
"{"
"\"first\":["

"\"class\","

"\"VERSION\","
"\"DEVICES\","
"\"DEVICE\","
"\"SKY\","
"\"WATCH\","
"\"TPV\","

"\"release\","
"\"rev\","
"\"proto_major\","
"\"proto_minor\","

"\"last\","

"\"\""
"]}";
/*}}}*/

json_parser_s g_gpsd_json_parser;
pointer_tree_s g_gpsd_json_string_map;

// === methods of generated structures =========================================

// -- gpsd_conn_s --
@begin
methods gpsd_conn_s
@end

int gpsd_conn_s_create(gpsd_conn_s *this,epoll_s *a_epoll,
  const char *a_server_ip,unsigned short a_server_port,
  cl_gpsd_cbreq_t a_gpsd_callback,
  void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  gpsd_conn_s_clear(this);

  // - log message -
  log_info_2("gpsd client, create, server_ip: %s, server_port: %hu",
      a_server_ip,a_server_port);

  string_s_set_ptr(&this->server_ip,a_server_ip);
  this->server_port = a_server_port;
  this->message_id = 0;

  this->gpsd_callback = a_gpsd_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;

  // - register epoll timer -
  struct itimerspec itimerspec = {{0,0},{0,1}};
  if (epoll_s_timer_callback(a_epoll,&this->epoll_timer,&itimerspec,0,gpsd_conn_s_connect_time_event,this,0))
  {
    throw_error(GPSD_CLIENT_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

unsigned gpsd_conn_s_retrieve_line(bc_array_s *a_message,unsigned *a_input_idx)
{/*{{{*/
#define GPSD_GET_NEXT_CHAR() \
{\
   if (*a_input_idx < a_message->used) {\
      in_char = (unsigned char)a_message->data[*a_input_idx];\
   }\
   else {\
      in_char = '\0';\
   }\
}

#define GPSD_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (*a_input_idx >= a_message->used) {\
      return RET_TERM_IDX;\
   }\
\
   (*a_input_idx)++;\
}

   unsigned char in_char;
   goto fa_start_label;

// - STATE 0 -
state_0_label:
   GPSD_CLOSE_CHAR(c_idx_not_exist);

fa_start_label:
   GPSD_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_0_label;
   }
   if (in_char == 13) {
      goto state_1_label;
   }
   if (in_char >= 14) {
      goto state_0_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   GPSD_CLOSE_CHAR(c_idx_not_exist);
   GPSD_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 2 -
state_2_label:
   GPSD_CLOSE_CHAR(0);
   return 0;

}/*}}}*/

int gpsd_conn_s_conn_recv(void *a_gpsd_conn,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
  (void)a_index;

  debug_message_6(fprintf(stderr,"gpsd_conn_s_conn_recv\n"));

  // - log message -
  log_info_2("gpsd client, <-- " LOG_MSG_FORMAT,
    LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  gpsd_conn_s *this = (gpsd_conn_s *)a_gpsd_conn;
  var_array_s *string_vars = &g_gpsd_json_parser.string_vars;

  unsigned input_idx = 0;
  do {

    // - line not ready -
    unsigned old_input_idx = input_idx;
    if (gpsd_conn_s_retrieve_line(a_message,&input_idx) != 0)
    {
      bc_array_s_tail(a_message,a_message->used - old_input_idx);
      break;
    }

    unsigned message_length = input_idx - old_input_idx - 2;
    bc_array_s message = {message_length,message_length,a_message->data + old_input_idx};

    CONT_INIT_CLEAR(var_s,msg_var);
    if (json_parser_s_parse(&g_gpsd_json_parser,&message,&msg_var))
    {
      // - reset parser state after syntax error -
      g_gpsd_json_parser.string_idxs.used = 0;
      g_gpsd_json_parser.values.used = 0;
      g_gpsd_json_parser.arrays.used = 0;
      g_gpsd_json_parser.objects.used = 0;

      throw_error(GPSD_MESSAGE_ERROR);
    }

    var_s class_var = loc_s_dict_get(msg_var,string_vars->data[cl_gpsd_class]);
    unsigned class_idx = pointer_tree_s_get_idx(&g_gpsd_json_string_map,class_var);

    switch (class_idx)
    {
    case cl_gpsd_VERSION:
      {/*{{{*/

        // - call callback -
        if (gpsd_conn_s_message_call(this,cl_gpsd_cbreq_VERSION,
              msg_var))
        {
          throw_error(GPSD_CLIENT_CALLBACK_ERROR);
        }
      }/*}}}*/
      break;
    case cl_gpsd_DEVICES:
      {/*{{{*/

        // - call callback -
        if (gpsd_conn_s_message_call(this,cl_gpsd_cbreq_DEVICES,
              msg_var))
        {
          throw_error(GPSD_CLIENT_CALLBACK_ERROR);
        }
      }/*}}}*/
      break;
    case cl_gpsd_DEVICE:
      {/*{{{*/

        // - call callback -
        if (gpsd_conn_s_message_call(this,cl_gpsd_cbreq_DEVICE,
              msg_var))
        {
          throw_error(GPSD_CLIENT_CALLBACK_ERROR);
        }
      }/*}}}*/
      break;
    case cl_gpsd_SKY:
      {/*{{{*/

        // - call callback -
        if (gpsd_conn_s_message_call(this,cl_gpsd_cbreq_SKY,
              msg_var))
        {
          throw_error(GPSD_CLIENT_CALLBACK_ERROR);
        }
      }/*}}}*/
      break;
    case cl_gpsd_WATCH:
      {/*{{{*/

        // - call callback -
        if (gpsd_conn_s_message_call(this,cl_gpsd_cbreq_WATCH,
              msg_var))
        {
          throw_error(GPSD_CLIENT_CALLBACK_ERROR);
        }
      }/*}}}*/
      break;
    case cl_gpsd_TPV:
      {/*{{{*/

        // - call callback -
        if (gpsd_conn_s_message_call(this,cl_gpsd_cbreq_TPV,
              msg_var))
        {
          throw_error(GPSD_CLIENT_CALLBACK_ERROR);
        }
      }/*}}}*/
      break;
    default:
      throw_error(GPSD_MESSAGE_ERROR);
    }

  } while(1);

  return 0;
}/*}}}*/

int gpsd_conn_s_fd_event(void *a_gpsd_conn,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  gpsd_conn_s *this = (gpsd_conn_s *)a_gpsd_conn;

  // - store connecting state -
  int connecting = this->connection.connecting;

  if (tcp_conn_s_fd_event(&this->connection,0,a_epoll_event,a_epoll))
  {
    tcp_conn_s_clear(&this->connection);

    // - register epoll timer -
    struct itimerspec itimerspec = {{0,0},{0,500000000}};
    if (epoll_s_timer_callback(a_epoll,&this->epoll_timer,&itimerspec,0,gpsd_conn_s_connect_time_event,this,0))
    {
      throw_error(GPSD_CLIENT_EPOLL_ERROR);
    }

    if (!connecting)
    {
      // - call callback -
      if (gpsd_conn_s_message_call(this,cl_gpsd_cbreq_DROP))
      {
        throw_error(GPSD_CLIENT_CALLBACK_ERROR);
      }
    }
  }
  else
  {
    if (connecting)
    {
      // - call callback -
      if (gpsd_conn_s_message_call(this,cl_gpsd_cbreq_NEW))
      {
        throw_error(GPSD_CLIENT_CALLBACK_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int gpsd_conn_s_connect_time_event(void *a_gpsd_conn,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(GPSD_CLIENT_TIMER_READ_ERROR);
  }

  gpsd_conn_s *this = (gpsd_conn_s *)a_gpsd_conn;

  if (tcp_conn_s_create_client(&this->connection,
        this->server_ip.data,this->server_port,gpsd_conn_s_conn_recv,NULL,this,0) ||
      epoll_s_fd_callback(a_epoll,&this->connection.epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,gpsd_conn_s_fd_event,this,0))
  {
    // - register epoll timer -
    struct itimerspec itimerspec = {{0,0},{0,500000000}};
    if (epoll_s_timer_callback(a_epoll,&this->epoll_timer,&itimerspec,0,gpsd_conn_s_connect_time_event,this,0))
    {
      throw_error(GPSD_CLIENT_EPOLL_ERROR);
    }
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void libgpsd_cll_init()
{/*{{{*/
  json_parser_s_init(&g_gpsd_json_parser);
  pointer_tree_s_init(&g_gpsd_json_string_map);

  // - initialize json parser strings -
  CONT_INIT_CLEAR(var_s,dummy_var);
  unsigned jpi_length = strlen(g_gpsd_json_parser_init);
  bc_array_s buffer = {jpi_length,jpi_length,(char *)g_gpsd_json_parser_init};
  cassert(json_parser_s_parse(&g_gpsd_json_parser,&buffer,&dummy_var) == 0);

  // - initialize json string map -
  var_array_s *string_vars = &g_gpsd_json_parser.string_vars;

  unsigned s_idx = cl_gpsd_first;
  unsigned s_idx_end = cl_gpsd_last;
  do {
    pointer_tree_s_insert(&g_gpsd_json_string_map,string_vars->data[s_idx]);
  } while(++s_idx <= s_idx_end);
}/*}}}*/

void libgpsd_cll_clear()
{/*{{{*/
  json_parser_s_clear(&g_gpsd_json_parser);
  pointer_tree_s_clear(&g_gpsd_json_string_map);
}/*}}}*/

