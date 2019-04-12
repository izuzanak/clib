
@begin
include "cl_rtsp.h"
@end

// === constants and definitions ===============================================

const char *c_week_day_names[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
const char *c_month_names[] =
  {"Jan","Feb","Mar","Apr","May","June","July","Aug","Sept","Oct","Nov","Dec"};

// === methods of generated structures =========================================

// -- rtsp_setup_s --
@begin
methods rtsp_setup_s
@end

// -- rtsp_setups_s --
@begin
methods rtsp_setups_s
@end

// -- rtsp_conn_s --
@begin
methods rtsp_conn_s
@end

int rtsp_conn_s_create(rtsp_conn_s *this,rtsp_server_s *a_server,
    unsigned a_index,epoll_fd_s *a_epoll_fd,socket_address_s *a_client_addr)
{/*{{{*/
  debug_message_3(fprintf(stderr,"rtsp_conn_s_create\n"));

  rtsp_conn_s_clear(this);

  this->server = a_server;
  this->client_addr = *a_client_addr;
  this->index = a_index;
  epoll_fd_s_swap(&this->epoll_fd,a_epoll_fd);

  // - set no delay on packet socket -
  int yes = 1;
  if (setsockopt(this->epoll_fd.fd,SOL_TCP,TCP_NODELAY,&yes,sizeof(yes)))
  {
    epoll_fd_s_clear(&this->epoll_fd);

    throw_error(RTSP_CONN_SETSOCKOPT_ERROR);
  }

  this->state = c_rtsp_conn_state_RECV_COMMAND;
  this->sequence = 0;
  this->session = 0;
  this->tcp = -1;

  return 0;
}/*}}}*/

void rtsp_conn_s_append_time(bc_array_s *a_trg)
{/*{{{*/
  time_s time;
  time_s_now(&time);

  datetime_s dt;
  time_s_datetime(&time,&dt);

  bc_array_s_append_format(a_trg,"%s, %s %2.2hhu %4.4hu %2.2hhu:%2.2hhu:%2.2hhu GMT",
    c_week_day_names[dt.wday - 1],c_month_names[dt.month - 1],dt.day,dt.year,dt.hour,dt.min,dt.sec);
}/*}}}*/

int rtsp_conn_s_send_resp(rtsp_conn_s *this,bc_array_s *a_msg)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_conn_s >>>>>\n%.*s",a_msg->used,a_msg->data));

  return fd_s_write(&this->epoll_fd.fd,a_msg->data,a_msg->used);
}/*}}}*/

