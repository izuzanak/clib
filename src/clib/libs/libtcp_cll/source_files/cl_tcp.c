
@begin
include "cl_tcp.h"
@end

// === var type tcp_message  ===================================================

unsigned g_type_tcp_message = c_idx_not_exist;

// === methods of generated structures =========================================

// -- tcp_conn_s --
@begin
methods tcp_conn_s
@end

void tcp_conn_s_create(tcp_conn_s *this,epoll_fd_s *a_epoll_fd,
    tcp_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"tcp_conn_s_create\n"));

  tcp_conn_s_clear(this);

  epoll_fd_s_swap(&this->epoll_fd,a_epoll_fd);
  this->conn_message_callback = a_conn_message_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;
  this->out_msg_offset = 0;

  // - reset client connecting flag -
  this->connecting = 0;
}/*}}}*/

int tcp_conn_s_create_client(tcp_conn_s *this,
    const char *a_server_ip,unsigned short a_server_port,
    tcp_conn_message_callback_t a_conn_message_callback,
    void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"tcp_conn_s_create_client\n"));

  tcp_conn_s_clear(this);

  this->conn_message_callback = a_conn_message_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;
  this->out_msg_offset = 0;

  int nonblock_io = 1;

  // - connect to server -
  socket_address_s address;
  if (socket_address_s_create(&address,a_server_ip,a_server_port) ||
      socket_s_create(&this->epoll_fd.fd,AF_INET,SOCK_STREAM) ||
      ioctl(this->epoll_fd.fd,FIONBIO,&nonblock_io))
  {
    throw_error(TCP_CONN_CONNECT_ERROR);
  }

  int yes = 1;
  if (setsockopt(this->epoll_fd.fd,SOL_TCP,TCP_NODELAY,&yes,sizeof(int)))
  {
    throw_error(TCP_CONN_SOCKOPT_ERROR);
  }

  // - connect to server -
  if (connect(this->epoll_fd.fd,(struct sockaddr *)&address,sizeof(struct sockaddr_in)) != 0)
  {
    if (errno != EINPROGRESS)
    {
      throw_error(TCP_CONN_CONNECT_ERROR);
    }
  }

  // - set client connecting flag -
  this->connecting = 1;

  return 0;
}/*}}}*/

