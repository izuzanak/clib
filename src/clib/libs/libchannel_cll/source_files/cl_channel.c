
@begin
include "cl_channel.h"
@end

// === methods of generated structures =========================================

// -- var_queue_s --
@begin
methods var_queue_s
@end

// -- channel_conn_s --
@begin
methods channel_conn_s
@end

unsigned g_type_channel_message = c_idx_not_exist;

void channel_conn_s_create(channel_conn_s *this,epoll_fd_s *a_epoll_fd,
    channel_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"channel_conn_s_create\n"));

  channel_conn_s_clear(this);

  epoll_fd_s_swap(&this->epoll_fd,a_epoll_fd);
  this->conn_message_callback = a_conn_message_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;
  this->in_msg_length = 0;
  this->out_msg_offset = 0;

  // - reset client connecting flag -
  this->connecting = 0;

}/*}}}*/

int channel_conn_s_create_client(channel_conn_s *this,
    const char *a_server_ip,unsigned short a_server_port,
    channel_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"channel_conn_s_create_client\n"));

  channel_conn_s_clear(this);

  this->conn_message_callback = a_conn_message_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;
  this->in_msg_length = 0;
  this->out_msg_offset = 0;

  int nonblock_io = 1;

  // - connect to server -
  socket_address_s address;
  if (socket_address_s_create(&address,a_server_ip,a_server_port) ||
      socket_s_create(&this->epoll_fd.fd,AF_INET,SOCK_STREAM) ||
      ioctl(this->epoll_fd.fd,FIONBIO,&nonblock_io))
  {
    throw_error(CHANNEL_CONN_CONNECT_ERROR);
  }

  // - connect to server -
  if (connect(this->epoll_fd.fd,(struct sockaddr *)&address,sizeof(struct sockaddr_in)) != 0)
  {
    if (errno != EINPROGRESS)
    {
      throw_error(CHANNEL_CONN_CONNECT_ERROR);
    }
  }

  // - set client connecting flag -
  this->connecting = 1;

  return 0;
}/*}}}*/

int channel_conn_s_recv_msg(channel_conn_s *this)
{/*{{{*/
  bc_array_s *msg = &this->in_msg;

  unsigned msg_old_used = msg->used;
  if (fd_s_read(&this->epoll_fd.fd,msg) || msg->used == msg_old_used)
  {
    throw_error(CHANNEL_CONN_READ_ERROR);
  }

  unsigned msg_offset = 0;
  do
  {
    if (this->in_msg_length == 0)
    {
      if ((msg->used - msg_offset) < 11)
      {
        break;
      }

      debug_message_6(fprintf(stderr,"message length: %.*s\n",11,msg->data + msg_offset));

      // - retrieve length of message -
      char *ptr = msg->data + msg_offset;
      char *end_ptr;

      this->in_msg_length = strtol(ptr,&end_ptr,16);

      if (end_ptr - ptr != 10)
      {
        throw_error(CHANNEL_CONN_READ_ERROR);
      }

      msg_offset += 11;
    }
    else
    {
      if ((msg->used - msg_offset) < this->in_msg_length)
      {
        break;
      }

      debug_message_6(fprintf(stderr,"recevived message: %.*s\n",this->in_msg_length,msg->data + msg_offset));

      bc_array_s message = {this->in_msg_length,this->in_msg_length,msg->data + msg_offset};

      // - call conn_message_callback -
      if (((channel_conn_message_callback_t)this->conn_message_callback)(this->cb_object,this->cb_index,&message))
      {
        throw_error(CHANNEL_CONN_CALLBACK_ERROR);
      }

      msg_offset += this->in_msg_length;

      // - reset message length -
      this->in_msg_length = 0;
    }
  } while(1);

  bc_array_s_tail(msg,msg->used - msg_offset);

  return 0;
}/*}}}*/

int channel_conn_s_send_msg(channel_conn_s *this)
{/*{{{*/
  if (this->out_msg_queue.used != 0)
  {
    const bc_array_s *message = loc_s_channel_message_value(*var_queue_s_first(&this->out_msg_queue));
    size_t write_cnt = message->used - this->out_msg_offset;

    // - limit maximal write size -
    if (write_cnt > 4096)
    {
      write_cnt = 4096;
    }

    ssize_t cnt = write(this->epoll_fd.fd,message->data + this->out_msg_offset,write_cnt);

    // - ERROR -
    if (cnt == -1)
    {
      throw_error(CHANNEL_CONN_WRITE_ERROR);
    }

    // - whole message was send -
    if ((this->out_msg_offset += cnt) >= message->used)
    {
      // - remove message from queue -
      var_queue_s_next(&this->out_msg_queue);

      // - reset out message offset -
      this->out_msg_offset = 0;
    }
  }
  else
  {
    // - modify fd epoll events: only input -
    if (epoll_fd_s_modify_events(&this->epoll_fd,EPOLLIN | EPOLLPRI))
    {
      throw_error(CHANNEL_CONN_EPOLL_ERROR);
    }
  }

  return 0;
}/*}}}*/

