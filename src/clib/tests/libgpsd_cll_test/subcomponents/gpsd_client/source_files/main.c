
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- gpsd_client_s --
@begin
methods gpsd_client_s
@end

int gpsd_client_s_create(gpsd_client_s *this,const char *a_server_ip,usi a_server_port)
{/*{{{*/

  // - create gpsd connection -
  if (gpsd_conn_s_create(&this->connection,
        a_server_ip,a_server_port,
        gpsd_client_s_gpsd_callback,
        this,0))
  {
    throw_error(GPSD_CONN_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

int gpsd_client_s_run(gpsd_client_s *this)
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
        throw_error(GPSD_CONN_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int gpsd_client_s_gpsd_callback(void *a_gpsd_client,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  (void)a_ap;

  gpsd_client_s *this = (gpsd_client_s *)a_gpsd_client;

  switch (a_type)
  {
  case cl_gpsd_cbreq_NEW:
    {/*{{{*/
      this->buffer.used = 0;
      bc_array_s_append_ptr(&this->buffer,"?WATCH={\"class\":\"WATCH\",\"json\":true}");

      if (gpsd_conn_s_send_message(&this->connection,&this->buffer))
      {
        throw_error(GPSD_CONN_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case cl_gpsd_cbreq_DROP:
    {/*{{{*/
    }/*}}}*/
    break;
  case cl_gpsd_cbreq_VERSION:
    {/*{{{*/
      //var_s msg = va_arg(a_ap,var_s);
    }/*}}}*/
    break;
  case cl_gpsd_cbreq_DEVICES:
    {/*{{{*/
      //var_s msg = va_arg(a_ap,var_s);
    }/*}}}*/
    break;
  case cl_gpsd_cbreq_DEVICE:
    {/*{{{*/
      //var_s msg = va_arg(a_ap,var_s);
    }/*}}}*/
    break;
  case cl_gpsd_cbreq_SKY:
    {/*{{{*/
      //var_s msg = va_arg(a_ap,var_s);
    }/*}}}*/
    break;
  case cl_gpsd_cbreq_WATCH:
    {/*{{{*/
      //var_s msg = va_arg(a_ap,var_s);
    }/*}}}*/
    break;
  case cl_gpsd_cbreq_TPV:
    {/*{{{*/
      //var_s msg = va_arg(a_ap,var_s);
    }/*}}}*/
    break;
  default:
    {/*{{{*/

      // - log message -
      log_info_2("gpsd client %u, unknown message",a_index);
    }/*}}}*/
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
  libgpsd_cll_init();

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(gpsd_client_s,gpsd_client);

    cassert(gpsd_client_s_create(&gpsd_client,"127.0.0.1",8001) == 0);
    cassert(gpsd_client_s_run(&gpsd_client) == 0);
  }

  libgpsd_cll_clear();
  libtcp_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

