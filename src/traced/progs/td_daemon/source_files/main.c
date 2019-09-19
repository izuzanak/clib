
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === methods of generated structures =========================================

// -- td_daemon_s --
@begin
methods td_daemon_s
@end

int td_daemon_s_create(td_daemon_s *this)
{/*{{{*/
  td_daemon_s_clear(this);

  // - reset configuration changed flag -
  this->config_changed_flag = 0;

  epoll_s_create(&this->epoll,0);

  return 0;
}/*}}}*/

int td_daemon_s_process_config(td_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("process configuration");

  // - update last configuration -
  td_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int td_daemon_s_run(td_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_0("running");

  while(!g_terminate)
  {
    if (this->config_changed_flag)
    {
      if (td_daemon_s_process_config(this))
      {
        throw_error(TD_DAEMON_CONFIG_DATA_ERROR);
      }

      if (g_terminate)
      {
        break;
      }

      // - reset configuration changed flag -
      this->config_changed_flag = 0;
    }

    // - wait on events -
    int err;
    if ((err = epoll_s_wait(&this->epoll,1,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(TD_DAEMON_EPOLL_WAIT_ERROR);
      }
    }
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
  libchannel_cll_init();
  libtrace_cll_init();
  libconf_tdl_init();

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(process_s,process);
    cassert(process_s_create(&process,"td_daemon") == 0);

    do {
      CONT_INIT_CLEAR(td_daemon_s,daemon);

      if (td_daemon_s_create(&daemon) ||
          td_config_s_read_file(&daemon.config,TD_JSON_CONFIG_FILE))
      {
        break;
      }

      // - set configuration changed flag -
      daemon.config_changed_flag = 1;

      if (td_daemon_s_run(&daemon)) {}
    } while(0);
  }

  libconf_tdl_clear();
  libtrace_cll_clear();
  libchannel_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

