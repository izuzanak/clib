
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- http_comm_s --
@begin
methods http_comm_s
@end

int http_comm_s_create(http_comm_s *this,const char *a_ip,unsigned short a_port)
{/*{{{*/
  if (http_server_s_create(&this->server,a_ip,a_port,
        http_comm_s_conn_request,
        http_comm_s_conn_response,
        this))
  {
    throw_error(HTTP_COMM_SERVER_CREATE_ERROR);
  }

//#ifdef CLIB_WITH_OPENSSL
//  if (tcp_server_s_init_ssl(&this->server.server,
//    "tests/libhttp_cll_test/resources/mycert.pem",
//    "tests/libhttp_cll_test/resources/mycert.pem"))
//  {
//    throw_error(HTTP_COMM_SERVER_INIT_SSL_ERROR);
//  }
//#endif

  if(epoll_s_fd_callback(&this->server.server.epoll_fd,
        EPOLLIN | EPOLLPRI,http_comm_s_fd_event,this,0))
  {
    throw_error(HTTP_COMM_SERVER_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int http_comm_s_run(http_comm_s *this)
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
        throw_error(HTTP_COMM_SERVER_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int http_comm_s_conn_request(void *a_http_comm,unsigned a_index,int a_complete)
{/*{{{*/
  http_comm_s *this = (http_comm_s *)a_http_comm;

  debug_message_5(fprintf(stderr,"http_comm_s_conn_request: %u\n",a_index));

  if (a_complete)
  {
    // TODO (devel) debug response
    CONT_INIT(bc_array_s,buffer);
    bc_array_s_append_ptr(&buffer,
"HTTP/1.1 200 OK\r\n"
"Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
"Server: Apache/2.2.14 (Win32)\r\n"
"Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
"Content-Length: 55\r\n"
"Content-Type: text/html\r\n"
"Connection: close\r\n"
"\r\n"
"<html>\r\n"
"<body>\r\n"
"<h1>Hello, World!</h1>\r\n"
"</body>\r\n"
"</html>\r\n");

    tcp_conn_s *tcp_conn = tcp_conn_list_s_at(&this->server.server.conn_list,a_index);

    if (tcp_conn_s_schedule_message(tcp_conn,&buffer))
    {
      throw_error(HTTP_COMM_SCHEDULE_RESPONSE_ERROR);
    }
  }

  return 0;
}/*}}}*/

int http_comm_s_conn_response(void *a_http_comm,unsigned a_index)
{/*{{{*/
  (void)a_http_comm;

  debug_message_5(fprintf(stderr,"http_comm_s_conn_response: %u\n",a_index));

  return 0;
}/*}}}*/

int http_comm_s_fd_event(void *a_http_comm,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  http_comm_s *this = (http_comm_s *)a_http_comm;

  if (tcp_server_s_fd_event(&this->server.server,a_index,a_epoll_event))
  {
    throw_error(HTTP_COMM_CONN_SERVER_FD_EVENT_ERROR);
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

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    const char *address = "192.168.3.6";
    const unsigned short port = 8001;

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(http_comm_s,comm);
    cassert(http_comm_s_create(&comm,address,port) == 0);
    cassert(http_comm_s_run(&comm) == 0);
  }

  libtcp_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

