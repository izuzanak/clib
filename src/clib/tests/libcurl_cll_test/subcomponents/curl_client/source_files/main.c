
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

int epoll_curl_fd_event(void *a_curl_multi,unsigned a_index,epoll_event_s *a_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;
  (void)a_epoll;

  debug_message_6(fprintf(stderr,"epoll_curl_fd_event\n"));

  curl_multi_s *curl_multi = (curl_multi_s *)a_curl_multi;

  int running;
  return curl_multi_s_socket_action(curl_multi,a_event->data.fd,a_event->events,&running);
}/*}}}*/

int curl_socket_cb(curl_multi_s *a_curl_multi,int a_what,int a_fd,unsigned a_events)
{/*{{{*/
  (void)a_what;

  debug_message_6(fprintf(stderr,"curl_socket_cb\n"));

  epoll_s *epoll = (epoll_s *)a_curl_multi->user_data;
  epoll_fd_s epoll_fd = {NULL,a_fd};

  return epoll_s_fd_callback(epoll,&epoll_fd,a_events,epoll_curl_fd_event,a_curl_multi,0);
}/*}}}*/

int curl_response_cb(curl_result_s *a_curl_result)
{/*{{{*/
  debug_message_6(fprintf(stderr,"curl_response_cb\n"));

  // FIXME TODO continue ...
  fprintf(stderr,"a_curl_result->data.used: %u\n",a_curl_result->data.used);
  fprintf(stderr,"a_curl_result->user_data: %p\n",a_curl_result->user_data);

  return 0;
}/*}}}*/
// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();
  libcurl_cll_init();

  cassert(signal_s_simple_handler(signal_handler) == 0);

  {
    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,0);

    CONT_INIT_CLEAR(curl_multi_s,curl_multi);
    cassert(curl_multi_s_create(&curl_multi,curl_socket_cb,curl_response_cb,&epoll) == 0);

    unsigned idx = 0;
    do {
      cassert(curl_multi_s_GET(&curl_multi,"http://127.0.0.1",(void *)idx) == 0);
    } while(++idx < 100);

    int running;
    cassert(curl_multi_s_socket_action(&curl_multi,CURL_SOCKET_TIMEOUT,0,&running) == 0);

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

  libcurl_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

