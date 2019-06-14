
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  __sync_add_and_fetch(&g_terminate,1);
}/*}}}*/

int epoll_ws_fd_event(void *a_ws_context,unsigned a_index,epoll_event_s *a_event,epoll_s *a_epoll)
{/*{{{*/
  debug_message_6(fprintf(stderr,"epoll_ws_fd_event\n"));

  ws_context_s *ws_context = (ws_context_s *)a_ws_context;

  struct pollfd pollfd = {a_event->data.fd,a_event->events,a_event->events};
  return ws_context_s_process_fd(ws_context,&pollfd);
}/*}}}*/

int ws_fd_event_cb(ws_context_s *a_ws_context,enum libwebsocket_callback_reasons a_reason,int a_fd,unsigned a_events)
{/*{{{*/
  debug_message_6(fprintf(stderr,"ws_fd_event_cb\n"));

  epoll_fd_s epoll_fd = {NULL,a_fd};
  return epoll_s_fd_callback(a_ws_context->user_data,&epoll_fd,a_events,epoll_ws_fd_event,a_ws_context,0);
}/*}}}*/

int ws_prot_conn_cb(ws_conn_s *a_conn)
{/*{{{*/
  debug_message_6(fprintf(stderr,"ws_prot_conn_cb\n"));

  switch (a_conn->reason)
  {
    case LWS_CALLBACK_ESTABLISHED:
      debug_message_6(fprintf(stderr,"LWS_CALLBACK_ESTABLISHED\n"));
      break;
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
      debug_message_6(fprintf(stderr,"LWS_CALLBACK_CLIENT_ESTABLISHED\n"));
      break;
    case LWS_CALLBACK_CLOSED:
      debug_message_6(fprintf(stderr,"LWS_CALLBACK_CLOSED\n"));
      break;
    case LWS_CALLBACK_RECEIVE:
      debug_message_6(fprintf(stderr,"LWS_CALLBACK_RECEIVE\n"));
      break;
    case LWS_CALLBACK_CLIENT_RECEIVE:
      debug_message_6(fprintf(stderr,"LWS_CALLBACK_CLIENT_RECEIVE\n"));
      break;
    case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
      debug_message_6(fprintf(stderr,"LWS_CALLBACK_CLIENT_RECEIVE_PONG\n"));
      break;
    case LWS_CALLBACK_CLIENT_WRITEABLE:
      debug_message_6(fprintf(stderr,"LWS_CALLBACK_CLIENT_WRITEABLE\n"));
      break;
    case LWS_CALLBACK_SERVER_WRITEABLE:
      debug_message_6(fprintf(stderr,"LWS_CALLBACK_SERVER_WRITEABLE\n"));
      break;
    default:
      break;
  }

  return 0;
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();
  libwebsocket_cll_init();

  cassert(signal_s_simple_handler(signal_handler) == 0);

  {
    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,0);

    CONT_INIT_CLEAR(string_array_s,prot_names);
    string_array_s_push_ptr(&prot_names,"protocol_0");
    string_array_s_push_ptr(&prot_names,"protocol_1");

    CONT_INIT_CLEAR(pointer_array_s,prot_callbacks);
    pointer_array_s_push(&prot_callbacks,ws_prot_conn_cb);
    pointer_array_s_push(&prot_callbacks,ws_prot_conn_cb);

    CONT_INIT_CLEAR(ws_context_s,ctx);
    cassert(ws_context_s_create(&ctx,8888,&prot_names,&prot_callbacks,ws_fd_event_cb,&epoll) == 0);

    while (!g_terminate)
    {
      // - wait on events -
      int err;
      if ((err = epoll_s_wait(&epoll,32,-1)))
      {
        cassert(err == ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED);
      }
    }
  }

  libwebsocket_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

