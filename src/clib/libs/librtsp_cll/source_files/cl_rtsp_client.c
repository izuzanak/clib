
@begin
include "cl_rtsp.h"
@end

// === methods of generated structures =========================================

// -- rtsp_client_s --
@begin
methods rtsp_client_s
@end

int rtsp_client_s_create(rtsp_client_s *this,
    const string_s *a_server_ip,unsigned short a_server_port,const string_s *a_media,
    rtsp_recv_sdp_callback_t a_recv_sdp_callback,
    rtsp_recv_packet_callback_t a_recv_packet_callback,
    void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"rtsp_client_s_create\n"));

  rtsp_client_s_clear(this);

  string_s_copy(&this->server_ip,a_server_ip);
  this->server_port = a_server_port;
  string_s_set_format(&this->media_url,"rtsp://%s:%d/%s",a_server_ip->data,a_server_port,a_media->data);
  this->recv_sdp_callback = a_recv_sdp_callback;
  this->recv_packet_callback = a_recv_packet_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;

  int nonblock_io = 1;

  // - create and setup client socket -
  socket_address_s address;
  if (socket_address_s_create(&address,this->server_ip.data,this->server_port) ||
      socket_s_create(&this->epoll_fd.fd,AF_INET,SOCK_STREAM) ||
      ioctl(this->epoll_fd.fd,FIONBIO,&nonblock_io))
  {
    this->state = c_rtsp_client_state_ERROR;
    throw_error(RTSP_CLIENT_CONNECT_ERROR);
  }

  // - connect to server -
  if (connect(this->epoll_fd.fd,(struct sockaddr *)&address,sizeof(struct sockaddr_in)) != 0)
  {
    if (errno != EINPROGRESS)
    {
      this->state = c_rtsp_client_state_ERROR;
      throw_error(RTSP_CLIENT_CONNECT_ERROR);
    }
  }

  // - connect in progress -
  this->state = c_rtsp_client_state_CONNECTING;
  return 0;
}/*}}}*/

#ifdef CLIB_WITH_OPENSSL
int rtsp_client_s_init_ssl(rtsp_client_s *this)
{/*{{{*/
  CONT_INIT_CLEAR(ssl_context_s,ssl_ctx);

  // - ERROR -
  if (ssl_context_s_create_client(&ssl_ctx) ||
      ssl_conn_s_create(&this->ssl,&ssl_ctx,this->epoll_fd.fd))
  {
    throw_error(RTSP_CLIENT_SSL_INIT_ERROR);
  }

  ssl_conn_s_set_connect_state(&this->ssl);

  return 0;
}/*}}}*/
#endif

int rtsp_client_s_send_cmd(rtsp_client_s *this)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_client_s >>>>>\n%.*s",this->out_msg.used,this->out_msg.data));

#ifdef CLIB_WITH_OPENSSL
  if (this->ssl != NULL)
  {
    return ssl_conn_s_write(&this->ssl,this->out_msg.data,this->out_msg.used);
  }
  else
  {
#endif
    return fd_s_write(&this->epoll_fd.fd,this->out_msg.data,this->out_msg.used);
#ifdef CLIB_WITH_OPENSSL
  }
#endif
}/*}}}*/

int rtsp_client_s_recv_cmd_resp(rtsp_client_s *this)
{/*{{{*/
  bc_array_s *msg = &this->in_msg;

  unsigned msg_old_used = msg->used;
  if ((
#ifdef CLIB_WITH_OPENSSL
      this->ssl != NULL ? ssl_conn_s_read(&this->ssl,this->epoll_fd.fd,msg) :
#endif
      fd_s_read(&this->epoll_fd.fd,msg)) || msg->used == msg_old_used)
  {
    throw_error(RTSP_CLIENT_READ_ERROR);
  }

  // - parse command response -
  string_s string = {msg->used + 1,msg->data};

  // - parse check command response -
  if (rtsp_parser_s_parse(&this->parser,&string,0))
  {
    return 0;
  }

  debug_message_6(fprintf(stderr,"rtsp_client_s <<<<<\n%.*s\n",this->parser.input_idx,msg->data));

  // - parse process command response -
  if (rtsp_parser_s_parse(&this->parser,&string,1))
  {
    throw_error(RTSP_CONN_PARSE_ERROR);
  }

  bc_array_s_tail(msg,msg->used - this->parser.input_idx);

  return 0;
}/*}}}*/

