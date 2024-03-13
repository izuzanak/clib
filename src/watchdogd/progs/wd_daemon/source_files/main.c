
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- wd_monitor_s --
@begin
methods wd_monitor_s
@end

// -- wd_monitor_tree_s --
@begin
methods wd_monitor_tree_s
@end

// -- wd_daemon_s --
@begin
methods wd_daemon_s
@end

int wd_daemon_s_create(wd_daemon_s *this)
{/*{{{*/
  wd_daemon_s_clear(this);

  // - reset configuration changed flag -
  this->config_changed_flag = 0;

  // - set monitors ok flag -
  this->monitors_ok = 1;

  return 0;
}/*}}}*/

int wd_daemon_s_process_config(wd_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("process configuration");

  // - if communication channel configuration changed -
  if (!wd_conf_ip_port_s_compare(&this->config.channel,&this->last_config.channel))
  {
    wd_conf_ip_port_s *channel_cfg = &this->config.channel;

    // - create communication channel -
    if (wd_channel_s_create(&this->channel,channel_cfg->ip.data,channel_cfg->port,
          wd_daemon_s_channel_callback,
          this))
    {
      throw_error(WD_DAEMON_CHANNEL_CREATE_ERROR);
    }
  }

  // - if watchdog configuration changed -
  if (!wd_conf_watchdog_s_compare(&this->config.watchdog,&this->last_config.watchdog))
  {
    wd_conf_watchdog_s *watchdog_cfg = &this->config.watchdog;

    if (this->wdg_fd != -1 && this->monitors_ok)
    {
      // - stop watchdog driver -
      if (fd_s_write(&this->wdg_fd,"V",1))
      {
        throw_error(WD_DAEMON_WATCHDOG_WRITE_ERROR);
      }
    }

    fd_s_clear(&this->wdg_fd);

    // - watchdog path is not empty -
    if (watchdog_cfg->path.size > 1)
    {
      // - open watchdog driver -
      if ((this->wdg_fd = open(watchdog_cfg->path.data,O_WRONLY | O_APPEND)) == -1)
      {
        throw_error(WD_DAEMON_WATCHDOG_OPEN_ERROR);
      }

      // - schedule watchdog timer -
      struct itimerspec its_watchdog = {{watchdog_cfg->period,0},{0,1}};
      if (epoll_s_timer_callback(&this->wdg_timer,&its_watchdog,0,
            wd_daemon_s_watchdog_time_event,this,0))
      {
        throw_error(WD_DAEMON_EPOLL_ERROR);
      }
    }
    else
    {
      // - disable watchdog timer -
      epoll_timer_s_clear(&this->wdg_timer);
    }
  }

  // - update last configuration -
  wd_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int wd_daemon_s_run(wd_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_0("running");

  while(!g_terminate)
  {
    if (this->config_changed_flag)
    {
      if (wd_daemon_s_process_config(this))
      {
        throw_error(WD_DAEMON_CONFIG_DATA_ERROR);
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
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(WD_DAEMON_EPOLL_WAIT_ERROR);
      }
    }
  }

  // - terminated normally -
  if (this->wdg_fd != -1 && this->monitors_ok)
  {
    // - stop watchdog driver -
    if (fd_s_write(&this->wdg_fd,"V",1))
    {
      throw_error(WD_DAEMON_WATCHDOG_WRITE_ERROR);
    }
  }

  return 0;
}/*}}}*/

int wd_daemon_s_channel_callback(void *a_wd_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  wd_daemon_s *this = (wd_daemon_s *)a_wd_daemon;

  switch (a_type)
  {
  case wd_channel_cbreq_NEW:
    {/*{{{*/
      while (this->channel_watches.used <= a_index)
      {
        ui_array_s_push(&this->channel_watches,0);
      }

      // - reset channel watch flag -
      *ui_array_s_at(&this->channel_watches,a_index) = 0;
    }/*}}}*/
    break;
  case wd_channel_cbreq_DROP:
    {/*{{{*/

      // - reset channel watch flag -
      *ui_array_s_at(&this->channel_watches,a_index) = 0;
    }/*}}}*/
    break;
  case wd_channel_cbreq_ENABLE:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *name = va_arg(a_ap,const string_s *);
      lli timeout = va_arg(a_ap,lli);

      // - log message -
      log_info_2("channel server %u, enable monitor, name: %s, timeout: %" HOST_LL_FORMAT "d",
          a_index,name->data,timeout);

      wd_monitor_s search_monitor = {.name={name->size,name->data}};
      unsigned monitor_idx = wd_monitor_tree_s_get_idx(&this->monitor_tree,&search_monitor);

      // - monitor does not exists -
      if (monitor_idx == c_idx_not_exist)
      {
        CONT_INIT_CLEAR(wd_monitor_s,insert_monitor);
        string_s_copy(&insert_monitor.name,name);

        monitor_idx = wd_monitor_tree_s_swap_insert(&this->monitor_tree,&insert_monitor);
      }

      wd_monitor_s *monitor = wd_monitor_tree_s_at(&this->monitor_tree,monitor_idx);
      monitor->timeout = timeout;

      // - retrieve monitor enable time -
      if (clock_s_gettime(CLOCK_MONOTONIC,&monitor->enable_time))
      {
        throw_error(WD_DAEMON_GET_TIME_ERROR);
      }

      // - set monitor keepalive time -
      monitor->keepalive_time = monitor->enable_time;

      // - schedule monitor timer -
      struct itimerspec its_monitor = {{0,0},{monitor->timeout,0}};
      if (epoll_s_timer_callback(&monitor->timer,&its_monitor,0,
            wd_daemon_s_monitor_time_event,this,monitor_idx))
      {
        throw_error(WD_DAEMON_EPOLL_ERROR);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"enable\",\"id\":%" HOST_LL_FORMAT "d}",id);

      if (wd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(WD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_cbreq_DISABLE:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *name = va_arg(a_ap,const string_s *);

      // - log message -
      log_info_2("channel server %u, disable monitor, name: %s",a_index,name->data);

      wd_monitor_s search_monitor = {.name={name->size,name->data}};
      unsigned monitor_idx = wd_monitor_tree_s_get_idx(&this->monitor_tree,&search_monitor);

      if (monitor_idx != c_idx_not_exist)
      {
        wd_monitor_s *monitor = wd_monitor_tree_s_at(&this->monitor_tree,monitor_idx);
        wd_monitor_tree_s_remove(&this->monitor_tree,monitor_idx);
        wd_monitor_s_clear(monitor);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"disable\",\"id\":%" HOST_LL_FORMAT "d}",id);

      if (wd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(WD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_cbreq_KEEPALIVE:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      (void)id;

      const string_s *name = va_arg(a_ap,const string_s *);

      wd_monitor_s search_monitor = {.name={name->size,name->data}};
      unsigned monitor_idx = wd_monitor_tree_s_get_idx(&this->monitor_tree,&search_monitor);

      if (monitor_idx == c_idx_not_exist)
      {
        throw_error(WD_DAEMON_INVALID_MONITOR_NAME);
      }

      wd_monitor_s *monitor = wd_monitor_tree_s_at(&this->monitor_tree,monitor_idx);

      // - retrieve monitor keepalive time -
      if (clock_s_gettime(CLOCK_MONOTONIC,&monitor->keepalive_time))
      {
        throw_error(WD_DAEMON_GET_TIME_ERROR);
      }

      // - schedule monitor timer -
      struct itimerspec its_monitor = {{0,0},{monitor->timeout,0}};
      if (epoll_s_timer_callback(&monitor->timer,&its_monitor,0,
            wd_daemon_s_monitor_time_event,this,monitor_idx))
      {
        throw_error(WD_DAEMON_EPOLL_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_cbreq_WATCH:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);

      // - set channel watch flag -
      *ui_array_s_at(&this->channel_watches,a_index) = 1;

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"watch\",\"id\":%" HOST_LL_FORMAT "d}",id);

      if (wd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(WD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_cbreq_IGNORE:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);

      // - reset channel watch flag -
      *ui_array_s_at(&this->channel_watches,a_index) = 0;

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"ignore\",\"id\":%" HOST_LL_FORMAT "d}",id);

      if (wd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(WD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_cbreq_STATUS:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"status\",\"id\":%" HOST_LL_FORMAT "d,\"data\":[",id);

      if (this->monitor_tree.count != 0)
      {
        time_s time;
        if (clock_s_gettime(CLOCK_MONOTONIC,&time))
        {
          throw_error(WD_DAEMON_GET_TIME_ERROR);
        }

        wd_monitor_tree_s_node *mtn_ptr = this->monitor_tree.data;
        wd_monitor_tree_s_node *mtn_ptr_end = mtn_ptr + this->monitor_tree.used;
        int first_monitor = 1;

        do {
          if (mtn_ptr->valid)
          {
            wd_monitor_s *monitor = &mtn_ptr->object;

            if (first_monitor)
            {
              first_monitor = 0;
            }
            else
            {
              bc_array_s_push(&this->buffer,',');
            }

            bc_array_s_append_ptr(&this->buffer,"{\"name\":");
            string_s_to_json(&monitor->name,&this->buffer);
            bc_array_s_append_format(&this->buffer,
                ",\"timeout\":%" HOST_LL_FORMAT "d"
                ",\"enabled\":%" HOST_LL_FORMAT "d"
                ",\"keepalive\":%" HOST_LL_FORMAT "d"
                "}",
                monitor->timeout,
                (time - monitor->enable_time) / 1000000000ULL,
                (time - monitor->keepalive_time) / 1000000000ULL);
          }
        } while(++mtn_ptr < mtn_ptr_end);
      }

      bc_array_s_append(&this->buffer,2,"]}");

      if (wd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(WD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  default:
    {/*{{{*/

      // - log message -
      log_info_2("channel server %u, unknown request",a_index);
    }/*}}}*/
  }

  return 0;
}/*}}}*/

int wd_daemon_s_watchdog_time_event(void *a_wd_daemon,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(WD_DAEMON_TIMER_READ_ERROR);
  }

  wd_daemon_s *this = (wd_daemon_s *)a_wd_daemon;

  // - all monitors are ok -
  if (this->wdg_fd != -1 && this->monitors_ok)
  {
    // - refresh timer driver -
    if (fd_s_write(&this->wdg_fd,".",1))
    {
      throw_error(WD_DAEMON_WATCHDOG_WRITE_ERROR);
    }
  }

  return 0;
}/*}}}*/

int wd_daemon_s_monitor_time_event(void *a_wd_daemon,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(WD_DAEMON_TIMER_READ_ERROR);
  }

  wd_daemon_s *this = (wd_daemon_s *)a_wd_daemon;
  wd_monitor_s *monitor = wd_monitor_tree_s_at(&this->monitor_tree,a_index);

  // - log message -
  log_info_2("monitor TIMEOUT, name: %s",monitor->name.data);

  // - monitors were ok -
  if (this->monitors_ok)
  {
    if (this->channel_watches.used != 0)
    {
      CONT_INIT_CLEAR(ui_array_s,indexes);

      unsigned index = 0;
      do
      {
        if (this->channel_watches.data[index])
        {
          ui_array_s_push(&indexes,index);
        }
      } while(++index < this->channel_watches.used);

      if (indexes.used != 0)
      {
        this->buffer.used = 0;
        bc_array_s_append_ptr(&this->buffer,"{\"type\":\"update\",\"reason\":\"timeout\",\"id\":0}");

        // - send update message -
        if (wd_channel_s_send_multi_message(&this->channel,&indexes,&this->buffer))
        {
          throw_error(WD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
        }
      }
    }
  }

  // - reset monitors ok flag -
  this->monitors_ok = 0;

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
  libchannel_cll_init();
  libchannel_wdl_init();
  libconf_wdl_init();

  char *name = "wd_daemon";
  char *conf = "wd_config.json";

  // - process command line arguments -
  if (argc > 1)
  {
    int arg_idx = 1;
    do {
      if (strncmp("--name=",argv[arg_idx],7) == 0)
      {
        name = argv[arg_idx] + 7;
      }
      else if (strncmp("--conf=",argv[arg_idx],7) == 0)
      {
        conf = argv[arg_idx] + 7;
      }
      else
      {
        cassert(0);
      }
    } while(++arg_idx < argc);
  }

  // - create process directories -
  cassert(system("mkdir -p -m 777 " PROCESS_RUN_DIR_PATH " " PROCESS_LOG_DIR_PATH) == 0); // NOLINT

  // - create process -
  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(process_s,process);
    cassert(process_s_create(&process,name) == 0);

    do {
      CONT_INIT_CLEAR(wd_daemon_s,daemon);

      if (wd_daemon_s_create(&daemon) ||
          wd_config_s_read_file(&daemon.config,conf))
      {
        break;
      }

      // - set configuration changed flag -
      daemon.config_changed_flag = 1;

      if (wd_daemon_s_run(&daemon))
      {
        // - ignore return value, always terminate -
      }

    } while(0);
  }

  libconf_wdl_clear();
  libchannel_wdl_clear();
  libchannel_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

