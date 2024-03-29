
@begin
include "cl_rtsp.h"
@end

// === constants and definitions ===============================================

const char *c_week_day_names[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
const char *c_month_names[] =
  {"Jan","Feb","Mar","Apr","May","June","July","Aug","Sept","Oct","Nov","Dec"};

ulli rtsp_powers_of_10[] =
{/*{{{*/
  1,
  10,
  100,
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000,
  1000000000,
};/*}}}*/

// === methods of generated structures =========================================

// -- rtsp_setup_s --
@begin
methods rtsp_setup_s
@end

// -- rtsp_setups_s --
@begin
methods rtsp_setups_s
@end

// -- rtsp_play_options_s --
@begin
methods rtsp_play_options_s
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

  // - set size of output queue -
  int buff_size = RTSP_TCP_OUTPUT_QUEUE_SIZE;
  socklen_t length = sizeof(buff_size);
  if (setsockopt(this->epoll_fd.fd,SOL_SOCKET,SO_SNDBUF,&buff_size,sizeof(buff_size)) ||
      getsockopt(this->epoll_fd.fd,SOL_SOCKET,SO_SNDBUF,&buff_size,&length) ||
      buff_size < RTSP_TCP_OUTPUT_QUEUE_SIZE)
  {
    throw_error(RTSP_CONN_SETSOCKOPT_ERROR);
  }

  this->state = c_rtsp_conn_state_RECV_COMMAND;
  this->sequence = 0;
  this->session = 0;
  this->x_session_map_idx = c_idx_not_exist;
  this->tcp = -1;
  this->auth = 0;

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

int rtsp_conn_s_parse_range_time(char **a_ptr,time_s *a_time)
{/*{{{*/
  char *ptr = *a_ptr;

  char datetime[15];
  char *dt_ptr = datetime;
  char *dt_ptr_end = dt_ptr + sizeof(datetime);
  do
  {
    if (isdigit((int)*ptr)) { *dt_ptr++ = *ptr; }
    else if (*ptr != 'T') { break; }
  } while(++ptr,dt_ptr < dt_ptr_end);

  if (dt_ptr - datetime != 14)
  {
    throw_error(RTSP_CONN_INVALID_RANGE_TIME);
  }

  // - retrieve milliseconds -
  ulli subsec = 0;
  if (*ptr == '.')
  {
    // - skip dot -
    ++ptr;

    char *ptr_end;
    subsec = strtoll(ptr,&ptr_end,10);

    unsigned subsec_len = ptr_end - ptr;
    if (subsec_len < 1 || subsec_len > 9)
    {
      throw_error(RTSP_CONN_INVALID_RANGE_TIME);
    }

    subsec *= rtsp_powers_of_10[9 - subsec_len];
    ptr = ptr_end;
  }

  if (time_s_from_string(a_time,14,datetime))
  {
    throw_error(RTSP_CONN_INVALID_RANGE_TIME);
  }

  *a_time += subsec;
  *a_ptr = ptr;

  return 0;
}/*}}}*/

int rtsp_conn_s_send_resp(rtsp_conn_s *this,bc_array_s *a_msg)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_conn_s >>>>>\n%.*s",a_msg->used,a_msg->data));

  return
#ifdef CLIB_WITH_OPENSSL
    this->ssl != NULL ? ssl_conn_s_write(&this->ssl,a_msg->data,a_msg->used) :
#endif
    fd_s_write(&this->epoll_fd.fd,a_msg->data,a_msg->used);
}/*}}}*/

