
@begin
include "od_channel.h"
@end

// === constants and definitions ===============================================

const char g_od_json_parser_init[] =
/*{{{*/
"{"
"\"first\":["

"\"id\","

"\"type\","
"\"resp\","
"\"path\","
"\"data\","

"\"set\","
"\"cmd\","
"\"get\","
"\"watch\","
"\"ignore\","
"\"update\","
"\"ping\","

"\"last\","

"\"\""
"]}";
/*}}}*/

json_parser_s g_od_channel_json_parser;
pointer_tree_s g_od_channel_json_string_map;

// === methods of generated structures =========================================

// -- od_channel_s --
@begin
methods od_channel_s
@end

int od_channel_s_create(od_channel_s *this,const char *a_ip,unsigned short a_port,epoll_s *a_epoll,
    od_channel_cbreq_t a_channel_callback,
    void *a_cb_object)
{/*{{{*/
  od_channel_s_clear(this);

  // - log message -
  log_info_2("channel server, create, ip: %s, port: %hu",a_ip,a_port);

  // - create channel server -
  if(channel_server_s_create(&this->server,a_ip,a_port,
        od_channel_s_conn_new,
        od_channel_s_conn_drop,
        od_channel_s_conn_message,
        this))
  {
    throw_error(OD_CHANNEL_SERVER_CREATE_ERROR);
  }

  if(epoll_s_fd_callback(a_epoll,&this->server.epoll_fd,EPOLLIN | EPOLLPRI,od_channel_s_fd_event,this,0))
  {
    throw_error(OD_CHANNEL_SERVER_EPOLL_ERROR);
  }

  this->channel_callback = a_channel_callback;
  this->cb_object = a_cb_object;

  return 0;
}/*}}}*/

