
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === methods of generated structures =========================================

// -- message_queue_s --
@begin
methods message_queue_s
@end

// -- message_queues_s --
@begin
methods message_queues_s
@end

// -- gpsd_server_s --
@begin
methods gpsd_server_s
@end

int gpsd_server_s_create(gpsd_server_s *this,const char *a_ip,unsigned short a_port)
{/*{{{*/
  epoll_s_create(&this->epoll,0);

  if(tcp_server_s_create(&this->server,a_ip,a_port,
        gpsd_server_s_conn_new,
        gpsd_server_s_conn_drop,
        gpsd_server_s_conn_message,
        this))
  {
    throw_error(GPSD_SERVER_CREATE_ERROR);
  }

  if(epoll_s_fd_callback(&this->epoll,&this->server.epoll_fd,EPOLLIN | EPOLLPRI,gpsd_server_s_fd_event,this,0))
  {
    throw_error(GPSD_SERVER_EPOLL_ERROR);
  }

  // - schedule message send timer -
  struct itimerspec its_message = {{0,25000000},{0,1}};

  if (epoll_s_timer_callback(&this->epoll,&this->message_timer,&its_message,0,
        gpsd_server_s_message_time_event,this,0))
  {
    throw_error(GPSD_SERVER_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

void gpsd_server_s_schedule_message(gpsd_server_s *this,unsigned a_index,bc_array_s *a_buffer)
{/*{{{*/
  CONT_INIT(bc_array_s,buffer);

  unsigned offset = 0;
  do {
    unsigned chunk_size = rand() % 10 + 1;
    
    if (chunk_size > a_buffer->used - offset)
    {
      chunk_size = a_buffer->used - offset;
    }

    // - insert chunk to message queue -
    bc_array_s_set(&buffer,chunk_size,a_buffer->data + offset);

    message_queue_s *queue = message_queues_s_at(&this->message_queues,a_index);
    message_queue_s_insert_blank(queue);
    bc_array_s_swap(message_queue_s_last(queue),&buffer);

    offset += chunk_size;
  } while(a_buffer->used > offset);
}/*}}}*/

int gpsd_server_s_run(gpsd_server_s *this)
{/*{{{*/
  while(!g_terminate)
  {
    // - wait on events -
    int err;
    if ((err = epoll_s_wait(&this->epoll,32,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(GPSD_SERVER_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int gpsd_server_s_message_time_event(void *a_gpsd_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  (void)a_index;
  (void)a_epoll;

  debug_message_7(fprintf(stderr,"gpsd_server_s_message_time_event\n"));

  gpsd_server_s *this = (gpsd_server_s *)a_gpsd_server;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(GPSD_SERVER_TIMER_READ_ERROR);
  }

  if (this->message_queues.used != 0)
  {
    // - process all queues -
    unsigned index = 0;
    do {
      message_queue_s *queue = message_queues_s_at(&this->message_queues,index);

      // - queue is not empty -
      if (queue->used != 0)
      {
        bc_array_s *message = message_queue_s_next(queue);
        tcp_conn_s *conn = &this->server.conn_list.data[index].object;
        
        if (tcp_conn_s_schedule_message(conn,message))
        {
          throw_error(GPSD_CONN_SCHEDULE_MESSAGE_ERROR);
        }
      }
    } while(++index < this->message_queues.used);
  }

  return 0;
}/*}}}*/

int gpsd_server_s_conn_new(void *a_gpsd_server,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"gpsd_server_s_conn_new: %u\n",a_index));

  gpsd_server_s *this = (gpsd_server_s *)a_gpsd_server;

  while (a_index >= this->message_queues.used)
  {
    message_queues_s_push_blank(&this->message_queues);
  }

  this->buffer.used = 0;
  bc_array_s_append_ptr(&this->buffer,
      "{\"class\":\"VERSION\",\"release\":\"3.17\",\"rev\":\"3.17\",\"proto_major\":3,\"proto_minor\":12}\x0d\x0a"
      "{\"class\":\"VERSION\",\"release\":\"3.17\",\"rev\":\"3.17\",\"proto_major\":3,\"proto_minor\":12}\x0d\x0a"
      "{\"class\":\"VERSION\",\"release\":\"3.17\",\"rev\":\"3.17\",\"proto_major\":3,\"proto_minor\":12}\x0d\x0a"
      "{\"class\":\"VERSION\",\"release\":\"3.17\",\"rev\":\"3.17\",\"proto_major\":3,\"proto_minor\":12}\x0d\x0a");
  gpsd_server_s_schedule_message(this,a_index,&this->buffer);

  return 0;
}/*}}}*/

int gpsd_server_s_conn_drop(void *a_gpsd_server,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"gpsd_server_s_conn_drop: %u\n",a_index));

  gpsd_server_s *this = (gpsd_server_s *)a_gpsd_server;

  // - clear connection message queue -
  message_queue_s_clear(message_queues_s_at(&this->message_queues,a_index));

  return 0;
}/*}}}*/

int gpsd_server_s_conn_message(void *a_gpsd_server,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
  debug_message_5(fprintf(stderr,"gpsd_server_s_conn_message: %u - %.*s\n",a_index,a_message->used,a_message->data));

  // FIXME TODO continue ...

  // - clear message buffer -
  a_message->used = 0;

  return 0;
}/*}}}*/

int gpsd_server_s_fd_event(void *a_gpsd_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  gpsd_server_s *this = (gpsd_server_s *)a_gpsd_server;

  if (tcp_server_s_fd_event(&this->server,a_index,a_epoll_event,a_epoll))
  {
    throw_error(GPSD_CONN_SERVER_FD_EVENT_ERROR);
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

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();
  libtcp_cll_init();
  libgpsd_cll_init();

  cassert(signal_s_simple_handler(signal_handler) == 0);

  const char *address = "127.0.0.1";
  const unsigned short port = 8001;

  CONT_INIT(gpsd_server_s,comm);
  cassert(gpsd_server_s_create(&comm,address,port) == 0);
  cassert(gpsd_server_s_run(&comm) == 0);
  gpsd_server_s_clear(&comm);

  libgpsd_cll_clear();
  libtcp_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