int rtsp_conn_s_parse_and_process_command(rtsp_conn_s *this,rtsp_parser_s *a_parser,string_s *a_source)
{/*{{{*/
  rtsp_server_s *server = (rtsp_server_s *)this->server;

  debug_message_6(fprintf(stderr,"rtsp_conn_s <<<<<\n%.*s\n",a_source->size - 1,a_source->data));

  // - parse process command -
  if (rtsp_parser_s_parse(a_parser,a_source,1))
  {
    throw_error(RTSP_CONN_PARSE_ERROR);
  }

  switch (a_parser->command)
  {
    case c_rtsp_command_GET:
      {/*{{{*/

        // - process x session cookie -
        if (a_parser->x_session != NULL)
        {
          // - store x session cookie -
          string_s_set(&this->x_session,a_parser->x_session_length,a_parser->x_session);

          x_session_map_s x_session_map = {this->x_session,this->index};
          unsigned x_session_map_idx = x_session_map_tree_s_get_idx(&server->x_session_map_tree,&x_session_map);
          if (x_session_map_idx != c_idx_not_exist)
          {
            this->state = c_rtsp_conn_state_ERROR;
            throw_error(RTSP_CONN_DUPLICATE_X_SESSION_COOKIE);
          }

          this->x_session_map_idx =
            x_session_map_tree_s_insert(&server->x_session_map_tree,&x_session_map);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"HTTP/1.1 200 OK\r\n"
"Connection: close\r\n"
"Cache-Control: no-store\r\n"
"Pragma: no-cache\r\n"
"Content-Type: application/x-rtsp-tunnelled\r\n"
"Date: "
        );
        rtsp_conn_s_append_time(&this->out_msg);
        bc_array_s_append_format(&this->out_msg,
"\r\n"
"\r\n");

        if (rtsp_conn_s_send_resp(this,&this->out_msg))
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_SEND_ERROR);
        }
      }/*}}}*/
      break;

    case c_rtsp_command_POST:
      {/*{{{*/

        // - check x session cookie -
        if (a_parser->x_session == NULL)
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_MISSING_X_SESSION_COOKIE);
        }

        // - store x session cookie -
        string_s_set(&this->x_session,a_parser->x_session_length,a_parser->x_session);

        // - update connection state -
        this->state = c_rtsp_conn_state_RECV_HTTP_COMMAND;
      }/*}}}*/
      break;

    case c_rtsp_command_OPTIONS:
      {/*{{{*/
        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",a_parser->cseq);
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

        // - call conn_get_sdp_callback -
        this->buffer.used = 0;

        int get_sdp_result = 0;
        if ((get_sdp_result = ((rtsp_conn_get_sdp_callback_t)server->conn_get_sdp_callback)(
              server->cb_object,this->index,a_parser,&this->buffer)) ||
              this->buffer.used == 0)
        {
          if (get_sdp_result != ERROR_RTSP_CONN_AUTHENTICATE_ERROR)
          {
            throw_error(RTSP_CONN_CALLBACK_ERROR);
          }
          else
          {
            this->out_msg.used = 0;
            bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 401 Unauthorized\r\n"
"WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\"\r\n"
"CSeq: %u\r\n"
"\r\n",a_parser->digest_auth.realm.data,a_parser->digest_auth.nonce.data,a_parser->cseq);

            if (rtsp_conn_s_send_resp(this,&this->out_msg))
            {
              this->state = c_rtsp_conn_state_ERROR;
              throw_error(RTSP_CONN_SEND_ERROR);
            }
          }
        }
        else
        {
          // - authentication was successfull -
          this->auth = 1;

          this->out_msg.used = 0;
          bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",a_parser->cseq);
          rtsp_conn_s_append_time(&this->out_msg);
          bc_array_s_append_format(&this->out_msg,
"\r\n"
"Content-Base: %s/\r\n"
"Content-Type: application/sdp\r\n"
"Content-Length: %u\r\n"
"\r\n",(char *)a_parser->rtsp_url,this->buffer.used);

          if (rtsp_conn_s_send_resp(this,&this->out_msg) ||
              rtsp_conn_s_send_resp(this,&this->buffer))
          {
            this->state = c_rtsp_conn_state_ERROR;
            throw_error(RTSP_CONN_SEND_ERROR);
          }
        }
      }/*}}}*/
      break;

    case c_rtsp_command_SETUP:
      {/*{{{*/

        // - check authentication -
        if (!this->auth)
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        // - check datacast -
        // - call conn_check_media_callback -
        unsigned channel = c_idx_not_exist;
        if (a_parser->datacast == c_cast_type_unknown ||
            (a_parser->tcp && a_parser->datacast != c_cast_type_unicast) ||
            ((rtsp_conn_check_media_callback_t)server->conn_check_media_callback)(
              server->cb_object,this->index,a_parser->rtsp_url,&channel) ||
              channel == c_idx_not_exist)
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        // - update channel to setup map -
        while (this->setup_map.used < channel)
        {
          ui_array_s_push(&this->setup_map,c_idx_not_exist);
        }

        ui_array_s_push(&this->setup_map,this->setups.used);

        // - store setup -
        rtsp_setups_s_push_blank(&this->setups);
        rtsp_setup_s *setup = rtsp_setups_s_last(&this->setups);
        string_s_set_ptr(&setup->media_url,a_parser->rtsp_url);
        setup->inter_port_begin = a_parser->inter_port_begin;
        setup->inter_port_end = a_parser->inter_port_end;

        if (this->tcp != -1 && this->tcp != a_parser->tcp)
        {
          throw_error(RTSP_CONN_MISMATCH_RTSP_TRANSPORT);
        }

        this->tcp = a_parser->tcp;

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
"Date: ",a_parser->cseq);
        rtsp_conn_s_append_time(&this->out_msg);

        if (a_parser->tcp)
        {
          bc_array_s_append_format(&this->out_msg,
"\r\n"
"Transport: RTP/AVP/TCP;unicast;interleaved=%u-%u\r\n"
"Session: %" HOST_LL_FORMAT "u;timeout=60\r\n"
"\r\n",
a_parser->inter_port_begin,
a_parser->inter_port_end,
this->session);
        }
        else
        {
          socket_address_s data_in_addr = {0};
          socket_address_s ctrl_in_addr = {0};

          if (socket_address_s_create(&data_in_addr,server->ip.data,0) ||
              socket_s_create(&setup->udp_data.fd,AF_INET,SOCK_DGRAM) ||
              socket_s_create(&setup->udp_ctrl.fd,AF_INET,SOCK_DGRAM) ||
              socket_s_bind(&setup->udp_data.fd,&data_in_addr) ||
              socket_s_bind(&setup->udp_ctrl.fd,&data_in_addr) ||
              socket_s_address(&setup->udp_data.fd,&data_in_addr) ||
              socket_s_address(&setup->udp_ctrl.fd,&ctrl_in_addr) ||
              epoll_s_fd_callback(&setup->udp_data,EPOLLIN | EPOLLPRI,rtsp_server_s_conn_fd_event,server,this->index) ||
              epoll_s_fd_callback(&setup->udp_ctrl,EPOLLIN | EPOLLPRI,rtsp_server_s_conn_fd_event,server,this->index))
          {
            throw_error(RTSP_CONN_UDP_SETUP_ERROR);
          }

          bc_array_s_append_ptr(&this->out_msg,
"\r\n");

          switch (a_parser->datacast)
          {
          case c_cast_type_unicast:
            {/*{{{*/
              usi udp_data_in_port = socket_address_s_port(&data_in_addr);
              usi udp_ctrl_in_port = socket_address_s_port(&ctrl_in_addr);

              // - setup udp data address -
              setup->udp_data_addr = this->client_addr;
              setup->udp_data_addr.sin_port = htons(a_parser->inter_port_begin);

              // - setup udp control address -
              setup->udp_ctrl_addr = this->client_addr;
              setup->udp_ctrl_addr.sin_port = htons(a_parser->inter_port_end);

              bc_array_s_append_format(&this->out_msg,
"Transport: RTP/AVP;unicast;client_port=%u-%u;server_port=%hu-%hu\r\n",
a_parser->inter_port_begin,
a_parser->inter_port_end,
udp_data_in_port,
udp_ctrl_in_port);
            }/*}}}*/
            break;
          case c_cast_type_multicast:
            {/*{{{*/
              const char *multicast_ip = "239.0.0.1";

              // - setup udp data/control addresses -
              if (socket_address_s_create(&setup->udp_data_addr,multicast_ip,a_parser->inter_port_begin) ||
                  socket_address_s_create(&setup->udp_ctrl_addr,multicast_ip,a_parser->inter_port_end))
              {
                throw_error(RTSP_CONN_UDP_SETUP_ERROR);
              }

              bc_array_s_append_format(&this->out_msg,
"Transport: RTP/AVP;multicast;destination=%s;port=%hu-%hu\r\n",
multicast_ip,
a_parser->inter_port_begin,
a_parser->inter_port_end);
            }/*}}}*/
            break;
          }

          bc_array_s_append_format(&this->out_msg,
"Session: %" HOST_LL_FORMAT "u;timeout=60\r\n"
"\r\n",
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

        // - check authentication -
        if (!this->auth)
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",a_parser->cseq);
        rtsp_conn_s_append_time(&this->out_msg);
        bc_array_s_append_format(&this->out_msg,
"\r\n"
"%s\r\n"
"Session: %" HOST_LL_FORMAT "u\r\n"
"\r\n",a_parser->range.size > 1 ? a_parser->range.data : "Range: npt=0.000-",this->session);

        if (rtsp_conn_s_send_resp(this,&this->out_msg))
        {
          this->state = c_rtsp_conn_state_ERROR;
          throw_error(RTSP_CONN_SEND_ERROR);
        }

        // - cancel send timer -
        epoll_timer_s_clear(&this->epoll_send_timer);

        // - reset setups packet sequences -
        if (this->setups.used != 0)
        {
          rtsp_setup_s *s_ptr = this->setups.data;
          rtsp_setup_s *s_ptr_end = s_ptr + this->setups.used;
          do {
            rtsp_setup_s_reset_sequences(s_ptr);
          } while(++s_ptr < s_ptr_end);
        }

        // - initialize packet time -
        if (clock_s_gettime(CLOCK_MONOTONIC,&this->packet_time))
        {
          throw_error(RTSP_CONN_GET_TIME_ERROR);
        }

        // - prepare play options -
        rtsp_play_options_s play_options = {.session=this->session,.cseq=a_parser->cseq,.flags=0};

        // - set onvif replay flag -
        if (a_parser->onvif_replay)
        {
          play_options.flags |= c_rtsp_flag_ONVIF_REPLAY;
        }

        if (a_parser->range.size > 1)
        {
          unsigned npt_idx = string_s_get_idx(&a_parser->range,0,4,"npt=");
          if (npt_idx != c_idx_not_exist)
          {
            char *ptr = a_parser->range.data + npt_idx + 4;
            double offset = strtod(ptr,NULL);
            play_options.offset_time = ((lli)offset*1000.0)*1000000ULL;
          }

          unsigned clock_idx = string_s_get_idx(&a_parser->range,0,6,"clock=");
          if (clock_idx != c_idx_not_exist)
          {
            char *ptr = a_parser->range.data + clock_idx + 6;

            // - retrieve start time -
            if (rtsp_conn_s_parse_range_time(&ptr,&play_options.start_time))
            {
              throw_error(RTSP_CONN_INVALID_RANGE_TIME);
            }

            ptr = strchr(ptr,'-');
            if (ptr != NULL && *++ptr != '\0')
            {
              // - retrieve end time -
              if (rtsp_conn_s_parse_range_time(&ptr,&play_options.end_time))
              {
                throw_error(RTSP_CONN_INVALID_RANGE_TIME);
              }
            }
          }
        }

        // - call conn_playing_callback -
        if (((rtsp_conn_playing_callback_t)server->conn_playing_callback)(
              server->cb_object,this->index,&play_options))
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        // - prepare first packet -
        if (rtsp_conn_s_next_packet(this))
        {
          throw_error(RTSP_CONN_NEXT_PACKET_ERROR);
        }
      }/*}}}*/
      break;

    case c_rtsp_command_PAUSE:
      {/*{{{*/

        // - check authentication -
        if (!this->auth)
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        // - cancel send timer -
        epoll_timer_s_clear(&this->epoll_send_timer);

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",a_parser->cseq);
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

    case c_rtsp_command_SET_PARAMETER:
    case c_rtsp_command_GET_PARAMETER:
      {/*{{{*/

        // - check authentication -
        if (!this->auth)
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"Date: ",a_parser->cseq);
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

    case c_rtsp_command_TEARDOWN:
      {/*{{{*/

        // - check authentication -
        if (!this->auth)
        {
          throw_error(RTSP_CONN_CALLBACK_ERROR);
        }

        this->out_msg.used = 0;
        bc_array_s_append_format(&this->out_msg,
"RTSP/1.0 200 OK\r\n"
"CSeq: %u\r\n"
"\r\n",a_parser->cseq);

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

  return 0;
}/*}}}*/

int rtsp_conn_s_recv_cmd(rtsp_conn_s *this)
{/*{{{*/
  bc_array_s *msg = &this->in_msg;

  unsigned msg_old_used = msg->used;
  if ((
#ifdef CLIB_WITH_OPENSSL
      this->ssl != NULL ? ssl_conn_s_read(&this->ssl,this->epoll_fd.fd,msg) :
#endif
      fd_s_read(&this->epoll_fd.fd,msg)) || msg->used == msg_old_used)
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
      if ((
#ifdef CLIB_WITH_OPENSSL
          this->ssl != NULL ? ssl_conn_s_read(&this->ssl,this->epoll_fd.fd,msg) :
#endif
          fd_s_read(&this->epoll_fd.fd,msg)) || msg->used == msg_old_used)
      {
        throw_error(RTSP_CLIENT_READ_ERROR);
      }

      if (msg->used < pkt_size)
      {
        return 0;
      }
    }

    debug_message_6(fprintf(stderr,"TCP: CONTROL PACKET\n"));

    bc_array_s_tail(msg,msg->used - pkt_size);
  }

  // - parse command -
  string_s source = {msg->used + 1,msg->data};

  // - parse check command -
  if (source.size <= 1 || rtsp_parser_s_parse(&this->parser,&source,0))
  {
    return 0;
  }

  // - parse and process command -
  if (rtsp_conn_s_parse_and_process_command(this,&this->parser,&source))
  {
    throw_error(RTSP_CONN_PARSE_AND_PROCESS_ERROR);
  }

  bc_array_s_tail(msg,msg->used - this->parser.input_idx);

  // - waiting on http commands, and data are already available -
  if (this->state == c_rtsp_conn_state_RECV_HTTP_COMMAND &&
      msg->used != 0)
  {
    if (rtsp_conn_s_recv_http_cmd(this,0))
    {
      throw_error(RTSP_CONN_RECEIVE_ERROR);
    }
  }

  return 0;
}/*}}}*/

