
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- psql_client_s --
@begin
methods psql_client_s
@end

int psql_client_s_create(psql_client_s *this,const char *a_conninfo)
{/*{{{*/
  if (psql_conn_s_create(&this->connection,
        a_conninfo,
        psql_client_s_result,
        psql_client_s_notify,
        this,0))
  {
    throw_error(PSQL_CLIENT_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

int psql_client_s_run(psql_client_s *this)
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
        throw_error(PSQL_CLIENT_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int psql_client_s_result(void *a_psql_client,unsigned a_index,psql_result_s *a_result)
{/*{{{*/
  (void)a_psql_client;
  (void)a_index;
  (void)a_result;

  fprintf(stderr,"psql_client_s_result (%p)\n",a_result);

  if (a_result != NULL)
  {
    CONT_INIT_CLEAR(var_arrays_s,tuples);
    cassert(psql_result_s_tuples(a_result,&tuples) == 0);
  }

  return 0;
}/*}}}*/

int psql_client_s_notify(void *a_psql_client,unsigned a_index,psql_notify_s *a_notify)
{/*{{{*/
  (void)a_psql_client;
  (void)a_index;
  (void)a_notify;

  fprintf(stderr,"psql_client_s_notify: %s, %d, %s\n",
      psql_notify_s_channel(a_notify),
      psql_notify_s_pid(a_notify),
      psql_notify_s_payload(a_notify));

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

  cassert(signal_s_simple_handler(signal_handler) == 0);

  {
    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(psql_client_s,psql_client);
    cassert(psql_client_s_create(&psql_client,
          "host=deb12vm port=5432 user=devel password=devel dbname=metcoll") == 0);

    cassert(psql_conn_s_send_query(&psql_client.connection,"LISTEN test;") == 0);
#ifdef LIBPQ_HAS_PIPELINING
    cassert(psql_conn_s_send_query(&psql_client.connection,"BEGIN;") == 0);
    cassert(psql_conn_s_send_query(&psql_client.connection,"END;") == 0);
    cassert(psql_conn_s_flush(&psql_client.connection) == 0);
#endif

    cassert(psql_client_s_run(&psql_client) == 0);
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

