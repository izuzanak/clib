
@begin
include "cl_websocket.h"
@end

// === methods of structure ws_context_s =======================================

void ws_context_s_log_emit(int level,const char *line)
{/*{{{*/
  //fprintf(stderr,"LWS_LOG: %d,%s",level,line);
}/*}}}*/

int ws_context_s_http_func(struct libwebsocket_context *ctx,struct libwebsocket *wsi,
    enum libwebsocket_callback_reasons reason,void *user,void *in,size_t len)
{/*{{{*/
  switch (reason)
  {
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
      break;
    case LWS_CALLBACK_ADD_POLL_FD:
    case LWS_CALLBACK_DEL_POLL_FD:
    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
      {
        ws_context_s *wsc_ptr = (ws_context_s *)libwebsocket_context_user(ctx);
        struct libwebsocket_pollargs *args_ptr = (struct libwebsocket_pollargs *)in;

#if SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
        unsigned events =
          ((args_ptr->events & 0x01) ? POLLIN  : 0) |
          // - POLLPRI ignored on windows -
          ((args_ptr->events & 0x04) ? POLLOUT : 0) |
          // - POLLERR ignored on windows -
          ((args_ptr->events & 0x10) ? POLLHUP : 0);

        wsc_ptr->ws_fd_event_cb(wsc_ptr,reason,args_ptr->fd,events);
#else
        wsc_ptr->ws_fd_event_cb(wsc_ptr,reason,args_ptr->fd,args_ptr->events);
#endif
        // FIXME TODO process callback result value
      }
      break;
    default:
      break;
  }

  return 0;
}/*}}}*/

int ws_context_s_protocol_func(struct libwebsocket_context *ctx,struct libwebsocket *wsi,
    enum libwebsocket_callback_reasons reason,void *user,void *in,size_t len)
{/*{{{*/
  switch (reason)
  {
    case LWS_CALLBACK_ESTABLISHED:
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
    case LWS_CALLBACK_CLOSED:
    case LWS_CALLBACK_RECEIVE:
    case LWS_CALLBACK_CLIENT_RECEIVE:
    case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
    case LWS_CALLBACK_CLIENT_WRITEABLE:
    case LWS_CALLBACK_SERVER_WRITEABLE:
      {
        // FIXME TODO continue ...
        cassert(0);
      }
      break;
    default:
      break;
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
  this->protocols = (struct libwebsocket_protocols *)cmalloc((2 + a_prot_names->used)*sizeof(struct libwebsocket_protocols));

  // - configure http protocol -
  struct libwebsocket_protocols *http_prot = this->protocols;
  http_prot->name = "http-only";
  http_prot->callback = ws_context_s_http_func;
  http_prot->per_session_data_size = 0;
  http_prot->rx_buffer_size = 0;

  unsigned prot_idx = 0;
  do
  {
    // - configure websocket protocol -
    struct libwebsocket_protocols *ws_prot = this->protocols + prot_idx + 1;
    ws_prot->name = a_prot_names->data[prot_idx].data;
    ws_prot->callback = a_prot_callbacks->data[prot_idx];
    ws_prot->per_session_data_size = sizeof(pointer);
    ws_prot->rx_buffer_size = 0;
    ws_prot->no_buffer_all_partial_tx = 1;

  } while(++prot_idx < a_prot_names->used);

  // - configure terminator protocol -
  struct libwebsocket_protocols *term_prot = this->protocols + prot_idx + 1;
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
  info.extensions = libwebsocket_get_internal_extensions();
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
  this->context = libwebsocket_create_context(&info);

  // - ERROR -
  if (this->context == NULL)
  {
    ws_context_s_clear(this);

    throw_error(WS_CONTEXT_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

// === methods of generated structures =========================================

// === global functions ========================================================

void libwebsocket_cll_init()
{/*{{{*/
  lws_set_log_level(9,ws_context_s_log_emit);
}/*}}}*/

void libwebsocket_cll_clear()
{/*{{{*/
}/*}}}*/