int rtsp_conn_s_recv_http_cmd(rtsp_conn_s *this,int a_read)
{/*{{{*/
  rtsp_server_s *server = (rtsp_server_s *)this->server;

  if (a_read)
  {
    unsigned msg_old_used = this->in_msg.used;
    if ((
#ifdef CLIB_WITH_OPENSSL
        this->ssl != NULL ? ssl_conn_s_read(&this->ssl,this->epoll_fd.fd,&this->in_msg) :
#endif
        fd_s_read(&this->epoll_fd.fd,&this->in_msg)) || this->in_msg.used == msg_old_used)
    {
      throw_error(RTSP_CONN_READ_ERROR);
    }
  }

  // - decode data -
  if (crypto_decode_base64(this->in_msg.data,this->in_msg.used,&this->out_msg))
  {
    throw_error(RTSP_CONN_BASE64_DECODE_ERROR);
  }

  // - reset input message size -
  this->in_msg.used = 0;

  bc_array_s *msg = &this->out_msg;

  // - begin of data packet -
  if (msg->data[0] == '$')
  {
    unsigned pkt_size = ntohs(*(uint16_t *)(msg->data + 2)) + 4;

    debug_message_6(fprintf(stderr,"TCP: CONTROL PACKET\n"));

    bc_array_s_tail(msg,msg->used - pkt_size);
  }

  // - parse command -
  string_s source = {msg->used + 1,msg->data};

  // - parse check command -
  if (source.size <= 1 || rtsp_parser_s_parse(&this->parser,&source,0))
  {
    return 0;
  }

  // - retrieve pair connection -
  x_session_map_s search_x_session_map = {.x_session=this->x_session};
  unsigned x_session_map_idx = x_session_map_tree_s_get_idx(&server->x_session_map_tree,&search_x_session_map);
  if (x_session_map_idx == c_idx_not_exist)
  {
    throw_error(RTSP_CONN_INVALID_X_SESSION);
  }

  x_session_map_s *x_session_map = x_session_map_tree_s_at(&server->x_session_map_tree,x_session_map_idx);

  // - parse and process command -
  if (rtsp_conn_s_parse_and_process_command(
        rtsp_conn_list_s_at(&server->conn_list,x_session_map->conn_idx),&this->parser,&source))
  {
    throw_error(RTSP_CONN_PARSE_AND_PROCESS_ERROR);
  }

  bc_array_s_tail(msg,msg->used - this->parser.input_idx);

  return 0;
}/*}}}*/

