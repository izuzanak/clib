
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === methods of generated structures =========================================

// -- channel_message_s --
@begin
methods channel_message_s
@end

// -- channel_client_s --
@begin
methods channel_client_s
@end

// -- channel_client_list_s --
@begin
methods channel_client_list_s
@end

// -- channel_comm_s --
@begin
methods channel_comm_s
@end

int channel_comm_s_create(channel_comm_s *this,
  unsigned a_count,const char **a_server_ips,const usi *a_server_ports)
{/*{{{*/
  epoll_s_create(&this->epoll,0);

  // - create client objects -
  if (a_count != 0)
  {
    unsigned create_idx = 0;
    do {
      unsigned client_idx = channel_client_list_s_append_blank(&this->client_list);
      channel_client_s *client = &this->client_list.data[client_idx].object;

      string_s_set_ptr(&client->server_ip,a_server_ips[create_idx]);
      client->server_port = a_server_ports[create_idx];

      client->message.type = 0;
      client->message.sequence = 0;

      if (channel_conn_s_create_client(&client->connection,
            client->server_ip.data,client->server_port,channel_comm_s_message,this,client_idx))
      {
        throw_error(CHANNEL_COMM_CONN_CREATE_ERROR);
      }

      // - register epoll fd events -
      if (epoll_s_fd_callback(&this->epoll,&client->connection.epoll_fd,EPOLLIN | EPOLLPRI,channel_comm_s_client_fd_event,this,client_idx))
      {
        channel_client_list_s_remove(&this->client_list,client_idx);

        throw_error(CHANNEL_COMM_CONN_EPOLL_ERROR);
      }

      // - register epoll timer -
      if (epoll_s_timer_period_now(&this->epoll,100000000ULL,channel_comm_s_client_time_event,this,client_idx,&client->epoll_timer))
      {
        channel_client_list_s_remove(&this->client_list,client_idx);

        throw_error(CHANNEL_COMM_CONN_EPOLL_ERROR);
      }

    } while(++create_idx < a_count);
  }

  return 0;
}/*}}}*/

int channel_comm_s_run(channel_comm_s *this)
{/*{{{*/
  while(!g_terminate && this->client_list.first_idx != c_idx_not_exist)
  {
    // - wait on events -
    int err;
    if ((err = epoll_s_wait(&this->epoll,32,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(CHANNEL_COMM_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int channel_comm_s_client_time_event(void *a_channel_comm,unsigned a_index,unsigned a_timer,epoll_s *a_epoll)
{/*{{{*/
  (void)a_timer;
  (void)a_epoll;

  channel_comm_s *this = (channel_comm_s *)a_channel_comm;
  channel_client_s *client = &this->client_list.data[a_index].object;

  this->buffer.used = 0;
#if OPTION_TO_JSON == ENABLED
  channel_message_s_to_json(&client->message,&this->buffer);
#else
  bc_array_s_append_format(&this->buffer,"[type:%u,sequence:%u]",
    client->message.type,client->message.sequence);
#endif
  ++client->message.sequence;
  if (channel_conn_s_schedule_message(&client->connection,&this->buffer))
  {
    throw_error(CHANNEL_COMM_CONN_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int channel_comm_s_client_fd_event(void *a_channel_comm,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  channel_comm_s *this = (channel_comm_s *)a_channel_comm;
  channel_client_s *client = &this->client_list.data[a_index].object;

  if (channel_conn_s_fd_event(&client->connection,0,a_epoll_event,a_epoll))
  {
    channel_client_s_clear(client);
    channel_client_list_s_remove(&this->client_list,a_index);
  }

  return 0;
}/*}}}*/

int channel_comm_s_message(void *a_channel_comm,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  (void)a_channel_comm;

  debug_message_5(fprintf(stderr,"conn_message: %u - %.*s\n",a_index,a_message->used,a_message->data));

  return 0;
}/*}}}*/

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  __sync_add_and_fetch(&g_terminate,1);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();
  libchannel_cll_init();

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

  CONT_INIT(channel_comm_s,comm);

  cassert(channel_comm_s_create(&comm,10,server_ips,server_ports) == 0);
  cassert(channel_comm_s_run(&comm) == 0);
  channel_comm_s_clear(&comm);

  libchannel_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

