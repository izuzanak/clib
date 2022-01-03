
@begin
include "cl_rtsp.h"
@end

// === methods of generated structures =========================================

// -- x_session_map_s --
@begin
methods x_session_map_s
@end

// -- x_session_map_tree_s --
@begin
methods x_session_map_tree_s
@end

// -- rtsp_server_s --
@begin
methods rtsp_server_s
@end

int rtsp_server_s_create(rtsp_server_s *this,
    const string_s *a_ip,unsigned short a_port,
    rtsp_conn_new_callback_t a_conn_new_callback,
    rtsp_conn_drop_callback_t a_conn_drop_callback,
    rtsp_conn_get_sdp_callback_t a_conn_get_sdp_callback,
    rtsp_conn_check_media_callback_t a_conn_check_media_callback,
    rtsp_conn_playing_callback_t a_conn_playing_callback,
    rtsp_conn_get_packet_callback_t a_conn_get_packet_callback,
    void *a_cb_object)
{/*{{{*/
  debug_message_3(fprintf(stderr,"rtsp_server_s_create\n"));

  rtsp_server_s_clear(this);

  string_s_copy(&this->ip,a_ip);
  this->port = a_port;
  this->conn_new_callback = a_conn_new_callback;
  this->conn_drop_callback = a_conn_drop_callback;
  this->conn_get_sdp_callback = a_conn_get_sdp_callback;
  this->conn_check_media_callback = a_conn_check_media_callback;
  this->conn_playing_callback = a_conn_playing_callback;
  this->conn_get_packet_callback = a_conn_get_packet_callback;
  this->cb_object = a_cb_object;

  // - open server socket -
  socket_address_s address;
  if (socket_address_s_create(&address,this->ip.data,this->port) ||
      socket_s_create(&this->epoll_fd.fd,AF_INET,SOCK_STREAM) ||
      socket_s_listen(&this->epoll_fd.fd,&address,256))
  {
    this->state = c_rtsp_server_state_ERROR;
    throw_error(RTSP_SERVER_LISTEN_ERROR);
  }

  this->state = c_rtsp_server_state_LISTEN;

  return 0;
}/*}}}*/

#ifdef CLIB_WITH_OPENSSL
int rtsp_server_s_init_ssl(rtsp_server_s *this,const char *a_cert_file,const char *a_pkey_file)
{/*{{{*/

  // - ERROR -
  if (ssl_context_s_create_server(&this->ssl_ctx) ||
      ssl_context_s_use_certificate_file(&this->ssl_ctx,a_cert_file,SSL_FILETYPE_PEM) ||
      ssl_context_s_use_private_key_file(&this->ssl_ctx,a_pkey_file,SSL_FILETYPE_PEM))
  {
    ssl_context_s_clear(&this->ssl_ctx);

    throw_error(RTSP_SERVER_SSL_INIT_ERROR);
  }

  return 0;
}/*}}}*/
#endif

int rtsp_server_s_fd_event(rtsp_server_s *this,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  if (a_epoll_event->data.fd != this->epoll_fd.fd)
  {
    throw_error(RTSP_SERVER_INVALID_FD);
  }

  switch (this->state)
  {
    case c_rtsp_server_state_LISTEN:
      {/*{{{*/
        CONT_INIT(epoll_fd_s,epoll_fd);
        socket_address_s address;

        if (socket_s_accept(&this->epoll_fd.fd,&epoll_fd.fd,&address))
        {
          throw_error(RTSP_SERVER_ACCEPT_ERROR);
        }

        unsigned conn_idx = rtsp_conn_list_s_append_blank(&this->conn_list);
        if (epoll_s_fd_callback(&epoll_fd,EPOLLIN | EPOLLPRI,rtsp_server_s_conn_fd_event,this,conn_idx))
        {
          epoll_fd_s_clear(&epoll_fd);
          rtsp_conn_list_s_remove(&this->conn_list,conn_idx);

          throw_error(RTSP_SERVER_ACCEPT_ERROR);
        }

        if (rtsp_conn_s_create(&this->conn_list.data[conn_idx].object,
            this,conn_idx,&epoll_fd,&address))
        {
          rtsp_conn_list_s_remove(&this->conn_list,conn_idx);

          throw_error(RTSP_SERVER_CONN_CREATE_ERROR);
        }

#ifdef CLIB_WITH_OPENSSL
        if (this->ssl_ctx != NULL)
        {
          rtsp_conn_s *conn = rtsp_conn_list_s_at(&this->conn_list,conn_idx);

          if (ssl_conn_s_create(&conn->ssl,&this->ssl_ctx,conn->epoll_fd.fd))
          {
            throw_error(RTSP_SERVER_SSL_ACCEPT_ERROR);
          }

          ssl_conn_s_set_accept_state(&conn->ssl);
        }
#endif

        // - call conn_new_callback -
        if (((rtsp_conn_new_callback_t)this->conn_new_callback)(this->cb_object,conn_idx))
        {
          rtsp_conn_s_clear(&this->conn_list.data[conn_idx].object);
          rtsp_conn_list_s_remove(&this->conn_list,conn_idx);

          throw_error(RTSP_SERVER_CONN_CREATE_ERROR);
        }
      }/*}}}*/
      break;

    default:
      throw_error(RTSP_SERVER_INVALID_STATE);
  }

  return 0;
}/*}}}*/

int rtsp_server_s_conn_time_event(void *a_rtsp_server,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(RTSP_SERVER_TIMER_READ_ERROR);
  }

  rtsp_server_s *this = (rtsp_server_s *)a_rtsp_server;
  rtsp_conn_s *conn = &this->conn_list.data[a_index].object;

  if (rtsp_conn_s_time_event(conn,0,a_epoll_event))
  {
    // - call conn_drop_callback -
    (void)((rtsp_conn_drop_callback_t)this->conn_drop_callback)(this->cb_object,a_index);

    // - remove x session map -
    if (conn->x_session_map_idx != c_idx_not_exist)
    {
      x_session_map_tree_s_remove(&this->x_session_map_tree,conn->x_session_map_idx);
    }

    rtsp_conn_s_clear(conn);
    rtsp_conn_list_s_remove(&this->conn_list,a_index);
  }

  return 0;
}/*}}}*/

int rtsp_server_s_conn_fd_event(void *a_rtsp_server,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  rtsp_server_s *this = (rtsp_server_s *)a_rtsp_server;
  rtsp_conn_s *conn = &this->conn_list.data[a_index].object;

  if (rtsp_conn_s_fd_event(conn,0,a_epoll_event))
  {
    // - call conn_drop_callback -
    (void)((rtsp_conn_drop_callback_t)this->conn_drop_callback)(this->cb_object,a_index);

    // - remove x session map -
    if (conn->x_session_map_idx != c_idx_not_exist)
    {
      x_session_map_tree_s_remove(&this->x_session_map_tree,conn->x_session_map_idx);
    }

    rtsp_conn_s_clear(conn);
    rtsp_conn_list_s_remove(&this->conn_list,a_index);
  }

  return 0;
}/*}}}*/