int rtsp_client_s_recv_cmd_resp_or_data(rtsp_client_s *this)
{/*{{{*/
  bc_array_s *msg = &this->in_msg;

  unsigned msg_old_used = msg->used;

  // - read message header -
  if (msg->used < 4)
  {
  if ((
#ifdef CLIB_WITH_OPENSSL
      this->ssl != NULL ? ssl_conn_s_read(&this->ssl,this->epoll_fd.fd,msg) :
#endif
      fd_s_read(&this->epoll_fd.fd,msg)) || msg->used == msg_old_used)
    {
      throw_error(RTSP_CLIENT_READ_ERROR);
    }

    if (msg->used < 4)
    {
      return 0;
    }
  }

  // - begin of data packet -
  if (msg->data[0] == '$')
  {
    unsigned pkt_size = ntohs(*(uint16_t *)(msg->data + 2)) + 4;

    if (msg->used < pkt_size)
    {
      unsigned pkt_msg_old_used = msg->used;
      if ((
#ifdef CLIB_WITH_OPENSSL
          this->ssl != NULL ? ssl_conn_s_read(&this->ssl,this->epoll_fd.fd,msg) :
#endif
          fd_s_read(&this->epoll_fd.fd,msg)) || msg->used == pkt_msg_old_used)
      {
        throw_error(RTSP_CLIENT_READ_ERROR);
      }

      if (msg->used < pkt_size)
      {
        return 0;
      }
    }

    // - adjust message size for callback -
    unsigned msg_used = msg->used;
    msg->used = pkt_size;

    // - call recv_packet_callback -
    if (((rtsp_recv_packet_callback_t)this->recv_packet_callback)(this->cb_object,this->cb_index,msg))
    {
      throw_error(RTSP_CLIENT_CALLBACK_ERROR);
    }

    // - reset message size -
    msg->used = msg_used;
    bc_array_s_tail(msg,msg->used - pkt_size);

    return 0;
  }

  // - read new data if available -
  if (msg_old_used == msg->used)
  {
    if ((
#ifdef CLIB_WITH_OPENSSL
        this->ssl != NULL ? ssl_conn_s_read(&this->ssl,this->epoll_fd.fd,msg) :
#endif
        fd_s_read(&this->epoll_fd.fd,msg)) || msg->used == msg_old_used)
    {
      throw_error(RTSP_CLIENT_READ_ERROR);
    }
  }

  // - parse command response -
  string_s string = {msg->used + 1,msg->data};

  // - parse check command response -
  if (rtsp_parser_s_parse(&this->parser,&string,0))
  {
    return 0;
  }

  debug_message_6(fprintf(stderr,"rtsp_client_s <<<<<\n%.*s\n",this->parser.input_idx,msg->data));

  // - parse process command response -
  if (rtsp_parser_s_parse(&this->parser,&string,1))
  {
    throw_error(RTSP_CONN_PARSE_ERROR);
  }

  switch (this->parser.command)
  {
    case c_rtsp_command_RESPONSE:
      break;
    case c_rtsp_command_SET_PARAMETER:
      {/*{{{*/

        // - if ping requested -
        if (this->parser.ping)
        {
          this->out_msg.used = 0;
          bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"\r\n",this->parser.cseq);

          if (rtsp_client_s_send_cmd(this))
          {
            this->state = c_rtsp_client_state_ERROR;
            throw_error(RTSP_CLIENT_SEND_ERROR);
          }
        }
      }/*}}}*/
      break;
    default:
      throw_error(RTSP_CLIENT_UNKNOWN_COMMAND);
  }

  bc_array_s_tail(msg,msg->used - this->parser.input_idx);

  return 0;
}/*}}}*/

