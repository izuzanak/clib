
@begin
include "cl_websocket.h"
@end

static const struct lws_extension ws_default_extensions[] =
{/*{{{*/
  { "permessage-deflate",lws_extension_callback_pm_deflate,"permessage-deflate" },
  { "deflate-frame",lws_extension_callback_pm_deflate,"deflate_frame" },
  { NULL,NULL,NULL }
};/*}}}*/

// === methods of structure ws_context_s =======================================

void ws_context_s_log_emit(int level,const char *line)
{/*{{{*/
  (void)level;
  (void)line;

  //fprintf(stderr,"LWS_LOG: %d,%s",level,line);
}/*}}}*/

int ws_context_s_http_func(struct lws *wsi,enum lws_callback_reasons reason,void *user,void *in,size_t len)
{/*{{{*/
  (void)user;
  (void)len;

  debug_message_7(fprintf(stderr,"ws_context_s_http_func\n"));

  switch (reason)
  {
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
      break;
    case LWS_CALLBACK_ADD_POLL_FD:
    case LWS_CALLBACK_DEL_POLL_FD:
    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
      {
        ws_context_s *wsc_ptr = (ws_context_s *)lws_context_user(lws_get_context(wsi));
        struct lws_pollargs *args_ptr = (struct lws_pollargs *)in;

#if SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
        unsigned events =
          ((args_ptr->events & 0x01) ? POLLIN  : 0) |
          // - POLLPRI ignored on windows -
          ((args_ptr->events & 0x04) ? POLLOUT : 0) |
          // - POLLERR ignored on windows -
          ((args_ptr->events & 0x10) ? POLLHUP : 0);

        int res = wsc_ptr->ws_fd_event_cb(wsc_ptr,reason,args_ptr->fd,events);
#else
        int res = wsc_ptr->ws_fd_event_cb(wsc_ptr,reason,args_ptr->fd,args_ptr->events);
#endif

        // - ERROR -
        if (res)
        {
          wsc_ptr->ret_code = 1;

          return 1;
        }
      }
      break;
    default:
      break;
  }

  return 0;
}/*}}}*/