int rtsp_conn_s_next_packet(rtsp_conn_s *this)
{/*{{{*/
  rtsp_server_s *server = (rtsp_server_s *)this->server;

  do {

    // - call conn_get_packet_callback -
    if (((rtsp_conn_get_packet_callback_t)server->conn_get_packet_callback)(
          server->cb_object,this->index,&this->pkt_delay,&this->packet))
    {
      // - write packet buffer -
      if (rtsp_conn_s_write_pkt_buffer(this))
      {
        throw_error(RTSP_CONN_NEXT_PACKET_ERROR);
      }

      throw_error(RTSP_CONN_CALLBACK_ERROR);
    }

    this->pkt_channel = RTP_PKT_GET_CHANNEL(this->packet.data);

    // - retrieve channel  -
    switch (this->pkt_channel)
    {
    case 0:
    case 2:
    case 4:
      break;
    default:
      throw_error(RTSP_CONN_INVALID_PACKET_CHANNEL);
    }

    // - packet send flag -
    int packet_send = 1;

    if (this->pkt_delay == 0)
    {
      if (rtsp_conn_s_send_packet(this,&packet_send))
      {
        throw_error(RTSP_CONN_SEND_PACKET_ERROR);
      }
    }
    else
    {
      // - write packet buffer -
      if (rtsp_conn_s_write_pkt_buffer(this))
      {
        throw_error(RTSP_CONN_NEXT_PACKET_ERROR);
      }

      // - schedule packet send timer -
      this->packet_time += RTSP_DELAY_TO_NANOSEC(this->pkt_delay);

      // - reset packet send flag -
      packet_send = 0;
    }

    if (!packet_send)
    {
      struct itimerspec itimerspec = {{0,0},{this->packet_time/1000000000,this->packet_time%1000000000}};
      if (epoll_s_timer_callback(&this->epoll_send_timer,&itimerspec,TFD_TIMER_ABSTIME,rtsp_server_s_conn_time_event,this->server,this->index))
      {
        throw_error(RTSP_CONN_TIMER_CREATE_ERROR);
      }

      break;
    }

  } while(1);

  return 0;
}/*}}}*/

