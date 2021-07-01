
@begin
include "wd_channel.h"
@end

// === constants and definitions ===============================================

const char *g_wd_channel_strings[] =
{/*{{{*/
  "first",

  "id",

  "type",
  "resp",
  "name",
  "timeout",
  "data",

  "enable",
  "disable",
  "keepalive",
  "status",
  "ping",

  "last",
};/*}}}*/

var_tree_s g_wd_channel_vars;

// === methods of generated structures =========================================

// -- wd_channel_s --
@begin
methods wd_channel_s
@end

int wd_channel_s_create(wd_channel_s *this,const char *a_ip,unsigned short a_port,
    wd_channel_cbreq_t a_channel_callback,
    void *a_cb_object)
{/*{{{*/
  wd_channel_s_clear(this);

  // - log message -
  log_info_2("channel server, create, ip: %s, port: %hu",a_ip,a_port);

  // - create channel server -
  if(channel_server_s_create(&this->server,a_ip,a_port,
        wd_channel_s_conn_new,
        wd_channel_s_conn_drop,
        wd_channel_s_conn_message,
        this))
  {
    throw_error(WD_CHANNEL_SERVER_CREATE_ERROR);
  }

  if(epoll_s_fd_callback(&this->server.epoll_fd,EPOLLIN | EPOLLPRI,wd_channel_s_fd_event,this,0))
  {
    throw_error(WD_CHANNEL_SERVER_EPOLL_ERROR);
  }

  this->channel_callback = a_channel_callback;
  this->cb_object = a_cb_object;

  return 0;
}/*}}}*/

int wd_channel_s_conn_new(void *a_wd_channel,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"wd_channel_s_conn_new: %u\n",a_index));

  // - log message -
  log_info_2("channel server %u, new connection",a_index);

  wd_channel_s *this = (wd_channel_s *)a_wd_channel;

  // - call callback -
  if (wd_channel_s_message_call(this,a_index,wd_channel_cbreq_NEW))
  {
    throw_error(WD_CHANNEL_SERVER_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int wd_channel_s_conn_drop(void *a_wd_channel,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"wd_channel_s_conn_drop: %u\n",a_index));

  // - log message -
  log_info_2("channel server %u, drop connection",a_index);

  wd_channel_s *this = (wd_channel_s *)a_wd_channel;

  // - call callback -
  if (wd_channel_s_message_call(this,a_index,wd_channel_cbreq_DROP))
  {
    throw_error(WD_CHANNEL_SERVER_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int wd_channel_s_send_multi_message_var(wd_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var)
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
        throw_error(WD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
      }
    } while(++i_ptr < i_ptr_end);
  }

  return 0;
}/*}}}*/

