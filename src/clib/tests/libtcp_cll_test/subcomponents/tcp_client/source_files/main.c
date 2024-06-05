
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- tcp_message_s --
@begin
methods tcp_message_s
@end

// -- tcp_client_s --
@begin
methods tcp_client_s
@end

// -- tcp_client_list_s --
@begin
methods tcp_client_list_s
@end

// -- tcp_comm_s --
@begin
methods tcp_comm_s
@end

int tcp_comm_s_create(tcp_comm_s *this,
  unsigned a_count,const char **a_server_ips,const usi *a_server_ports)
{/*{{{*/

  // - create client objects -
  if (a_count != 0)
  {
    unsigned create_idx = 0;
    do {
      unsigned client_idx = tcp_client_list_s_append_blank(&this->client_list);
      tcp_client_s *client = &this->client_list.data[client_idx].object;

      string_s_set_ptr(&client->server_ip,a_server_ips[create_idx]);
      client->server_port = a_server_ports[create_idx];

      client->message.type = 0;
      client->message.sequence = 0;

      if (tcp_conn_s_create_client(&client->connection,
            client->server_ip.data,client->server_port,tcp_comm_s_recv,tcp_comm_s_send,this,client_idx))
      {
        throw_error(TCP_COMM_CONN_CREATE_ERROR);
      }

//#ifdef CLIB_WITH_OPENSSL
//      if (tcp_conn_s_init_ssl(&client->connection))
//      {
//        throw_error(TCP_COMM_CONN_INIT_SSL_ERROR);
//      }
//#endif

      // - register epoll fd events -
      if (epoll_s_fd_callback(&client->connection.epoll_fd,EPOLLIN | EPOLLPRI,tcp_comm_s_client_fd_event,this,client_idx))
      {
        tcp_client_list_s_remove(&this->client_list,client_idx);

        throw_error(TCP_COMM_CONN_EPOLL_ERROR);
      }

      // - register epoll timer -
      struct itimerspec itimerspec = {{0,100000000},{0,1}};
      if (epoll_s_timer_callback(&client->epoll_timer,&itimerspec,0,tcp_comm_s_client_time_event,this,client_idx))
      {
        tcp_client_list_s_remove(&this->client_list,client_idx);

        throw_error(TCP_COMM_CONN_EPOLL_ERROR);
      }

    } while(++create_idx < a_count);
  }

  return 0;
}/*}}}*/

int tcp_comm_s_run(tcp_comm_s *this)
{/*{{{*/
  while(!g_terminate && this->client_list.first_idx != c_idx_not_exist)
  {
    // - wait on events -
    int err;
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(TCP_COMM_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int tcp_comm_s_client_time_event(void *a_tcp_comm,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(TCP_COMM_TIMER_READ_ERROR);
  }

  tcp_comm_s *this = (tcp_comm_s *)a_tcp_comm;
  tcp_client_s *client = &this->client_list.data[a_index].object;

  this->buffer.used = 0;
#if OPTION_TO_JSON == ENABLED
  tcp_message_s_to_json(&client->message,&this->buffer);
#else
  bc_array_s_append_format(&this->buffer,"[type:%u,sequence:%u]",
    client->message.type,client->message.sequence);
#endif
  ++client->message.sequence;
  if (tcp_conn_s_schedule_message(&client->connection,&this->buffer))
  {
    throw_error(TCP_COMM_CONN_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int tcp_comm_s_client_fd_event(void *a_tcp_comm,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  tcp_comm_s *this = (tcp_comm_s *)a_tcp_comm;
  tcp_client_s *client = &this->client_list.data[a_index].object;

  if (tcp_conn_s_fd_event(&client->connection,0,a_epoll_event))
  {
    tcp_client_s_clear(client);
    tcp_client_list_s_remove(&this->client_list,a_index);
  }

  return 0;
}/*}}}*/

int tcp_comm_s_recv(void *a_tcp_comm,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
  (void)a_tcp_comm;
  (void)a_index;

  debug_message_5(fprintf(stderr,"conn_message: %u - %.*s\n",a_index,a_message->used,a_message->data));

  // - clear message buffer -
  a_message->used = 0;

  return 0;
}/*}}}*/

int tcp_comm_s_send(void *a_tcp_comm,unsigned a_index)
{/*{{{*/
  (void)a_tcp_comm;
  (void)a_index;

  debug_message_5(fprintf(stderr,"conn_send: %u\n",a_index));

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

    const char *server_ips[] =
    {/*{{{*/
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
      "127.0.0.1",
    };/*}}}*/

    const usi server_ports[] =
    {/*{{{*/
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
      8001,
    };/*}}}*/

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(tcp_comm_s,comm);
    cassert(tcp_comm_s_create(&comm,32,server_ips,server_ports) == 0);
    cassert(tcp_comm_s_run(&comm) == 0);
  }

  libopenssl_cll_clear();
  libtcp_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

