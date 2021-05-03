
@begin
include "od_channel.h"
@end

// === constants and definitions ===============================================

const char *g_od_channel_strings[] =
{/*{{{*/
  "first",

  "id",

  "type",
  "resp",
  "path",
  "mod",
  "data",

  "set",
  "cmd",
  "list",
  "get",
  "watch",
  "ignore",
  "update",
  "ping",

  "options",

  "last",
};/*}}}*/

var_tree_s g_od_channel_vars;

// === methods of generated structures =========================================

// -- od_channel_s --
@begin
methods od_channel_s
@end

int od_channel_s_create(od_channel_s *this,const char *a_ip,unsigned short a_port,
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

  if(epoll_s_fd_callback(&this->server.epoll_fd,EPOLLIN | EPOLLPRI,od_channel_s_fd_event,this,0))
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
  CONT_INIT_CLEAR(json_parser_s,json_parser);
  brutal_assert(json_parser_s_parse(&json_parser,message,&msg_var) == 0);
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

  CONT_INIT_CLEAR(var_s,msg_var);
  CONT_INIT_CLEAR(json_parser_s,json_parser);
  if (json_parser_s_parse(&json_parser,a_message,&msg_var))
  {
    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  var_s id_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_ID].object);
  if (id_var == NULL || id_var->v_type != c_bi_type_integer)
  {
    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  lli id = loc_s_int_value(id_var);
  var_s type_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_TYPE].object);
  unsigned type_idx = var_tree_s_get_idx(&g_od_channel_vars,&type_var);

  switch (type_idx)
  {
  case od_channel_SET:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);
      var_s data_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_DATA].object);

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
      var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);
      var_s data_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_DATA].object);

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
  case od_channel_LIST:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);

      if (path_var == NULL || path_var->v_type != c_bi_type_string)
      {
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (od_channel_s_message_call(this,a_index,od_channel_cbreq_LIST,id,
            loc_s_string_value(path_var)))
      {
        throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_GET:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);

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
      var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);
      var_s options_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_OPTIONS].object);

      if ((path_var == NULL || path_var->v_type != c_bi_type_string) ||
          (options_var != NULL && options_var->v_type != c_bi_type_integer))
      {
        throw_error(OD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (od_channel_s_message_call(this,a_index,od_channel_cbreq_WATCH,id,
            loc_s_string_value(path_var),options_var == NULL ? 0 : loc_s_int_value(options_var)))
      {
        throw_error(OD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_IGNORE:
    {/*{{{*/
      var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);

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

int od_channel_s_fd_event(void *a_od_channel,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  od_channel_s *this = (od_channel_s *)a_od_channel;

  if(channel_server_s_fd_event(&this->server,a_index,a_epoll_event))
  {
    throw_error(OD_CHANNEL_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

// -- od_channel_client_s --
@begin
methods od_channel_client_s
@end

int od_channel_client_s_create(od_channel_client_s *this,
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
  if (epoll_s_timer_callback(&this->connect_timer,&its_connect,0,od_channel_client_s_connect_time_event,this,0))
  {
    throw_error(OD_CHANNEL_CLIENT_EPOLL_ERROR);
  }

  // - register ping timer -
  struct itimerspec its_ping = {OD_CHANNEL_PING_PERIOD,OD_CHANNEL_PING_PERIOD};
  if (epoll_s_timer_callback(&this->ping_timer,&its_ping,0,od_channel_client_s_ping_time_event,this,0))
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

  CONT_INIT_CLEAR(var_s,msg_var);
  CONT_INIT_CLEAR(json_parser_s,json_parser);
  if (json_parser_s_parse(&json_parser,a_message,&msg_var))
  {
    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  var_s id_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_ID].object);
  if (id_var == NULL || id_var->v_type != c_bi_type_integer)
  {
    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  lli id = loc_s_int_value(id_var);
  var_s type_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_TYPE].object);
  var_s resp_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_RESP].object);

  if (type_var != NULL)
  {/*{{{*/
    unsigned type_idx = var_tree_s_get_idx(&g_od_channel_vars,&type_var);

    switch (type_idx)
    {
    case od_channel_UPDATE:
      {/*{{{*/
        var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);
        var_s mod_var  = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_MOD].object);
        var_s data_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_DATA].object);

        if (path_var == NULL || path_var->v_type != c_bi_type_string ||
            (mod_var != NULL && mod_var->v_type != c_bi_type_string) ||
            data_var == NULL)
        {
          throw_error(OD_CHANNEL_MESSAGE_ERROR);
        }

        // - call callback -
        if (od_channel_client_s_message_call(this,od_channel_cbevt_UPDATE,id,
              loc_s_string_value(path_var),
              mod_var == NULL ? NULL : loc_s_string_value(mod_var),
              data_var))
        {
          throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
        }
      }/*}}}*/
      break;
    default:
      throw_error(OD_CHANNEL_MESSAGE_ERROR);
    }
  }/*}}}*/
  else if (resp_var != NULL)
  {/*{{{*/
    unsigned resp_idx = var_tree_s_get_idx(&g_od_channel_vars,&resp_var);

    switch (resp_idx)
    {
    case od_channel_SET:
      {/*{{{*/
        var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);

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
        var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);

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
    case od_channel_LIST:
      {/*{{{*/
        var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);
        var_s data_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_DATA].object);

        if (path_var == NULL || path_var->v_type != c_bi_type_string ||
            data_var == NULL)
        {
          throw_error(OD_CHANNEL_MESSAGE_ERROR);
        }

        // - call callback -
        if (od_channel_client_s_message_call(this,od_channel_cbresp_LIST,id,
              loc_s_string_value(path_var),data_var))
        {
          throw_error(OD_CHANNEL_CLIENT_CALLBACK_ERROR);
        }
      }/*}}}*/
      break;
    case od_channel_GET:
      {/*{{{*/
        var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);
        var_s data_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_DATA].object);

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
        var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);

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
        var_s path_var = loc_s_dict_get(msg_var,g_od_channel_vars.data[od_channel_PATH].object);

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
  else
  {
    throw_error(OD_CHANNEL_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int od_channel_client_s_fd_event(void *a_od_channel_client,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  od_channel_client_s *this = (od_channel_client_s *)a_od_channel_client;

  // - store connecting state -
  int connecting = this->connection.connecting;

  if (channel_conn_s_fd_event(&this->connection,0,a_epoll_event))
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

int od_channel_client_s_connect_time_event(void *a_od_channel_client,unsigned a_index,epoll_event_s *a_epoll_event)
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
      epoll_s_fd_callback(&this->connection.epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,od_channel_client_s_fd_event,this,0))
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

int od_channel_client_s_ping_time_event(void *a_od_channel_client,unsigned a_index,epoll_event_s *a_epoll_event)
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
  var_tree_s_init(&g_od_channel_vars);

  const char **s_ptr = g_od_channel_strings;
  const char **s_ptr_end = (void *)s_ptr + sizeof(g_od_channel_strings);
  do {
    VAR_CLEAR(value,loc_s_string_ptr(*s_ptr));
    var_tree_s_insert(&g_od_channel_vars,&value);
  } while(++s_ptr < s_ptr_end);
}/*}}}*/

void libchannel_odl_clear()
{/*{{{*/
  var_tree_s_clear(&g_od_channel_vars);
}/*}}}*/

