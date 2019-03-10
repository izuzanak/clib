
@begin
include "cl_rtsp.h"
@end

// === methods of generated structures =========================================

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

int rtsp_server_s_fd_event(rtsp_server_s *this,unsigned a_index,int a_fd,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;

  if (a_fd != this->epoll_fd.fd)
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
        if (epoll_s_fd_callback(a_epoll,&epoll_fd,EPOLLIN | EPOLLPRI,rtsp_server_s_conn_fd_event,this,conn_idx))
        {
          epoll_fd_s_clear(&epoll_fd);
          rtsp_conn_list_s_remove(&this->conn_list,conn_idx);

          throw_error(RTSP_SERVER_ACCEPT_ERROR);
        }

        epoll_fd.epoll = a_epoll;
        rtsp_conn_s_create(&this->conn_list.data[conn_idx].object,
            this,conn_idx,&epoll_fd);

        // - call conn_new_callback -
        ((rtsp_conn_new_callback_t)this->conn_new_callback)(this->cb_object,conn_idx);
      }/*}}}*/
      break;

    default:
      throw_error(RTSP_SERVER_INVALID_STATE);
  }

  return 0;
}/*}}}*/

void rtsp_server_s_conn_time_event(void *a_rtsp_server,unsigned a_index,unsigned a_timer,epoll_s *a_epoll)
{/*{{{*/
  rtsp_server_s *this = (rtsp_server_s *)a_rtsp_server;
  rtsp_conn_s *conn = &this->conn_list.data[a_index].object;

  if (rtsp_conn_s_time_event(conn,0,a_timer,a_epoll))
  {
    // - call conn_drop_callback -
    ((rtsp_conn_drop_callback_t)this->conn_drop_callback)(this->cb_object,a_index);

    rtsp_conn_s_clear(conn);
    rtsp_conn_list_s_remove(&this->conn_list,a_index);
  }
}/*}}}*/

void rtsp_server_s_conn_fd_event(void *a_rtsp_server,unsigned a_index,int a_fd,epoll_s *a_epoll)
{/*{{{*/
  rtsp_server_s *this = (rtsp_server_s *)a_rtsp_server;
  rtsp_conn_s *conn = &this->conn_list.data[a_index].object;

  if (rtsp_conn_s_fd_event(conn,0,a_fd,a_epoll))
  {
    // - call conn_drop_callback -
    ((rtsp_conn_drop_callback_t)this->conn_drop_callback)(this->cb_object,a_index);

    rtsp_conn_s_clear(conn);
    rtsp_conn_list_s_remove(&this->conn_list,a_index);
  }
}/*}}}*/

