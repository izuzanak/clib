
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- tcp_comm_s --
@begin
methods tcp_comm_s
@end

int tcp_comm_s_create(tcp_comm_s *this,const char *a_ip,unsigned short a_port)
{/*{{{*/
  if (tcp_server_s_create(&this->server,a_ip,a_port,
        tcp_comm_s_conn_new,
        tcp_comm_s_conn_drop,
        tcp_comm_s_conn_recv,
        tcp_comm_s_conn_send,
        this))
  {
    throw_error(TCP_COMM_SERVER_CREATE_ERROR);
  }

//#ifdef CLIB_WITH_OPENSSL
//  if (tcp_server_s_init_ssl(&this->server,
//    "tests/libchannel_cll_test/resources/mycert.pem",
//    "tests/libchannel_cll_test/resources/mycert.pem"))
//  {
//    throw_error(TCP_COMM_SERVER_INIT_SSL_ERROR);
//  }
//#endif

  if(epoll_s_fd_callback(&this->server.epoll_fd,EPOLLIN | EPOLLPRI,tcp_comm_s_fd_event,this,0))
  {
    throw_error(TCP_COMM_SERVER_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int tcp_comm_s_run(tcp_comm_s *this)
{/*{{{*/
  (void)this;

  while(!g_terminate)
  {
    // - wait on events -
    int err;
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(TCP_COMM_SERVER_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int tcp_comm_s_conn_new(void *a_tcp_comm,unsigned a_index)
{/*{{{*/
  (void)a_tcp_comm;

  debug_message_5(fprintf(stderr,"tcp_comm_s_conn_new: %u\n",a_index));

  return 0;
}/*}}}*/

int tcp_comm_s_conn_drop(void *a_tcp_comm,unsigned a_index)
{/*{{{*/
  (void)a_tcp_comm;

  debug_message_5(fprintf(stderr,"tcp_comm_s_conn_drop: %u\n",a_index));

  return 0;
}/*}}}*/

int tcp_comm_s_conn_recv(void *a_tcp_comm,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
  debug_message_5(fprintf(stderr,"tcp_comm_s_conn_recv: %u - %.*s\n",a_index,a_message->used,a_message->data));

  tcp_comm_s *this = (tcp_comm_s *)a_tcp_comm;
  tcp_conn_s *conn = &this->server.conn_list.data[a_index].object;

  this->buffer.used = 0;
  bc_array_s_append_ptr(&this->buffer,"RESPONSE: ");
  bc_array_s_append(&this->buffer,a_message->used,a_message->data);

  // - clear message buffer -
  a_message->used = 0;

  if (tcp_conn_s_schedule_message(conn,&this->buffer))
  {
    throw_error(TCP_COMM_CONN_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int tcp_comm_s_conn_send(void *a_tcp_comm,unsigned a_index)
{/*{{{*/
  (void)a_tcp_comm;

  debug_message_5(fprintf(stderr,"tcp_comm_s_conn_send: %u\n",a_index));

  return 0;
}/*}}}*/

int tcp_comm_s_fd_event(void *a_tcp_comm,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  tcp_comm_s *this = (tcp_comm_s *)a_tcp_comm;

  if (tcp_server_s_fd_event(&this->server,a_index,a_epoll_event))
  {
    throw_error(TCP_COMM_CONN_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  __sync_add_and_fetch(&g_terminate,1);
}/*}}}*/

int epoll_fd_update(int a_fd,unsigned a_evts,int a_update_cb,const epoll_callback_s *a_callback)
{/*{{{*/
  return epoll_s_fd_update(g_epoll,a_fd,a_evts,a_update_cb,a_callback);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();
  libtcp_cll_init();
  libopenssl_cll_init();

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    const char *address = "127.0.0.1";
    const unsigned short port = 8001;

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,0);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(tcp_comm_s,comm);
    cassert(tcp_comm_s_create(&comm,address,port) == 0);
    cassert(tcp_comm_s_run(&comm) == 0);
  }

  libopenssl_cll_clear();
  libtcp_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