int rtsp_conn_s_recv_cmd(rtsp_conn_s *this,epoll_s *a_epoll)
{/*{{{*/
  bc_array_s *msg = &this->in_msg;

  unsigned msg_old_used = msg->used;
  if (fd_s_read(&this->epoll_fd.fd,msg) || msg->used == msg_old_used)
  {
    throw_error(RTSP_CONN_READ_ERROR);
  }

  // - begin of data packet -
  if (msg->data[0] == '$')
  {
    unsigned pkt_size = ntohs(*(uint16_t *)(msg->data + 2)) + 4;

    if (msg->used < pkt_size)
    {
      unsigned msg_old_used = msg->used;
      if (fd_s_read(&this->epoll_fd.fd,msg) || msg->used == msg_old_used)
      {
        throw_error(RTSP_CLIENT_READ_ERROR);
      }

      if (msg->used < pkt_size)
      {
        return 0;
      }
    }

    // FIXME TODO process control packet
    debug_message_6(fprintf(stderr,"TCP: CONTROL PACKET\n"));

    bc_array_s_tail(msg,msg->used - pkt_size);

    return 0;
  }

  debug_message_6(fprintf(stderr,"rtsp_conn_s <<<<<\n%.*s",msg->used,msg->data));

  // - parse command response -
  string_s string = {msg->used + 1,msg->data};
  if (rtsp_parser_s_parse(&this->parser,&string))
  {
    return 0;
  }

  switch (this->parser.command)
  {
    case c_rtsp_command_OPTIONS:
      {/*{{{*/
        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",this->parser.cseq);
        rtsp_conn_s_append_time(&this->out_msg);
        bc_array_s_append_format(&this->out_msg,
"\r\n"
"Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, SET_PARAMETER, GET_PARAMETER\r\n"
"\r\n");

        if (rtsp_conn_s_send_resp(this,&this->out_msg))
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_SEND_ERROR);
        }
      }/*}}}*/
      break;

    case c_rtsp_command_DESCRIBE:
      {/*{{{*/
        rtsp_server_s *server = (rtsp_server_s *)this->server;

        // - call conn_get_sdp_callback -
        this->buffer.used = 0;
        if (((rtsp_conn_get_sdp_callback_t)server->conn_get_sdp_callback)(
              server->cb_object,this->index,this->parser.url_rtsp,&this->buffer) ||
              this->buffer.used == 0)
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",this->parser.cseq);
        rtsp_conn_s_append_time(&this->out_msg);
        bc_array_s_append_format(&this->out_msg,
"\r\n"
"Content-Base: %s/\r\n"
"Content-Type: application/sdp\r\n"
"Content-Length: %u\r\n"
"\r\n",(char *)this->parser.url_rtsp,this->buffer.used);

        if (rtsp_conn_s_send_resp(this,&this->out_msg) ||
            rtsp_conn_s_send_resp(this,&this->buffer))
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_SEND_ERROR);
        }
      }/*}}}*/
      break;

    case c_rtsp_command_SETUP:
      {/*{{{*/
        rtsp_server_s *server = (rtsp_server_s *)this->server;

        // - check unicast -
        // - call conn_check_media_callback -
        if (this->parser.unicast == 0 ||
            ((rtsp_conn_check_media_callback_t)server->conn_check_media_callback)(
              server->cb_object,this->index,this->parser.url_rtsp))
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        // - store setup -
        rtsp_setups_s_push_blank(&this->rtsp_setups);
        rtsp_setup_s *setup = rtsp_setups_s_last(&this->rtsp_setups);
        string_s_set_ptr(&setup->media_url,this->parser.url_rtsp);
        setup->inter_port_begin = this->parser.inter_port_begin;
        setup->inter_port_end = this->parser.inter_port_end;

        if (this->tcp != -1 && this->tcp != this->parser.tcp)
        {
          throw_error(RTSP_CONN_MISMATCH_RTSP_TRANSPORT);
        }

        this->tcp = this->parser.tcp;

        // - generate session number if needed -
        if (this->session == 0)
        {
          time_s time;
          time_s_now(&time);
          this->session = time_s_micro(&time);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",this->parser.cseq);
        rtsp_conn_s_append_time(&this->out_msg);

        if (this->parser.tcp)
        {
          bc_array_s_append_format(&this->out_msg,
"\r\n"
"Transport: RTP/AVP/TCP;unicast;interleaved=%u-%u\r\n"
"Session: %" HOST_LL_FORMAT "u;timeout=60\r\n"
"\r\n",
this->parser.inter_port_begin,
this->parser.inter_port_end,
this->session);
        }
        else
        {
          rtsp_server_s *server = (rtsp_server_s *)this->server;

          socket_address_s data_in_addr;
          socket_address_s ctrl_in_addr;

          if (socket_address_s_create(&data_in_addr,server->ip.data,0) ||
              socket_s_create(&setup->udp_data.fd,AF_INET,SOCK_DGRAM) ||
              socket_s_create(&setup->udp_ctrl.fd,AF_INET,SOCK_DGRAM) ||
              socket_s_bind(&setup->udp_data.fd,&data_in_addr) ||
              socket_s_bind(&setup->udp_ctrl.fd,&data_in_addr) ||
              socket_s_address(&setup->udp_data.fd,&data_in_addr) ||
              socket_s_address(&setup->udp_ctrl.fd,&ctrl_in_addr) ||
              epoll_s_fd_callback(a_epoll,&setup->udp_data,EPOLLIN | EPOLLPRI,rtsp_server_s_conn_fd_event,server,this->index) ||
              epoll_s_fd_callback(a_epoll,&setup->udp_ctrl,EPOLLIN | EPOLLPRI,rtsp_server_s_conn_fd_event,server,this->index))
          {
            throw_error(RTSP_CONN_UDP_SETUP_ERROR);
          }

          usi udp_data_in_port = socket_address_s_port(&data_in_addr);
          usi udp_ctrl_in_port = socket_address_s_port(&ctrl_in_addr);

          // - setup udp data address -
          setup->udp_data_addr = this->client_addr;
          setup->udp_data_addr.sin_port = htons(this->parser.inter_port_begin);

          // - setup udp control address -
          setup->udp_ctrl_addr = this->client_addr;
          setup->udp_ctrl_addr.sin_port = htons(this->parser.inter_port_end);

          bc_array_s_append_format(&this->out_msg,
"\r\n"
"Transport: RTP/AVP;unicast;client_port=%u-%u;server_port=%hu-%hu\r\n"
"Session: %" HOST_LL_FORMAT "u;timeout=60\r\n"
"\r\n",
this->parser.inter_port_begin,
this->parser.inter_port_end,
udp_data_in_port,
udp_ctrl_in_port,
this->session);
        }

        if (rtsp_conn_s_send_resp(this,&this->out_msg))
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_SEND_ERROR);
        }
      }/*}}}*/
      break;

    case c_rtsp_command_PLAY:
      {/*{{{*/
        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",this->parser.cseq);
        rtsp_conn_s_append_time(&this->out_msg);
        bc_array_s_append_format(&this->out_msg,
"\r\n"
"Range: npt=0.000-\r\n"
"Session: %" HOST_LL_FORMAT "u\r\n"
"\r\n",this->session);

        if (rtsp_conn_s_send_resp(this,&this->out_msg))
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_SEND_ERROR);
        }

        // - cancel send timer -
        epoll_timer_s_clear(&this->epoll_send_timer);

        // - reset packet sequences -
        this->packet_seq_0 = 0;
        this->packet_seq_2 = 0;

        // - initialize packet time -
        if (clock_s_gettime(CLOCK_MONOTONIC,&this->packet_time))
        {
          throw_error(RTSP_CONN_GET_TIME_ERROR);
        }

        // - burst first two seconds for rtsp-tcp -
        if (this->tcp)
        {
          this->packet_time -= 2000000000ULL;
        }

        // - prepare first packet -
        if (rtsp_conn_s_next_packet(this,a_epoll))
        {
          throw_error(RTSP_CONN_NEXT_PACKET_ERROR);
        }
      }/*}}}*/
      break;

    case c_rtsp_command_PAUSE:
      {/*{{{*/

        // - cancel send timer -
        epoll_timer_s_clear(&this->epoll_send_timer);

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",this->parser.cseq);
        rtsp_conn_s_append_time(&this->out_msg);
        bc_array_s_append_format(&this->out_msg,
"\r\n"
"Session: %" HOST_LL_FORMAT "u\r\n"
"\r\n",this->session);

        if (rtsp_conn_s_send_resp(this,&this->out_msg))
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_SEND_ERROR);
        }
      }/*}}}*/
      break;

    case c_rtsp_command_GET_PARAMETER:
      {/*{{{*/
        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",this->parser.cseq);
        rtsp_conn_s_append_time(&this->out_msg);
        bc_array_s_append_format(&this->out_msg,
"\r\n"
"Session: %" HOST_LL_FORMAT "u\r\n"
"\r\n",this->session);

        if (rtsp_conn_s_send_resp(this,&this->out_msg))
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_SEND_ERROR);
        }
      }/*}}}*/
      break;

    default:
      throw_error(RTSP_CONN_UNKNOWN_COMMAND);
  }

  bc_array_s_tail(msg,msg->used - this->parser.input_idx);

  return 0;
}/*}}}*/

