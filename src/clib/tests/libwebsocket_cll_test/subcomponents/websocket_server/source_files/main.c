
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

int ws_fd_event_cb(ws_context_s *a_ctx,enum libwebsocket_callback_reasons a_reason,int a_fd,unsigned a_events)
{/*{{{*/
  debug_message_6(fprintf(stderr,"ws_fd_event_cb\n"));

  epoll_fd_s epoll_fd = {NULL,a_fd};
  return epoll_s_fd(a_ctx->user_data,&epoll_fd,a_events);
}/*}}}*/

int ws_prot_conn_cb(ws_conn_s *a_conn)
{/*{{{*/
  (void)a_conn;

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

      // FIXME debug output
      fprintf(stderr,"epoll fired\n");
    }
  }

  libwebsocket_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