int rtsp_client_s_recv_sdp(rtsp_client_s *this)
{/*{{{*/
  bc_array_s *msg = &this->in_msg;

  // - sdp data not received yet -
  if (msg->used == 0)
  {
    unsigned msg_old_used = msg->used;
    if ((
#ifdef CLIB_WITH_OPENSSL
        this->ssl != NULL ? ssl_conn_s_read(&this->ssl,this->epoll_fd.fd,msg) :
#endif
        fd_s_read(&this->epoll_fd.fd,msg)) || msg->used == msg_old_used)
    {
      throw_error(RTSP_CLIENT_READ_ERROR);
    }
  }

  debug_message_6(fprintf(stderr,"rtsp_client_s <<<<<\n%.*s\n",msg->used,msg->data));

  // - call recv_sdp_callback -
  if (((rtsp_recv_sdp_callback_t)this->recv_sdp_callback)(this->cb_object,this->cb_index,&this->server_num_ip,msg))
  {
    throw_error(RTSP_CLIENT_CALLBACK_ERROR);
  }

  // - parse sdp information -
  bc_array_s_push(msg,'\0');
  string_s string = {msg->used,msg->data};

  CONT_INIT(rtsp_sdp_parser_s,sdp_parser);
  if (rtsp_sdp_parser_s_parse(&sdp_parser,&string))
  {
    rtsp_sdp_parser_s_clear(&sdp_parser);
    throw_error(RTSP_CLIENT_PARSE_ERROR);
  }

  // - store video and audio controls -
  string_s_swap(&this->video_control,&sdp_parser.video_control);
  string_s_swap(&this->audio_control,&sdp_parser.audio_control);

  rtsp_sdp_parser_s_clear(&sdp_parser);

  msg->used = 0;

  return 0;
}/*}}}*/