int ws_context_s_protocol_func(struct lws *wsi,enum lws_callback_reasons reason,void *user,void *in,size_t len)
{/*{{{*/
  debug_message_7(fprintf(stderr,"ws_context_s_protocol_func\n"));

  ws_context_s *wsc_ptr = (ws_context_s *)lws_context_user(lws_get_context(wsi));
  ws_conn_s *wscn_ptr = NULL;

  switch (reason)
  {
    case LWS_CALLBACK_ESTABLISHED:
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
      {/*{{{*/

        // - create websocket connection object -
        wscn_ptr = (ws_conn_s *)cmalloc(sizeof(ws_conn_s));
        ws_conn_s_init(wscn_ptr);

        // - set websocket context pointer -
        wscn_ptr->wsc_ptr = wsc_ptr;

        // - set protocol index pointer -
        wscn_ptr->prot_idx = ws_context_s_get_protocol_idx(wsc_ptr,wsi);
        debug_assert(wscn_ptr->prot_idx != c_idx_not_exist);

        // - set websocket pointer -
        wscn_ptr->ws_ptr = wsi;

        // - store websocket connection -
        *((pointer *)user) = wscn_ptr;
      }/*}}}*/
      break;
    case LWS_CALLBACK_CLOSED:
    case LWS_CALLBACK_RECEIVE:
    case LWS_CALLBACK_CLIENT_RECEIVE:
    case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
    case LWS_CALLBACK_CLIENT_WRITEABLE:
    case LWS_CALLBACK_SERVER_WRITEABLE:
      {/*{{{*/

        // - retrieve ws connection -
        wscn_ptr = (ws_conn_s *)*((pointer *)user);
      }/*}}}*/
      break;
    default:
      break;
  }

  if (wsc_ptr->ret_code == 0)
  {
    switch (reason)
    {
      case LWS_CALLBACK_RECEIVE:
      case LWS_CALLBACK_CLIENT_RECEIVE:
        {/*{{{*/

          // - retrieve count of remaining bytes of packet -
          size_t remaining = lws_remaining_packet_payload(wsi);
          int final_frag = lws_is_final_fragment(wsi);
          bc_array_s *data_buffer = &wscn_ptr->data_buffer;

          // - message is not complete or buffered data exists -
          if (remaining != 0 || !final_frag || data_buffer->used != 0)
          {
            bc_array_s_reserve(data_buffer,len + remaining);
            bc_array_s_append(data_buffer,len,(const char *)in);
          }

          if (remaining == 0 && (data_buffer->used == 0 || final_frag))
          {
            // - set callback reason -
            wscn_ptr->reason = reason;

            // - set callback input data -
            if (data_buffer->used != 0)
            {
              wscn_ptr->data_in = data_buffer->data;
              wscn_ptr->data_len = data_buffer->used;
              data_buffer->used = 0;
            }
            else
            {
              wscn_ptr->data_in = in;
              wscn_ptr->data_len = len;
            }

            if (((ws_prot_conn_cb_t)wsc_ptr->prot_callbacks.data[wscn_ptr->prot_idx])(wscn_ptr))
            {
              wsc_ptr->ret_code = 1;

              // - release connection -
              ws_conn_s_clear(wscn_ptr);
              cfree(wscn_ptr);

              return 1;
            }
          }
        }/*}}}*/
        break;
      case LWS_CALLBACK_ESTABLISHED:
      case LWS_CALLBACK_CLIENT_ESTABLISHED:
      case LWS_CALLBACK_CLOSED:
      case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
      case LWS_CALLBACK_CLIENT_WRITEABLE:
      case LWS_CALLBACK_SERVER_WRITEABLE:
        {/*{{{*/

          // - set callback reason -
          wscn_ptr->reason = reason;

          wscn_ptr->data_in = NULL;
          wscn_ptr->data_len = 0;

          if (((ws_prot_conn_cb_t)wsc_ptr->prot_callbacks.data[wscn_ptr->prot_idx])(wscn_ptr))
          {
            wsc_ptr->ret_code = 1;

            // - release connection -
            ws_conn_s_clear(wscn_ptr);
            cfree(wscn_ptr);

            return 1;
          }

          // - if connection closed -
          if (reason == LWS_CALLBACK_CLOSED)
          {
            // - release connection -
            ws_conn_s_clear(wscn_ptr);
            cfree(wscn_ptr);
          }
        }/*}}}*/
        break;
    default:
      break;
    }
  }

  return 0;
}/*}}}*/