int rtsp_conn_s_next_packet(rtsp_conn_s *this,epoll_s *a_epoll)
{/*{{{*/
  rtsp_server_s *server = (rtsp_server_s *)this->server;

  ulli delay;
  do
  {
    this->packet.used = 0;

    // - call conn_get_packet_callback -
    if (((rtsp_conn_get_packet_callback_t)server->conn_get_packet_callback)(
          server->cb_object,this->index,&delay,&this->packet))
    {
      throw_error(RTSP_CONN_CALLBACK_ERROR);
    }

    this->pkt_channel = RTP_PKT_GET_CHANNEL(this->packet.data);

    // - adjust packet sequence -
    switch (this->pkt_channel)
    {
    case 0:
      RTP_PKT_SET_SEQUENCE(this->packet.data,this->packet_seq_0++);
      break;
    case 2:
      RTP_PKT_SET_SEQUENCE(this->packet.data,this->packet_seq_2++);
      break;
    }

    // - if delay is not zero -
    if (delay != 0)
    {
      break;
    }

    if (rtsp_conn_s_send_packet(this))
    {
      throw_error(RTSP_CONN_SEND_PACKET_ERROR);
    }
  } while(1);

  // - schedule packet send timer -
  this->packet_time += delay*1000000ULL;
  epoll_s_timer_stamp(a_epoll,this->packet_time,rtsp_server_s_conn_time_event,this->server,this->index,&this->epoll_send_timer);

  return 0;
}/*}}}*/