int wd_channel_s_conn_message(void *a_wd_channel,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  debug_message_5(fprintf(stderr,"conn_message: %u - %u bytes\n",a_index,a_message->used));

  // - log message -
  log_info_2("channel server %u, <-- " LOG_MSG_FORMAT,a_index,
      LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  wd_channel_s *this = (wd_channel_s *)a_wd_channel;

  CONT_INIT_CLEAR(var_s,msg_var);
  CONT_INIT_CLEAR(json_parser_s,json_parser);
  if (json_parser_s_parse(&json_parser,a_message,&msg_var))
  {
    throw_error(WD_CHANNEL_MESSAGE_ERROR);
  }

  var_s id_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_ID].object);
  if (id_var == NULL || id_var->v_type != c_bi_type_integer)
  {
    throw_error(WD_CHANNEL_MESSAGE_ERROR);
  }

  lli id = loc_s_int_value(id_var);
  var_s type_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_TYPE].object);
  unsigned type_idx = var_tree_s_get_idx(&g_wd_channel_vars,&type_var);

  switch (type_idx)
  {
  case wd_channel_ENABLE:
    {/*{{{*/
      var_s name_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_NAME].object);
      var_s timeout_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_TIMEOUT].object);

      if (name_var == NULL || name_var->v_type != c_bi_type_string ||
          timeout_var == NULL || timeout_var->v_type != c_bi_type_integer)
      {
        throw_error(WD_CHANNEL_MESSAGE_ERROR);
      }

      lli timeout = loc_s_int_value(timeout_var);
      if (timeout <= 0)
      {
        throw_error(WD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (wd_channel_s_message_call(this,a_index,wd_channel_cbreq_ENABLE,id,
            loc_s_string_value(name_var),timeout))
      {
        throw_error(WD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_DISABLE:
    {/*{{{*/
      var_s name_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_NAME].object);

      if (name_var == NULL || name_var->v_type != c_bi_type_string)
      {
        throw_error(WD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (wd_channel_s_message_call(this,a_index,wd_channel_cbreq_DISABLE,id,
            loc_s_string_value(name_var)))
      {
        throw_error(WD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_KEEPALIVE:
    {/*{{{*/
      var_s name_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_NAME].object);

      if (name_var == NULL || name_var->v_type != c_bi_type_string)
      {
        throw_error(WD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (wd_channel_s_message_call(this,a_index,wd_channel_cbreq_KEEPALIVE,id,
            loc_s_string_value(name_var)))
      {
        throw_error(WD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_STATUS:
    {/*{{{*/

      // - call callback -
      if (wd_channel_s_message_call(this,a_index,wd_channel_cbreq_STATUS,id))
      {
        throw_error(WD_CHANNEL_SERVER_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_PING:
    {/*{{{*/
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"ping\",\"id\":%" HOST_LL_FORMAT "d}",id);

      // - send message -
      if (wd_channel_s_send_message(this,a_index,&this->buffer))
      {
        throw_error(WD_CHANNEL_SERVER_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  default:
    throw_error(WD_CHANNEL_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int wd_channel_s_fd_event(void *a_wd_channel,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  wd_channel_s *this = (wd_channel_s *)a_wd_channel;

  if(channel_server_s_fd_event(&this->server,a_index,a_epoll_event))
  {
    throw_error(WD_CHANNEL_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

// -- wd_channel_client_s --
@begin
methods wd_channel_client_s
@end

int wd_channel_client_s_create(wd_channel_client_s *this,
  const char *a_server_ip,unsigned short a_server_port,
  wd_channel_cbreq_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  wd_channel_client_s_clear(this);

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
  if (epoll_s_timer_callback(&this->connect_timer,&its_connect,0,wd_channel_client_s_connect_time_event,this,0))
  {
    throw_error(WD_CHANNEL_CLIENT_EPOLL_ERROR);
  }

  // - register ping timer -
  struct itimerspec its_ping = {WD_CHANNEL_PING_PERIOD,WD_CHANNEL_PING_PERIOD};
  if (epoll_s_timer_callback(&this->ping_timer,&its_ping,0,wd_channel_client_s_ping_time_event,this,0))
  {
    throw_error(WD_CHANNEL_CLIENT_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int wd_channel_client_s_conn_message(void *a_wd_channel_client,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  (void)a_wd_channel_client;
  (void)a_index;

  debug_message_6(fprintf(stderr,"wd_channel_client_s_conn_message\n"));

  // - log message -
  log_info_2("channel client, <-- " LOG_MSG_FORMAT,
    LOG_MSG_PARAMETERS(a_message->used,a_message->data));

  wd_channel_client_s *this = (wd_channel_client_s *)a_wd_channel_client;

  CONT_INIT_CLEAR(var_s,msg_var);
  CONT_INIT_CLEAR(json_parser_s,json_parser);
  if (json_parser_s_parse(&json_parser,a_message,&msg_var))
  {
    throw_error(WD_CHANNEL_MESSAGE_ERROR);
  }

  var_s id_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_ID].object);
  if (id_var == NULL || id_var->v_type != c_bi_type_integer)
  {
    throw_error(WD_CHANNEL_MESSAGE_ERROR);
  }

  lli id = loc_s_int_value(id_var);
  var_s resp_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_RESP].object);
  unsigned resp_idx = var_tree_s_get_idx(&g_wd_channel_vars,&resp_var);

  switch (resp_idx)
  {
  case wd_channel_ENABLE:
    {/*{{{*/
      // - call callback -
      if (wd_channel_client_s_message_call(this,wd_channel_cbresp_ENABLE,id))
      {
        throw_error(WD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_DISABLE:
    {/*{{{*/
      // - call callback -
      if (wd_channel_client_s_message_call(this,wd_channel_cbresp_DISABLE,id))
      {
        throw_error(WD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_STATUS:
    {/*{{{*/
      var_s data_var = loc_s_dict_get(msg_var,g_wd_channel_vars.data[wd_channel_DATA].object);

      if (data_var == NULL)
      {
        throw_error(WD_CHANNEL_MESSAGE_ERROR);
      }

      // - call callback -
      if (wd_channel_client_s_message_call(this,wd_channel_cbresp_STATUS,id,
            data_var))
      {
        throw_error(WD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_PING:
    {/*{{{*/

      // - update connect timer -
      struct itimerspec its_connect = {{0,0},WD_CHANNEL_PING_TIMEOUT};
      if (epoll_timer_s_settime(&this->connect_timer,&its_connect,0))
      {
        throw_error(WD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR);
      }
    }/*}}}*/
    break;
  default:
    throw_error(WD_CHANNEL_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int wd_channel_client_s_fd_event(void *a_wd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  wd_channel_client_s *this = (wd_channel_client_s *)a_wd_channel_client;

  // - store connecting state -
  int connecting = this->connection.connecting;

  if (channel_conn_s_fd_event(&this->connection,0,a_epoll_event))
  {
    channel_conn_s_clear(&this->connection);

    // - update connect timer -
    struct itimerspec its_connect = {{0,0},WD_CHANNEL_RECONNECT_PERIOD};
    if (epoll_timer_s_settime(&this->connect_timer,&its_connect,0))
    {
      throw_error(WD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR);
    }

    if (!connecting)
    {
      // - reset connected flag -
      this->connected = 0;

      // - call callback -
      if (wd_channel_client_s_message_call(this,wd_channel_cbreq_DROP))
      {
        throw_error(WD_CHANNEL_CLIENT_CALLBACK_ERROR);
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
      if (wd_channel_client_s_message_call(this,wd_channel_cbreq_NEW))
      {
        throw_error(WD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }

      // - update connect timer -
      struct itimerspec its_connect = {{0,0},WD_CHANNEL_PING_TIMEOUT};
      if (epoll_timer_s_settime(&this->connect_timer,&its_connect,0))
      {
        throw_error(WD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int wd_channel_client_s_connect_time_event(void *a_wd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(WD_CHANNEL_CLIENT_TIMER_READ_ERROR);
  }

  wd_channel_client_s *this = (wd_channel_client_s *)a_wd_channel_client;

  if (channel_conn_s_create_client(&this->connection,
        this->server_ip.data,this->server_port,wd_channel_client_s_conn_message,this,0) ||
      epoll_s_fd_callback(&this->connection.epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,wd_channel_client_s_fd_event,this,0))
  {
    // - update connect timer -
    struct itimerspec its_connect = {{0,0},WD_CHANNEL_RECONNECT_PERIOD};
    if (epoll_timer_s_settime(&this->connect_timer,&its_connect,0))
    {
      throw_error(WD_CHANNEL_CLIENT_TIMER_SETTIME_ERROR);
    }
  }

  return 0;
}/*}}}*/

int wd_channel_client_s_ping_time_event(void *a_wd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(WD_CHANNEL_CLIENT_TIMER_READ_ERROR);
  }

  wd_channel_client_s *this = (wd_channel_client_s *)a_wd_channel_client;

  if (this->connected)
  {
    this->buffer.used = 0;
    bc_array_s_append_format(&this->buffer,"{\"type\":\"ping\",\"id\":%" HOST_LL_FORMAT "d}",this->message_id++);

    // - send message -
    if (wd_channel_client_s_send_message(this,&this->buffer))
    {
      throw_error(WD_CHANNEL_CLIENT_SEND_MESSAGE_ERROR);
    }
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void libchannel_wdl_init()
{/*{{{*/
  var_tree_s_init(&g_wd_channel_vars);

  const char **s_ptr = g_wd_channel_strings;
  const char **s_ptr_end = (void *)s_ptr + sizeof(g_wd_channel_strings);
  do {
    VAR_CLEAR(value,loc_s_string_ptr(*s_ptr));
    var_tree_s_insert(&g_wd_channel_vars,&value);
  } while(++s_ptr < s_ptr_end);
}/*}}}*/

void libchannel_wdl_clear()
{/*{{{*/
  var_tree_s_clear(&g_wd_channel_vars);
}/*}}}*/