int rtsp_conn_s_send_packet(rtsp_conn_s *this,int *a_packet_send)
{/*{{{*/
  debug_message_7(fprintf(stderr,"rtsp_conn_s_send_packet\n"););

  rtsp_setup_s *rtsp_setup = rtsp_setups_s_at(&this->setups,
    *ui_array_s_at(&this->setup_map,this->pkt_channel));

#define RTSP_CONN_S_SEND_PACKET_MODIFY_PACKET() \
/*{{{*/\
  unsigned old_time_stamp = RTP_PKT_GET_TIME_STAMP(this->packet.data);\
  unsigned time_stamp = old_time_stamp + rtsp_setup->time_stamp_offset;\
\
  unsigned fdiff = time_stamp - rtsp_setup->last_time_stamp;\
  unsigned sdiff = rtsp_setup->last_time_stamp - time_stamp;\
\
  if (fdiff > 100000 && sdiff > 100000)\
  {\
    rtsp_setup->time_stamp_offset = rtsp_setup->last_time_stamp - old_time_stamp;\
    time_stamp = rtsp_setup->last_time_stamp;\
  }\
\
  /* - update last time stamp - */\
  rtsp_setup->last_time_stamp = time_stamp;\
\
  /* - modify packet data - */\
  RTP_PKT_SET_CHANNEL(this->packet.data,rtsp_setup->inter_port_begin);\
  RTP_PKT_SET_SEQUENCE(this->packet.data,rtsp_setup->packet_sequence++);\
  RTP_PKT_SET_TIME_STAMP(this->packet.data,time_stamp);\
/*}}}*/

  if (this->tcp)
  {
    // - not enought space in output queue -
    if (rtsp_setup->tcp_outq_space <= RTSP_TCP_OUTPUT_WRITE_LIMIT)
    {
      // - update to real queue values -
      if (rtsp_setup_s_update_tcp_outq(rtsp_setup,this->epoll_fd.fd))
      {
        throw_error(RTSP_CONN_UPDATE_TCP_QUEUE_STATE_ERROR);
      }

      // - update space by size of packet buffer -
      rtsp_setup->tcp_outq_space -= this->pkt_buffer.used;

      // - still not enought space in output queue -
      if (rtsp_setup->tcp_outq_space <= RTSP_TCP_OUTPUT_WRITE_LIMIT)
      {
        // - write packet buffer -
        if (rtsp_conn_s_write_pkt_buffer(this))
        {
          throw_error(RTSP_CONN_SEND_PACKET_ERROR);
        }

        // - modify fd epoll events: input and output -
        if (epoll_fd_s_modify_events(&this->epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI))
        {
          throw_error(RTSP_CONN_EPOLL_ERROR);
        }

        // - reset packet send flag -
        *a_packet_send = 0;

        return 0;
      }
    }

    debug_assert(this->packet.size <= RTSP_TCP_OUTPUT_WRITE_LIMIT);

    // - update output queue counter -
    rtsp_setup->tcp_outq_space -= this->packet.size;

    // - modify packet sequence and time stamp -
    RTSP_CONN_S_SEND_PACKET_MODIFY_PACKET();

    int result;
    if (this->pkt_delay == 0)
    {
      bc_array_s_append(&this->pkt_buffer,this->packet.size,this->packet.data);

      result = 0;
    }
    else
    {
      result =
#ifdef CLIB_WITH_OPENSSL
        this->ssl != NULL ? ssl_conn_s_write(&this->ssl,this->packet.data,this->packet.size) :
#endif
        fd_s_write(&this->epoll_fd.fd,this->packet.data,this->packet.size);
    }

    // - reset packet channel and time stamp -
    RTP_PKT_SET_CHANNEL(this->packet.data,this->pkt_channel);
    RTP_PKT_SET_TIME_STAMP(this->packet.data,old_time_stamp);

    return result;
  }

  // - modify packet sequence and time stamp -
  RTSP_CONN_S_SEND_PACKET_MODIFY_PACKET();

  int result = socket_s_sendto(&rtsp_setup->udp_data.fd,&rtsp_setup->udp_data_addr,
      this->packet.data + 4,this->packet.size - (4));

  // - reset packet channel and time stamp -
  RTP_PKT_SET_CHANNEL(this->packet.data,this->pkt_channel);
  RTP_PKT_SET_TIME_STAMP(this->packet.data,old_time_stamp);

  return result;
}/*}}}*/