int rtsp_client_s_fd_event(rtsp_client_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;
  (void)a_epoll;

  if (a_epoll_event->data.fd != this->epoll_fd.fd)
  {
    throw_error(RTSP_CLIENT_INVALID_FD);
  }

  switch (this->state)
  {
    case c_rtsp_client_state_CONNECTING:
      {/*{{{*/
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
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_CONNECT_ERROR);
        }

        // - retrieve server numeric ip address -
        {
          CONT_INIT_CLEAR(bc_array_s,buffer);

          socket_address_s addr;
          socklen_t addr_length = sizeof(addr);

          if (getpeername(this->epoll_fd.fd,&addr,&addr_length) ||
              socket_address_s_name(&addr,&buffer))
          {
            throw_error(RTSP_CLIENT_RETRIEVE_SERVER_IP_ERROR);
          }

          string_s_set(&this->server_num_ip,buffer.used,buffer.data);
        }

        // - reset sequence -
        this->sequence = 0;

        time_s time;
        if (clock_s_gettime(CLOCK_MONOTONIC,&time))
        {
          throw_error(RTSP_CLIENT_GET_TIME_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
      "GET %s HTTP/1.0\r\n"
"x-sessioncookie: %llu\r\n"
"Proxy-Connection: Keep-Alive\r\n"
"Pragma: no-cache\r\n"

      "\r\n",this->media_url.data,time);

        if (rtsp_client_s_send_cmd(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_SEND_ERROR);
        }

        this->state = c_rtsp_client_state_RECV_HTTP;
      }/*}}}*/
      break;

    case c_rtsp_client_state_RECV_HTTP:
      {/*{{{*/
        if (rtsp_client_s_recv_cmd_resp(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_RECEIVE_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
      "OPTIONS %s RTSP/1.0\r\n"
      "CSeq: %u\r\n"
      "\r\n",this->media_url.data,this->sequence++);

        if (rtsp_client_s_send_cmd(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_SEND_ERROR);
        }

        this->state = c_rtsp_client_state_RECV_OPTIONS;
      }/*}}}*/
      break;

    case c_rtsp_client_state_RECV_OPTIONS:
      {/*{{{*/
        if (rtsp_client_s_recv_cmd_resp(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_RECEIVE_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"DESCRIBE %s RTSP/1.0\r\n"
"CSeq: %u\r\n"
"\r\n",this->media_url.data,this->sequence++);

        if (rtsp_client_s_send_cmd(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_SEND_ERROR);
        }

        this->state = c_rtsp_client_state_RECV_DESCRIBE;
      }/*}}}*/
      break;

    case c_rtsp_client_state_RECV_DESCRIBE:
      {/*{{{*/
        if (rtsp_client_s_recv_cmd_resp(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_RECEIVE_ERROR);
        }

        this->state = c_rtsp_client_state_RECV_SDP;

        if (this->in_msg.used == 0)
        {
          break;
        }
      }/*}}}*/

    case c_rtsp_client_state_RECV_SDP:
      {/*{{{*/
        if (rtsp_client_s_recv_sdp(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_RECEIVE_ERROR);
        }

        // - no video control was retrieved from sdp -
        if (this->video_control.size <= 1)
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_RECEIVE_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"SETUP %s RTSP/1.0\r\n"
"CSeq: %u\r\n"
"Transport: RTP/AVP/TCP;unicast;interleaved=0-1\r\n"
"\r\n",this->video_control.data,this->sequence++);

        if (rtsp_client_s_send_cmd(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_SEND_ERROR);
        }

        this->state = c_rtsp_client_state_RECV_SETUP_VIDEO;
      }/*}}}*/
      break;

    case c_rtsp_client_state_RECV_SETUP_VIDEO:
      {/*{{{*/
        if (rtsp_client_s_recv_cmd_resp(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_RECEIVE_ERROR);
        }

        if (this->parser.session == NULL)
        {
          throw_error(RTSP_CLIENT_SESSION_ERROR);
        }

        // - set session -
        string_s_set(&this->session,this->parser.session_length,(char *)this->parser.session);

        // - no audio control was retrieved from sdp -
        if (this->audio_control.size <= 1)
        {
          this->out_msg.used = 0;
          bc_array_s_append_format(&this->out_msg,
"PLAY %s RTSP/1.0\r\n"
"CSeq: %u\r\n"
"Session: %s\r\n"
"\r\n",this->media_url.data,this->sequence++,this->session.data);

          if (rtsp_client_s_send_cmd(this))
          {
            this->state = c_rtsp_client_state_ERROR;
            throw_error(RTSP_CLIENT_SEND_ERROR);
          }

          this->state = c_rtsp_client_state_RECV_PLAY_OR_DATA;
        }
        else
        {
          this->out_msg.used = 0;
          bc_array_s_append_format(&this->out_msg,
"SETUP %s RTSP/1.0\r\n"
"CSeq: %u\r\n"
"Transport: RTP/AVP/TCP;unicast;interleaved=2-3\r\n"
"Session: %s\r\n"
"\r\n",this->audio_control.data,this->sequence++,this->session.data);

          if (rtsp_client_s_send_cmd(this))
          {
            this->state = c_rtsp_client_state_ERROR;
            throw_error(RTSP_CLIENT_SEND_ERROR);
          }

          this->state = c_rtsp_client_state_RECV_SETUP_AUDIO;
        }
      }/*}}}*/
      break;

    case c_rtsp_client_state_RECV_SETUP_AUDIO:
      {/*{{{*/
        if (rtsp_client_s_recv_cmd_resp(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_RECEIVE_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"PLAY %s RTSP/1.0\r\n"
"CSeq: %u\r\n"
"Session: %s\r\n"
"\r\n",this->media_url.data,this->sequence++,this->session.data);

        if (rtsp_client_s_send_cmd(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_SEND_ERROR);
        }

        this->state = c_rtsp_client_state_RECV_PLAY_OR_DATA;
      }/*}}}*/
      break;

    case c_rtsp_client_state_RECV_PLAY_OR_DATA:
      {/*{{{*/
        if (rtsp_client_s_recv_cmd_resp_or_data(this))
        {
          this->state = c_rtsp_client_state_ERROR;
          throw_error(RTSP_CLIENT_RECEIVE_ERROR);
        }
      }/*}}}*/
      break;

    default:
      throw_error(RTSP_CLIENT_INVALID_STATE);
  }

  return 0;
}/*}}}*/

