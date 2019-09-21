
@begin
include "td_channel.h"
@end

// === constants and definitions ===============================================

// === methods of generated structures =========================================

// -- td_channel_s --
@begin
methods td_channel_s
@end

int td_channel_s_create(td_channel_s *this,const char *a_ip,unsigned short a_port,epoll_s *a_epoll,
    td_channel_cbreq_t a_channel_callback,
    void *a_cb_object)
{/*{{{*/
  td_channel_s_clear(this);

  // - log message -
  log_info_2("channel server, create, ip: %s, port: %hu",a_ip,a_port);

  // - create channel server -
  if(channel_server_s_create(&this->server,a_ip,a_port,
        td_channel_s_conn_new,
        td_channel_s_conn_drop,
        td_channel_s_conn_message,
        this))
  {
    throw_error(TD_CHANNEL_SERVER_CREATE_ERROR);
  }

  if(epoll_s_fd_callback(a_epoll,&this->server.epoll_fd,EPOLLIN | EPOLLPRI,td_channel_s_fd_event,this,0))
  {
    throw_error(TD_CHANNEL_SERVER_EPOLL_ERROR);
  }

  this->channel_callback = a_channel_callback;
  this->cb_object = a_cb_object;

  return 0;
}/*}}}*/

int td_channel_s_conn_new(void *a_td_channel,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"td_channel_s_conn_new: %u\n",a_index));

  // - log message -
  log_info_2("channel server %u, new connection",a_index);

  td_channel_s *this = (td_channel_s *)a_td_channel;

  // - call callback -
  if (td_channel_s_message_call(this,a_index,td_channel_cbreq_NEW))
  {
    throw_error(TD_CHANNEL_SERVER_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int td_channel_s_conn_drop(void *a_td_channel,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"td_channel_s_conn_drop: %u\n",a_index));

  // - log message -
  log_info_2("channel server %u, drop connection",a_index);

  td_channel_s *this = (td_channel_s *)a_td_channel;

  // - call callback -
  if (td_channel_s_message_call(this,a_index,td_channel_cbreq_DROP))
  {
    throw_error(TD_CHANNEL_SERVER_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int td_channel_s_send_multi_message_var(td_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var)
{/*{{{*/
  const bc_array_s *message = loc_s_channel_message_value(a_data_var);

  if (a_indexes->used != 0)
  {
    VAR_CLEAR(length_var,loc_s_channel_message_buffer_length(message));

    const unsigned *i_ptr = a_indexes->data;
    const unsigned *i_ptr_end = i_ptr + a_indexes->used;
    do
    {
      // - log message -
      const bc_array_s *msg_buffer = loc_s_channel_message_value(a_data_var); // NOLINT
      log_info_2("channel server %u, --> " LOG_MSG_FORMAT,
          *i_ptr,LOG_MSG_PARAMETERS(msg_buffer->used,msg_buffer->data));

      // - schedule message to send -
      if (channel_conn_s_schedule_message_var(&this->server.conn_list.data[*i_ptr].object,length_var,a_data_var))
      {
        throw_error(TD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
      }
    } while(++i_ptr < i_ptr_end);
  }

  return 0;
}/*}}}*/

int td_channel_s_conn_message(void *a_td_channel,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  debug_message_5(fprintf(stderr,"conn_message: %u - %u bytes\n",a_index,a_message->used));

  // - log message -
  log_info_2("channel server %u, <-- %u bytes",a_index,a_message->used);

  td_channel_s *this = (td_channel_s *)a_td_channel;

  char *ptr = a_message->data;
  char *ptr_end = ptr + a_message->used;

  if (ptr_end - ptr >= 10)
  {
    ulli id;
    memcpy(&id,ptr,sizeof(ulli)); ptr += sizeof(ulli);
    id = be64toh(id);

    usi type;
    memcpy(&type,ptr,sizeof(usi)); ptr += sizeof(usi);
    type = be16toh(type);

    switch (type)
    {
      case td_channel_msg_type_REQUEST:
        {/*{{{*/
          if ((ptr_end - ptr) >= 4)
          {
            usi request;
            memcpy(&request,ptr,sizeof(usi)); ptr += sizeof(usi);
            request = be16toh(request);

            switch (request)
            {
              case td_channel_cbreq_TRACE_INFO:
              case td_channel_cbreq_TRACE_WRITE:
              case td_channel_cbreq_TRACE_READ:
              case td_channel_cbreq_TRACE_HEAD:
              case td_channel_cbreq_TRACE_TAIL:
                {
                  unsigned rest_length = a_message->used + (ptr - a_message->data);
                  unsigned trace_id_length = strnlen(ptr,rest_length);
                  if (trace_id_length < rest_length)
                  {
                    const string_s trace_id = {trace_id_length + 1,ptr};
                    ptr += trace_id.size;

                    switch (request)
                    {
                      case td_channel_cbreq_TRACE_INFO:
                      case td_channel_cbreq_TRACE_HEAD:
                      case td_channel_cbreq_TRACE_TAIL:
                        {/*{{{*/

                          // - call callback -
                          if (td_channel_s_message_call(
                                this,a_index,request,id,
                                &trace_id))
                          {
                            throw_error(TD_CHANNEL_SERVER_CALLBACK_ERROR);
                          }
                        }/*}}}*/
                        break;
                      case td_channel_cbreq_TRACE_WRITE:
                        {/*{{{*/
                          if (ptr_end - ptr >= 4)
                          {
                            ulli time;
                            memcpy(&time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                            time = be64toh(time);

                            const bc_array_s data = {ptr_end - ptr,ptr_end - ptr,ptr};

                            // - call callback -
                            if (td_channel_s_message_call(
                                  this,a_index,request,id,
                                  &trace_id,time,&data))
                            {
                              throw_error(TD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case td_channel_cbreq_TRACE_READ:
                        {/*{{{*/
                          if (ptr_end - ptr >= 4)
                          {
                            lli record_id;
                            memcpy(&record_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                            record_id = be64toh(record_id);

                            // - call callback -
                            if (td_channel_s_message_call(
                                  this,a_index,request,id,
                                  &trace_id,record_id))
                            {
                              throw_error(TD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                    }
                  }
                }
                break;
            }
          }
        }/*}}}*/
        break;
      case td_channel_msg_type_RESPONSE:
        break;
      case td_channel_msg_type_EVENT:
        break;
    }
  }

  return 0;
}/*}}}*/

int td_channel_s_fd_event(void *a_td_channel,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  td_channel_s *this = (td_channel_s *)a_td_channel;

  if(channel_server_s_fd_event(&this->server,a_index,a_epoll_event,a_epoll))
  {
    throw_error(TD_CHANNEL_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

// -- td_channel_client_s --
@begin
methods td_channel_client_s
@end

int td_channel_client_s_create(td_channel_client_s *this,epoll_s *a_epoll,
  const char *a_server_ip,unsigned short a_server_port,
  td_channel_cbreq_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  td_channel_client_s_clear(this);

  // - log message -
  log_info_2("channel client, create, server_ip: %s, server_port: %hu",
      a_server_ip,a_server_port);

  string_s_set_ptr(&this->server_ip,a_server_ip);
  this->server_port = a_server_port;
  this->message_id = 0;

  this->channel_callback = a_channel_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;

  // - register epoll timer -
  struct itimerspec itimerspec = {{0,0},{0,1}};
  if (epoll_s_timer_callback(a_epoll,&this->epoll_timer,&itimerspec,0,td_channel_client_s_connect_time_event,this,0))
  {
    throw_error(TD_CHANNEL_CLIENT_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int td_channel_client_s_conn_message(void *a_td_channel_client,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  (void)a_index;

  debug_message_6(fprintf(stderr,"td_channel_client_s_conn_message\n"));

  // - log message -
  log_info_2("channel client, <-- %u bytes",a_message->used);

  //td_channel_client_s *this = (td_channel_client_s *)a_td_channel_client;

  // FIXME TODO continue ...

  return 0;
}/*}}}*/

int td_channel_client_s_fd_event(void *a_td_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  td_channel_client_s *this = (td_channel_client_s *)a_td_channel_client;

  // - store connecting state -
  int connecting = this->connection.connecting;

  if (channel_conn_s_fd_event(&this->connection,0,a_epoll_event,a_epoll))
  {
    channel_conn_s_clear(&this->connection);

    // - register epoll timer -
    struct itimerspec itimerspec = {{0,0},{0,500000000}};
    if (epoll_s_timer_callback(a_epoll,&this->epoll_timer,&itimerspec,0,td_channel_client_s_connect_time_event,this,0))
    {
      throw_error(TD_CHANNEL_CLIENT_EPOLL_ERROR);
    }

    if (!connecting)
    {
      // - call callback -
      if (td_channel_client_s_message_call(this,td_channel_cbreq_DROP))
      {
        throw_error(TD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }
  }
  else
  {
    if (connecting)
    {
      // - call callback -
      if (td_channel_client_s_message_call(this,td_channel_cbreq_NEW))
      {
        throw_error(TD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int td_channel_client_s_connect_time_event(void *a_td_channel_client,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(TD_CHANNEL_CLIENT_TIMER_READ_ERROR);
  }

  td_channel_client_s *this = (td_channel_client_s *)a_td_channel_client;

  if (channel_conn_s_create_client(&this->connection,
        this->server_ip.data,this->server_port,td_channel_client_s_conn_message,this,0) ||
      epoll_s_fd_callback(a_epoll,&this->connection.epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,td_channel_client_s_fd_event,this,0))
  {
    // - register epoll timer -
    struct itimerspec itimerspec = {{0,0},{0,500000000}};
    if (epoll_s_timer_callback(a_epoll,&this->epoll_timer,&itimerspec,0,td_channel_client_s_connect_time_event,this,0))
    {
      throw_error(TD_CHANNEL_CLIENT_EPOLL_ERROR);
    }
  }

  return 0;
}/*}}}*/