int rtsp_conn_s_process_packet(rtsp_conn_s *this)
{/*{{{*/

  // - packet send flag -
  int packet_send = 1;

  if (rtsp_conn_s_send_packet(this,&packet_send))
  {
    throw_error(RTSP_CONN_SEND_PACKET_ERROR);
  }

  // - packet was send -
  if (packet_send)
  {
    // - retrieve next packet -
    if (rtsp_conn_s_next_packet(this))
    {
      throw_error(RTSP_CONN_NEXT_PACKET_ERROR);
    }
  }

  return 0;
}/*}}}*/

int rtsp_conn_s_fd_event(rtsp_conn_s *this,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  if (a_epoll_event->data.fd == this->epoll_fd.fd)
  {
    // - process input data -
    if (a_epoll_event->events & EPOLLIN)
    {
      switch (this->state)
      {
        case c_rtsp_conn_state_RECV_COMMAND:
          {/*{{{*/
            if (rtsp_conn_s_recv_cmd(this))
            {
              this->state = c_rtsp_conn_state_ERROR;
              throw_error(RTSP_CONN_RECEIVE_ERROR);
            }
          }/*}}}*/
          break;
        case c_rtsp_conn_state_RECV_HTTP_COMMAND:
          {/*{{{*/
            if (rtsp_conn_s_recv_http_cmd(this,1))
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

    // - write previously blocked packets -
    if (a_epoll_event->events & EPOLLOUT)
    {
      // - modify fd epoll events: only input -
      if (epoll_fd_s_modify_events(&this->epoll_fd,EPOLLIN | EPOLLPRI))
      {
        throw_error(RTSP_CONN_EPOLL_ERROR);
      }

      if (rtsp_conn_s_process_packet(this))
      {
        throw_error(RTSP_CONN_PROCESS_PACKET_ERROR);
      }
    }
  }
  else
  {
    socket_address_s address;

    int udp_fd = a_epoll_event->data.fd;
    if (socket_s_recvfrom(&udp_fd,&this->buffer,&address))
    {
      throw_error(RTSP_CONN_RECEIVE_ERROR);
    }

    debug_message_6(fprintf(stderr,"UDP: CONTROL PACKET\n"));
  }

  return 0;
}/*}}}*/

// -- rtsp_conn_list_s --
@begin
methods rtsp_conn_list_s
@end