int od_channel_s_conn_new(void *a_od_channel,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"od_channel_s_conn_new: %u\n",a_index));

  // - log message -
  log_info_2("channel server %u, new connection",a_index);

  od_channel_s *this = (od_channel_s *)a_od_channel;

  // - call callback -
  if (od_channel_s_message_call(this,a_index,od_channel_cbreq_NEW))
  {
    throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int od_channel_s_conn_drop(void *a_od_channel,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"od_channel_s_conn_drop: %u\n",a_index));

  // - log message -
  log_info_2("channel server %u, drop connection",a_index);

  od_channel_s *this = (od_channel_s *)a_od_channel;

  // - call callback -
  if (od_channel_s_message_call(this,a_index,od_channel_cbreq_DROP))
  {
    throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int od_channel_s_send_multi_message_var(od_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var)
{/*{{{*/
  const bc_array_s *message = loc_s_channel_message_value(a_data_var);

#if OPTION_BRUTAL_ASSERT == ENABLED
  CONT_INIT_CLEAR(var_s,msg_var);
  brutal_assert(json_parser_s_parse(&g_od_channel_json_parser,message,&msg_var) == 0);
#endif

  if (a_indexes->used != 0)
  {
    VAR_CLEAR(length_var,loc_s_channel_message_buffer_length(message));

    const unsigned *i_ptr = a_indexes->data;
    const unsigned *i_ptr_end = i_ptr + a_indexes->used;
    do
    {
      // - log message -
      log_info_2("channel server %u, --> "LOG_MSG_FORMAT,*i_ptr,
          LOG_MSG_PARAMETERS(message->used,message->data));

      // - schedule message to send -
      if (channel_conn_s_schedule_message_var(&this->server.conn_list.data[*i_ptr].object,length_var,a_data_var))
      {
        throw_error(OD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
      }
    } while(++i_ptr < i_ptr_end);
  }

  return 0;
}/*}}}*/

int od_channel_s_conn_message(void *a_od_channel,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  debug_message_5(fprintf(stderr,"conn_message: %u - %u bytes\n",a_index,a_message->used));

  // - log message -
  log_info_2("channel server %u, <-- " LOG_MSG_FORMAT,a_index,
      LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  od_channel_s *this = (od_channel_s *)a_od_channel;
  var_array_s *string_vars = &g_od_channel_json_parser.string_vars;

  CONT_INIT_CLEAR(var_s,msg_var);
  if (json_parser_s_parse(&g_od_channel_json_parser,a_message,&msg_var))
  {
    // - reset parser state after syntax error -
    g_od_channel_json_parser.string_idxs.used = 0;
    g_od_channel_json_parser.values.used = 0;
    g_od_channel_json_parser.arrays.used = 0;
    g_od_channel_json_parser.objects.used = 0;

    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  var_s id_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_ID]);
  if (id_var == NULL || id_var->v_type != c_bi_type_integer)
  {
    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  lli id = loc_s_int_value(id_var);
  var_s type_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_TYPE]);
  unsigned type_idx = pointer_tree_s_get_idx(&g_od_channel_json_string_map,type_var);

  switch (type_idx)
  {
  case od_channel_SET:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);
      var_s data_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_DATA]);

      if (path_var == NULL || path_var->v_type != c_bi_type_string ||
          data_var == NULL)
      {
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (od_channel_s_message_call(this,a_index,od_channel_cbreq_SET,id,
            loc_s_string_value(path_var),data_var))
      {
        throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_CMD:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);
      var_s data_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_DATA]);

      if (path_var == NULL || path_var->v_type != c_bi_type_string ||
          data_var == NULL)
      {
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (od_channel_s_message_call(this,a_index,od_channel_cbreq_CMD,id,
            loc_s_string_value(path_var),data_var))
      {
        throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_GET:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);

      if (path_var == NULL || path_var->v_type != c_bi_type_string)
      {
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (od_channel_s_message_call(this,a_index,od_channel_cbreq_GET,id,
            loc_s_string_value(path_var)))
      {
        throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_WATCH:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);

      if (path_var == NULL || path_var->v_type != c_bi_type_string)
      {
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (od_channel_s_message_call(this,a_index,od_channel_cbreq_WATCH,id,
            loc_s_string_value(path_var)))
      {
        throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_IGNORE:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);

      if (path_var == NULL || path_var->v_type != c_bi_type_string)
      {
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (od_channel_s_message_call(this,a_index,od_channel_cbreq_IGNORE,id,
            loc_s_string_value(path_var)))
      {
        throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_PING:
    {/*{{{*/
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"ping\",\"id\":%" HOST_LL_FORMAT "d}",id);

      // - send message -
      if (od_channel_s_send_message(this,a_index,&this->buffer))
      {
        throw_error(OD_CHANNEL_SERVER_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  default:
    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int od_channel_s_fd_event(void *a_od_channel,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  od_channel_s *this = (od_channel_s *)a_od_channel;

  if(channel_server_s_fd_event(&this->server,a_index,a_epoll_event,a_epoll))
  {
    throw_error(OD_CHANNEL_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

// -- od_channel_client_s --
@begin
methods od_channel_client_s
@end

int od_channel_client_s_create(od_channel_client_s *this,epoll_s *a_epoll,
  const char *a_server_ip,unsigned short a_server_port,
  od_channel_cbreq_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  od_channel_client_s_clear(this);

  // - log message -
  log_info_2("channel client, create, server_ip: %s, server_port: %hu",
      a_server_ip,a_server_port);

  string_s_set_ptr(&this->server_ip,a_server_ip);
  this->server_port = a_server_port;
  this->connected = 0;
  this->message_id = 0;

  this->channel_callback = a_channel_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;

  // - register connect timer -
  struct itimerspec its_connect = {{0,0},{0,1}};
  if (epoll_s_timer_callback(a_epoll,&this->connect_timer,&its_connect,0,od_channel_client_s_connect_time_event,this,0))
  {
    throw_error(OD_CHANNEL_CLIENT_EPOLL_ERROR);
  }

  // - register ping timer -
  struct itimerspec its_ping = {OD_CHANNEL_PING_PERIOD,OD_CHANNEL_PING_PERIOD};
  if (epoll_s_timer_callback(a_epoll,&this->ping_timer,&its_ping,0,od_channel_client_s_ping_time_event,this,0))
  {
    throw_error(OD_CHANNEL_CLIENT_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int od_channel_client_s_conn_message(void *a_od_channel_client,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  (void)a_od_channel_client;
  (void)a_index;

  debug_message_6(fprintf(stderr,"od_channel_client_s_conn_message\n"));

  // - log message -
  log_info_2("channel client, <-- " LOG_MSG_FORMAT,
    LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  od_channel_client_s *this = (od_channel_client_s *)a_od_channel_client;
  var_array_s *string_vars = &g_od_channel_json_parser.string_vars;

  CONT_INIT_CLEAR(var_s,msg_var);
  if (json_parser_s_parse(&g_od_channel_json_parser,a_message,&msg_var))
  {
    // - reset parser state after syntax error -
    g_od_channel_json_parser.string_idxs.used = 0;
    g_od_channel_json_parser.values.used = 0;
    g_od_channel_json_parser.arrays.used = 0;
    g_od_channel_json_parser.objects.used = 0;

    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  var_s id_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_ID]);
  if (id_var == NULL || id_var->v_type != c_bi_type_integer)
  {
    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  lli id = loc_s_int_value(id_var);
  var_s type_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_TYPE]);
  unsigned type_idx = pointer_tree_s_get_idx(&g_od_channel_json_string_map,type_var);

  switch (type_idx)
  {
  case od_channel_UPDATE:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);
      var_s data_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_DATA]);

      if (path_var == NULL || path_var->v_type != c_bi_type_string ||
          data_var == NULL)
      {
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (od_channel_client_s_message_call(this,od_channel_cbevt_UPDATE,id,
            loc_s_string_value(path_var),data_var))
      {
        throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  default:
    {/*{{{*/
      var_s resp_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_RESP]);
      unsigned resp_idx = pointer_tree_s_get_idx(&g_od_channel_json_string_map,resp_var);

      switch (resp_idx)
      {
      case od_channel_SET:
        {/*{{{*/
          var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);

          if (path_var == NULL || path_var->v_type != c_bi_type_string)
          {
            throw_error(OD_CHANNEL_MESSAGE_ERROR);
          }

          // - call callback -
          if (od_channel_client_s_message_call(this,od_channel_cbresp_SET,id,
                loc_s_string_value(path_var)))
          {
            throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
          }
        }/*}}}*/
        break;
      case od_channel_CMD:
        {/*{{{*/
          var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);

          if (path_var == NULL || path_var->v_type != c_bi_type_string)
          {
            throw_error(OD_CHANNEL_MESSAGE_ERROR);
          }

          // - call callback -
          if (od_channel_client_s_message_call(this,od_channel_cbresp_CMD,id,
                loc_s_string_value(path_var)))
          {
            throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
          }
        }/*}}}*/
        break;
      case od_channel_GET:
        {/*{{{*/
          var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);
          var_s data_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_DATA]);

          if (path_var == NULL || path_var->v_type != c_bi_type_string ||
              data_var == NULL)
          {
            throw_error(OD_CHANNEL_MESSAGE_ERROR);
          }

          // - call callback -
          if (od_channel_client_s_message_call(this,od_channel_cbresp_GET,id,
                loc_s_string_value(path_var),data_var))
          {
            throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
          }
        }/*}}}*/
        break;
      case od_channel_WATCH:
        {/*{{{*/
          var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);

          if (path_var == NULL || path_var->v_type != c_bi_type_string)
          {
            throw_error(OD_CHANNEL_MESSAGE_ERROR);
          }

          // - call callback -
          if (od_channel_client_s_message_call(this,od_channel_cbresp_WATCH,id,
                loc_s_string_value(path_var)))
          {
            throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
          }
        }/*}}}*/
        break;
      case od_channel_IGNORE:
        {/*{{{*/
          var_s path_var = loc_s_dict_get(msg_var,string_vars->data[od_channel_PATH]);

          if (path_var == NULL || path_var->v_type != c_bi_type_string)
          {
            throw_error(OD_CHANNEL_MESSAGE_ERROR);
          }

          // - call callback -
          if (od_channel_client_s_message_call(this,od_channel_cbresp_IGNORE,id,
                loc_s_string_value(path_var)))
          {
            throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
          }
        }/*}}}*/
        break;
      case od_channel_PING:
        {/*{{{*/

          // - update connect timer -
          struct itimerspec its_connect = {{0,0},OD_CHANNEL_PING_TIMEOUT};
          if (epoll_timer_s_settime(&this->connect_timer,&its_connect,0))
          {
            throw_error(OD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR);
          }
        }/*}}}*/
        break;
      default:
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }
    }/*}}}*/
  }

  return 0;
}/*}}}*/

int od_channel_client_s_fd_event(void *a_od_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  od_channel_client_s *this = (od_channel_client_s *)a_od_channel_client;

  // - store connecting state -
  int connecting = this->connection.connecting;

  if (channel_conn_s_fd_event(&this->connection,0,a_epoll_event,a_epoll))
  {
    channel_conn_s_clear(&this->connection);

    // - update connect timer -
    struct itimerspec its_connect = {{0,0},OD_CHANNEL_RECONNECT_PERIOD};
    if (epoll_timer_s_settime(&this->connect_timer,&its_connect,0))
    {
      throw_error(OD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR);
    }

    if (!connecting)
    {
      // - reset connected flag -
      this->connected = 0;

      // - call callback -
      if (od_channel_client_s_message_call(this,od_channel_cbreq_DROP))
      {
        throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }
  }
  else
  {
    if (connecting)
    {
      // - set connected flag -
      this->connected = 1;

      // - call callback -
      if (od_channel_client_s_message_call(this,od_channel_cbreq_NEW))
      {
        throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }

      // - update connect timer -
      struct itimerspec its_connect = {{0,0},OD_CHANNEL_PING_TIMEOUT};
      if (epoll_timer_s_settime(&this->connect_timer,&its_connect,0))
      {
        throw_error(OD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int od_channel_client_s_connect_time_event(void *a_od_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(OD_CHANNEL_CLIENT_TIMER_READ_ERROR);
  }

  od_channel_client_s *this = (od_channel_client_s *)a_od_channel_client;

  if (channel_conn_s_create_client(&this->connection,
        this->server_ip.data,this->server_port,od_channel_client_s_conn_message,this,0) ||
      epoll_s_fd_callback(a_epoll,&this->connection.epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,od_channel_client_s_fd_event,this,0))
  {
    // - update connect timer -
    struct itimerspec its_connect = {{0,0},OD_CHANNEL_RECONNECT_PERIOD};
    if (epoll_timer_s_settime(&this->connect_timer,&its_connect,0))
    {
      throw_error(OD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR);
    }
  }

  return 0;
}/*}}}*/

int od_channel_client_s_ping_time_event(void *a_od_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(OD_CHANNEL_CLIENT_TIMER_READ_ERROR);
  }

  od_channel_client_s *this = (od_channel_client_s *)a_od_channel_client;

  if (this->connected)
  {
    this->buffer.used = 0;
    bc_array_s_append_format(&this->buffer,"{\"type\":\"ping\",\"id\":%" HOST_LL_FORMAT "d}",this->message_id++);

    // - send message -
    if (od_channel_client_s_send_message(this,&this->buffer))
    {
      throw_error(OD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
    }
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void libchannel_odl_init()
{/*{{{*/
  json_parser_s_init(&g_od_channel_json_parser);
  pointer_tree_s_init(&g_od_channel_json_string_map);

  // - initialize json parser strings -
  CONT_INIT_CLEAR(var_s,dummy_var);
  unsigned jpi_length = strlen(g_od_json_parser_init);
  bc_array_s buffer = {jpi_length,jpi_length,(char *)g_od_json_parser_init};
  cassert(json_parser_s_parse(&g_od_channel_json_parser,&buffer,&dummy_var) == 0);

  // - initialize json string map -
  var_array_s *string_vars = &g_od_channel_json_parser.string_vars;

  unsigned s_idx = od_channel_FIRST;
  unsigned s_idx_end = od_channel_LAST;
  do {
    pointer_tree_s_insert(&g_od_channel_json_string_map,string_vars->data[s_idx]);
  } while(++s_idx <= s_idx_end);
}/*}}}*/

void libchannel_odl_clear()
{/*{{{*/
  json_parser_s_clear(&g_od_channel_json_parser);
  pointer_tree_s_clear(&g_od_channel_json_string_map);
}/*}}}*/