int rtsp_conn_s_send_packet(rtsp_conn_s *this)
{/*{{{*/
  debug_message_7(fprintf(stderr,"rtsp_conn_s_send_packet\n"););

  rtsp_setup_s *setup;

  switch (this->pkt_channel)
  {
  case 0:
    setup = this->rtsp_setups.data + 0;
    break;
  case 2:
    setup = this->rtsp_setups.data + 1;
    break;
  default:
    return 0;
  }

  if (this->tcp)
  {
    return fd_s_write(&this->epoll_fd.fd,
        this->packet.data + sizeof(rtsp_pkt_delay_t),this->packet.used - sizeof(rtsp_pkt_delay_t));
  }

  return socket_s_sendto(&setup->udp_data.fd,&setup->udp_data_addr,
      this->packet.data + sizeof(rtsp_pkt_delay_t) + 4,this->packet.used - (sizeof(rtsp_pkt_delay_t) + 4));
}/*}}}*/

int rtsp_conn_s_time_event(rtsp_conn_s *this,unsigned a_index,unsigned a_timer,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;
  (void)a_timer;

  // - drop one shot timer -
  this->epoll_send_timer.timer = c_idx_not_exist;

  if (rtsp_conn_s_send_packet(this))
  {
    throw_error(RTSP_CONN_SEND_PACKET_ERROR);
  }

  if (rtsp_conn_s_next_packet(this,a_epoll))
  {
    throw_error(RTSP_CONN_NEXT_PACKET_ERROR);
  }

  return 0;
}/*}}}*/

int rtsp_conn_s_fd_event(rtsp_conn_s *this,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;
  (void)a_epoll;

  if (a_epoll_event->data.fd == this->epoll_fd.fd)
  {
    switch (this->state)
    {
      case c_rtsp_conn_state_RECV_COMMAND:
        {/*{{{*/
          if (rtsp_conn_s_recv_cmd(this,a_epoll))
          {
            this->state = c_rtsp_conn_state_ERROR;
            throw_error(RTSP_CONN_RECEIVE_ERROR);
          }
        }/*}}}*/
        break;

      default:
        throw_error(RTSP_CONN_INVALID_STATE);
    }
  }
  else
  {
    socket_address_s address;

    int udp_fd = a_epoll_event->data.fd;

    this->buffer.used = 0;
    if (socket_s_recvfrom(&udp_fd,&this->buffer,&address))
    {
      throw_error(RTSP_CONN_RECEIVE_ERROR);
    }

    // FIXME TODO process control packet
    debug_message_6(fprintf(stderr,"UDP: CONTROL PACKET\n"));
  }

  return 0;
}/*}}}*/

// -- rtsp_conn_list_s --
@begin
methods rtsp_conn_list_s
@end