int tcp_conn_s_recv_msg(tcp_conn_s *this)
{/*{{{*/
  bc_array_s *msg = &this->in_msg;

  unsigned msg_old_used = msg->used;
  if (fd_s_read(&this->epoll_fd.fd,msg) || msg->used == msg_old_used)
  {
    throw_error(TCP_CONN_READ_ERROR);
  }

  // - call conn_message_callback -
  if (((tcp_conn_message_callback_t)this->conn_message_callback)(this->cb_object,this->cb_index,msg))
  {
    throw_error(TCP_CONN_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int tcp_conn_s_send_msg(tcp_conn_s *this)
{/*{{{*/
  if (this->out_msg_queue.used != 0)
  {
    const bc_array_s *message = loc_s_tcp_message_value(*var_queue_s_first(&this->out_msg_queue));
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
      throw_error(TCP_CONN_WRITE_ERROR);
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
      throw_error(TCP_CONN_EPOLL_ERROR);
    }
  }

  return 0;
}/*}}}*/

int tcp_conn_s_fd_event(tcp_conn_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;
  (void)a_epoll;

  if (a_epoll_event->data.fd != this->epoll_fd.fd)
  {
    throw_error(TCP_CONN_INVALID_FD);
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
      throw_error(TCP_CONN_CONNECT_ERROR);
    }

    // - reset client connecting flag -
    this->connecting = 0;
  }
  else
  {
    if (a_epoll_event->events & EPOLLOUT)
    {
      if (tcp_conn_s_send_msg(this))
      {
        throw_error(TCP_CONN_SEND_ERROR);
      }
    }

    if (a_epoll_event->events & EPOLLIN)
    {
      if (tcp_conn_s_recv_msg(this))
      {
        throw_error(TCP_CONN_RECEIVE_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

// -- tcp_conn_list_s --
@begin
methods tcp_conn_list_s
@end

// -- tcp_server_s --
@begin
methods tcp_server_s
@end

int tcp_server_s_create(tcp_server_s *this,
  const char *a_ip,unsigned short a_port,
  tcp_conn_new_callback_t a_conn_new_callback,
  tcp_conn_drop_callback_t a_conn_drop_callback,
  tcp_conn_message_callback_t a_conn_message_callback,
  void *a_cb_object)
{/*{{{*/
  debug_message_3(fprintf(stderr,"tcp_server_s_create\n"));

  tcp_server_s_clear(this);

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
    throw_error(TCP_SERVER_LISTEN_ERROR);
  }

  int yes = 1;
  if (setsockopt(this->epoll_fd.fd,SOL_TCP,TCP_NODELAY,&yes,sizeof(int)))
  {
    throw_error(TCP_SERVER_SOCKOPT_ERROR);
  }

  return 0;
}/*}}}*/

int tcp_server_s_fd_event(tcp_server_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  if (a_epoll_event->data.fd != this->epoll_fd.fd)
  {
    throw_error(TCP_SERVER_INVALID_FD);
  }

  CONT_INIT(epoll_fd_s,epoll_fd);
  socket_address_s address;

  if (socket_s_accept(&this->epoll_fd.fd,&epoll_fd.fd,&address))
  {
    throw_error(TCP_SERVER_ACCEPT_ERROR);
  }

  unsigned conn_idx = tcp_conn_list_s_append_blank(&this->conn_list);
  if (epoll_s_fd_callback(a_epoll,&epoll_fd,EPOLLIN | EPOLLPRI,tcp_server_s_conn_fd_event,this,conn_idx))
  {
    epoll_fd_s_clear(&epoll_fd);
    tcp_conn_list_s_remove(&this->conn_list,conn_idx);

    throw_error(TCP_SERVER_ACCEPT_ERROR);
  }

  epoll_fd.epoll = a_epoll;
  tcp_conn_s_create(&this->conn_list.data[conn_idx].object,
      &epoll_fd,this->conn_message_callback,this->cb_object,conn_idx);

  // - call conn_new_callback -
  if (((tcp_conn_new_callback_t)this->conn_new_callback)(this->cb_object,conn_idx))
  {
    throw_error(TCP_CONN_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int tcp_server_s_conn_fd_event(void *a_tcp_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  tcp_server_s *this = (tcp_server_s *)a_tcp_server;
  tcp_conn_s *conn = &this->conn_list.data[a_index].object;

  if (tcp_conn_s_fd_event(conn,0,a_epoll_event,a_epoll))
  {
    // - call conn_drop_callback -
    if (((tcp_conn_drop_callback_t)this->conn_drop_callback)(this->cb_object,a_index))
    {
      tcp_conn_s_clear(conn);
      tcp_conn_list_s_remove(&this->conn_list,a_index);

      throw_error(TCP_CONN_CALLBACK_ERROR);
    }

    tcp_conn_s_clear(conn);
    tcp_conn_list_s_remove(&this->conn_list,a_index);
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void libtcp_cll_init()
{/*{{{*/

  // - loc_s_register_type -
  g_type_tcp_message = loc_s_register_type(
    loc_s_tcp_message_clear,
    loc_s_tcp_message_order,
#if OPTION_TO_STRING == ENABLED
    loc_s_tcp_message_to_string,
#else
    NULL,
#endif
#if OPTION_TO_JSON == ENABLED
    loc_s_tcp_message_to_json,
    loc_s_tcp_message_to_json_nice
#else
    NULL,
    NULL
#endif
    );

}/*}}}*/

void libtcp_cll_clear()
{/*{{{*/
}/*}}}*/