int ws_context_s_create(ws_context_s *this,
    usi a_port,string_array_s *a_prot_names,pointer_array_s *a_prot_callbacks,
    ws_fd_event_cb_t a_ws_fd_event_cb,void *a_user_data)
{/*{{{*/
  ws_context_s_clear(this);

  // - ERROR -
  if (a_prot_names->used != a_prot_callbacks->used)
  {
    throw_error(WS_CONTEXT_CREATE_MISMATCH_PROTOCOL_ARRAYS_SIZE);
  }

  // - ERROR -
  if (a_prot_names->used == 0)
  {
    throw_error(WS_CONTEXT_CREATE_EMPTY_PROTOCOL_ARRAYS);
  }

  // - create websocket protocols structure -
  this->protocols = (struct lws_protocols *)cmalloc((2 + a_prot_names->used)*sizeof(struct lws_protocols));

  // - configure http protocol -
  struct lws_protocols *http_prot = this->protocols;
  http_prot->name = "http-only";
  http_prot->callback = ws_context_s_http_func;
  http_prot->per_session_data_size = 0;
  http_prot->rx_buffer_size = 0;

  unsigned prot_idx = 0;
  do
  {
    // - configure websocket protocol -
    struct lws_protocols *ws_prot = this->protocols + prot_idx + 1;
    ws_prot->name = a_prot_names->data[prot_idx].data;
    ws_prot->callback = ws_context_s_protocol_func;
    ws_prot->per_session_data_size = sizeof(pointer);
    ws_prot->rx_buffer_size = 0;

  } while(++prot_idx < a_prot_names->used);

  // - configure terminator protocol -
  struct lws_protocols *term_prot = this->protocols + prot_idx + 1;
  term_prot->name = NULL;
  term_prot->callback = NULL;
  term_prot->per_session_data_size = 0;
  term_prot->rx_buffer_size = 0;

  // - create websocket context creation info -
  struct lws_context_creation_info info;
  memset(&info,0,sizeof(info));

  info.port = a_port;
  info.iface = NULL;
  info.protocols = this->protocols;
  info.extensions = ws_default_extensions;
  info.gid = -1;
  info.uid = -1;
  info.options = 0;
  info.user = this;

  // - store protocol names -
  string_array_s_copy(&this->prot_names,a_prot_names);

  // - store protocol callbacks -
  pointer_array_s_copy(&this->prot_callbacks,a_prot_callbacks);

  // - store fd change event callback -
  this->ws_fd_event_cb = a_ws_fd_event_cb;

  // - store user data -
  this->user_data = a_user_data;

  // - create wesocket context -
  this->context = lws_create_context(&info);

  // - ERROR -
  if (this->context == NULL)
  {
    ws_context_s_clear(this);

    throw_error(WS_CONTEXT_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

// === methods of structure ws_conn_s ==========================================

int ws_conn_s_write(ws_conn_s *this,
    const char *a_data,size_t a_size,enum lws_write_protocol a_protocol)
{/*{{{*/
  unsigned message_size = LWS_SEND_BUFFER_PRE_PADDING + a_size +
    LWS_SEND_BUFFER_POST_PADDING;

  // - retrieve message buffer -
  bc_array_s *buffer = &this->message_buffer;

  // - resize message buffer -
  if (buffer->used < message_size)
  {
    buffer->used = 0;
    bc_array_s_reserve(buffer,message_size);
    buffer->used = buffer->size;
  }

  // - pointer to data in buffer -
  unsigned char *buff_ptr = (unsigned char *)buffer->data + LWS_SEND_BUFFER_PRE_PADDING;

  // - fill data to buffer -
  memcpy(buff_ptr,a_data,a_size);

  // - ERROR -
  if (lws_write(this->ws_ptr,buff_ptr,a_size,a_protocol) != (int)a_size)
  {
    throw_error(WS_CONN_WRITE_ERROR);
  }

  return 0;
}/*}}}*/

// === methods of structure ws_client_s ========================================

int ws_client_s_create(ws_client_s *this,ws_context_s *a_ctx,
    const char *a_address,usi a_port,const char *a_path,const char *a_protocol)
{/*{{{*/
  ws_client_s_clear(this);

  // - fill client connect info structure -
  struct lws_client_connect_info info;
  memset(&info,0,sizeof(info));

  info.context = a_ctx->context;
  info.address = a_address;
  info.port = a_port;
  info.ssl_connection = 0;
  info.path = a_path;
  info.host = a_address;
  info.protocol = a_protocol;
  info.ietf_version_or_minus_one = -1;

  // - connect client to server -
  struct lws *wsi = lws_client_connect_via_info(&info);

  // - ERROR -
  if (wsi == NULL)
  {
    throw_error(WS_CLIENT_CREATE_ERROR);
  }

  // - store client context pointer -
  this->wsc_ptr = a_ctx;

  // - store client wsi pointer -
  this->ws_ptr = wsi;

  return 0;
}/*}}}*/

// === global functions ========================================================

void libwebsocket_cll_init()
{/*{{{*/
  lws_set_log_level(9,ws_context_s_log_emit);
}/*}}}*/

void libwebsocket_cll_clear()
{/*{{{*/
}/*}}}*/

