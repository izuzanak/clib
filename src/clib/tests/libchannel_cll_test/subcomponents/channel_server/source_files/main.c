
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

void conn_new(void *a_channel_server,unsigned a_index)
{/*{{{*/
  (void)a_channel_server;
  (void)a_index;

  debug_message_5(fprintf(stderr,"conn_new: %u\n",a_index));
}/*}}}*/

void conn_drop(void *a_channel_server,unsigned a_index)
{/*{{{*/
  (void)a_channel_server;
  (void)a_index;

  debug_message_5(fprintf(stderr,"conn_drop: %u\n",a_index));
}/*}}}*/

int conn_message(void *a_channel_server,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  (void)a_channel_server;
  (void)a_index;

  debug_message_5(fprintf(stderr,"conn_message: %u - %.*s\n",a_index,a_message->used,a_message->data));

  return 0;
}/*}}}*/

void server_fd_event(void *a_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  channel_server_s *server = (channel_server_s *)a_server;

  cassert(channel_server_s_fd_event(server,a_index,a_epoll_event,a_epoll) == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  cassert(signal_s_simple_handler(signal_handler) == 0);

  const char *address = "127.0.0.1";
  const unsigned short port = 8001;

  CONT_INIT(epoll_s,epoll);
  cassert(epoll_s_create(&epoll,0) == 0);

  CONT_INIT(string_s,ip);
  string_s_set_ptr(&ip,address);

  CONT_INIT(channel_server_s,server);
  cassert(channel_server_s_create(&server,&ip,port,
        conn_new,
        conn_drop,
        conn_message,
        &server) == 0);

  cassert(epoll_s_fd_callback(&epoll,&server.epoll_fd,EPOLLIN | EPOLLPRI,server_fd_event,&server,0) == 0)

  while(!g_terminate)
  {
    // - wait on events -
    int err;
    if ((err = epoll_s_wait(&epoll,32,-1)))
    {
      cassert(err == ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED);
    }
  }

  channel_server_s_clear(&server);
  epoll_s_clear(&epoll);
  string_s_clear(&ip);

  memcheck_release_assert();

  return 0;
}/*}}}*/