int channel_conn_s_fd_event(channel_conn_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;
  (void)a_epoll;

  if (a_epoll_event->data.fd != this->epoll_fd.fd)
  {
    throw_error(CHANNEL_CONN_INVALID_FD);
  }

  if (this->connecting)
  {
    int nonblock_io = 0;
    int error;
    socklen_t length = sizeof(error);

    // - check connect result -
    // - disable nonblocking io -
    // - modify fd epoll events: only input -
    if (getsockopt(this->epoll_fd.fd,SOL_SOCKET,SO_ERROR,&error,&length) ||
        error != 0 ||
        ioctl(this->epoll_fd.fd,FIONBIO,&nonblock_io) ||
        epoll_fd_s_modify_events(&this->epoll_fd,EPOLLIN | EPOLLPRI))
    {
      throw_error(CHANNEL_CONN_CONNECT_ERROR);
    }

    // - reset client connecting flag -
    this->connecting = 0;
  }
  else
  {
    if (a_epoll_event->events & EPOLLOUT)
    {
      if (channel_conn_s_send_msg(this))
      {
        throw_error(CHANNEL_CONN_SEND_ERROR);
      }
    }

    if (a_epoll_event->events & EPOLLIN)
    {
      if (channel_conn_s_recv_msg(this))
      {
        throw_error(CHANNEL_CONN_RECEIVE_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

// -- channel_conn_list_s --
@begin
methods channel_conn_list_s
@end

// -- channel_server_s --
@begin
methods channel_server_s
@end

int channel_server_s_create(channel_server_s *this,
  const char *a_ip,unsigned short a_port,
  channel_conn_new_callback_t a_conn_new_callback,
  channel_conn_drop_callback_t a_conn_drop_callback,
  channel_conn_message_callback_t a_conn_message_callback,
  void *a_cb_object)
{/*{{{*/
  debug_message_3(fprintf(stderr,"channel_server_s_create\n"));

  channel_server_s_clear(this);

  string_s_set_ptr(&this->ip,a_ip);
  this->port = a_port;

  this->conn_new_callback = a_conn_new_callback;
  this->conn_drop_callback = a_conn_drop_callback;
  this->conn_message_callback = a_conn_message_callback;
  this->cb_object = a_cb_object;

  // - open server socket -
  socket_address_s address;
  if (socket_address_s_create(&address,this->ip.data,this->port) ||
      socket_s_create(&this->epoll_fd.fd,AF_INET,SOCK_STREAM) ||
      socket_s_listen(&this->epoll_fd.fd,&address,256))
  {
    throw_error(CHANNEL_SERVER_LISTEN_ERROR);
  }

  return 0;
}/*}}}*/

int channel_server_s_fd_event(channel_server_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  if (a_epoll_event->data.fd != this->epoll_fd.fd)
  {
    throw_error(CHANNEL_SERVER_INVALID_FD);
  }

  CONT_INIT(epoll_fd_s,epoll_fd);
  socket_address_s address;

  if (socket_s_accept(&this->epoll_fd.fd,&epoll_fd.fd,&address))
  {
    throw_error(CHANNEL_SERVER_ACCEPT_ERROR);
  }

  unsigned conn_idx = channel_conn_list_s_append_blank(&this->conn_list);
  if (epoll_s_fd_callback(a_epoll,&epoll_fd,EPOLLIN | EPOLLPRI,channel_server_s_conn_fd_event,this,conn_idx))
  {
    epoll_fd_s_clear(&epoll_fd);
    channel_conn_list_s_remove(&this->conn_list,conn_idx);

    throw_error(CHANNEL_SERVER_ACCEPT_ERROR);
  }

  epoll_fd.epoll = a_epoll;
  channel_conn_s_create(&this->conn_list.data[conn_idx].object,
      &epoll_fd,this->conn_message_callback,this->cb_object,conn_idx);

  // - call conn_new_callback -
  if (((channel_conn_new_callback_t)this->conn_new_callback)(this->cb_object,conn_idx))
  {
    throw_error(CHANNEL_CONN_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int channel_server_s_conn_fd_event(void *a_channel_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  channel_server_s *this = (channel_server_s *)a_channel_server;
  channel_conn_s *conn = &this->conn_list.data[a_index].object;

  if (channel_conn_s_fd_event(conn,0,a_epoll_event,a_epoll))
  {
    // - call conn_drop_callback -
    if (((channel_conn_drop_callback_t)this->conn_drop_callback)(this->cb_object,a_index))
    {
      channel_conn_s_clear(conn);
      channel_conn_list_s_remove(&this->conn_list,a_index);

      throw_error(CHANNEL_CONN_CALLBACK_ERROR);
    }

    channel_conn_s_clear(conn);
    channel_conn_list_s_remove(&this->conn_list,a_index);
  }

  return 0;
}/*}}}*/

void libchannel_cll_init()
{/*{{{*/

  // - loc_s_register_type -
  g_type_channel_message = loc_s_register_type(
    loc_s_channel_message_clear,
    loc_s_channel_message_order,
#if OPTION_TO_STRING == ENABLED
    loc_s_channel_message_to_string,
#else
    NULL,
#endif
#if OPTION_TO_JSON == ENABLED
    loc_s_channel_message_to_json,
    loc_s_channel_message_to_json_nice
#else
    NULL,
    NULL
#endif
    );

}/*}}}*/

void libchannel_cll_clear()
{/*{{{*/
}/*}}}*/

