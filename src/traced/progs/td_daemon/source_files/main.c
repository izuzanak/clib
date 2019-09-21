
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === methods of generated structures =========================================

// -- td_trace_s --
@begin
methods td_trace_s
@end

void td_trace_s_read_to_message(td_trace_s *this,lli a_record_id,bc_array_s *a_trg)
{/*{{{*/
  unsigned record_id_used = a_trg->used;
  bc_array_s_push_blanks(a_trg,sizeof(lli) + sizeof(ulli));

  // - read record from trace -
  time_s time;
  if (trace_s_read_record(&this->trace,a_record_id,&time,a_trg))
  {
    // - read error -
    a_trg->used = record_id_used;
    bc_array_s_append_be_lli(a_trg,-1);
    bc_array_s_append_be_ulli(a_trg,0);
  }
  else
  {
    unsigned buffer_used = a_trg->used;

    a_trg->used = record_id_used;
    bc_array_s_append_be_lli(a_trg,a_record_id);
    bc_array_s_append_be_ulli(a_trg,time);

    a_trg->used = buffer_used;
  }
}/*}}}*/

// -- td_trace_tree_s --
@begin
methods td_trace_tree_s
@end

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

  // - if traces configuration changed -
  if (!td_conf_trace_tree_s_compare(&this->config.traces,&this->last_config.traces))
  {
    // - update traces -
    if (td_daemon_s_update_traces(this))
    {
      throw_error(TD_DAEMON_CONFIG_DATA_ERROR);
    }
  }

  // - if communication channel configuration changed -
  if (!td_conf_ip_port_s_compare(&this->config.channel,&this->last_config.channel))
  {
    td_conf_ip_port_s *channel_cfg = &this->config.channel;

    // - create communication channel -
    if (td_channel_s_create(&this->channel,channel_cfg->ip.data,channel_cfg->port,&this->epoll,
          td_daemon_s_channel_callback,
          this))
    {
      throw_error(TD_DAEMON_CHANNEL_CREATE_ERROR);
    }
  }

  // - update last configuration -
  td_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int td_daemon_s_update_traces(td_daemon_s *this)
{/*{{{*/
  
  // - log message -
  log_info_2("td_daemon_s_update_traces");

  // - remove traces not present in configuration -
  if (this->traces.root_idx != c_idx_not_exist)
  {
    td_trace_tree_s_node *tttn_ptr = this->traces.data;
    td_trace_tree_s_node *tttn_ptr_end = tttn_ptr + this->traces.used;
    do {
      if (tttn_ptr->valid)
      {
        td_trace_s *trace = &tttn_ptr->object;

        // - retrieve trace configuration index -
        unsigned trace_conf_idx = td_conf_trace_tree_s_get_idx(&this->config.traces,&trace->config);

        // - remove trace not present in configuration -
        if (trace_conf_idx == c_idx_not_exist)
        {
          // - log message -
          log_info_2("remove trace %s",trace->config.trace_id.data);

          td_trace_s_clear(trace);
          td_trace_tree_s_remove(&this->traces,tttn_ptr - this->traces.data);
        }
      }
    } while(++tttn_ptr < tttn_ptr_end);
  }

  // - add or update traces according to configuration -
  if (this->config.traces.root_idx != c_idx_not_exist)
  {
    td_conf_trace_tree_s_node *tcttn_ptr = this->config.traces.data;
    td_conf_trace_tree_s_node *tcttn_ptr_end = tcttn_ptr + this->config.traces.used;
    do {
      if (tcttn_ptr->valid)
      {
        td_conf_trace_s *trace_config = &tcttn_ptr->object;

        // - trace to create or update -
        td_trace_s *trace = NULL;

        td_trace_s search_trace = {*trace_config,};
        unsigned trace_idx = td_trace_tree_s_get_idx(&this->traces,&search_trace);

        if (trace_idx == c_idx_not_exist)
        {
          CONT_INIT_CLEAR(td_trace_s,insert_trace);
          td_conf_trace_s_copy(&insert_trace.config,trace_config);

          trace_idx = td_trace_tree_s_swap_insert(&this->traces,&insert_trace);
          trace = td_trace_tree_s_at(&this->traces,trace_idx);
        }
        else
        {
          trace = td_trace_tree_s_at(&this->traces,trace_idx);

          // - trace configuration without change -
          if (td_conf_trace_s_compare(&trace->config,trace_config))
          {
            trace = NULL;
          }
          else
          {
            td_trace_s_clear(trace);
            td_conf_trace_s_copy(&trace->config,trace_config);
          }
        }

        // - create or update trace -
        if (trace != NULL)
        {
          // - log message -
          log_info_2("create trace %s",trace->config.trace_id.data);

          // - create trace memory maps -
          if (td_trace_s_mmap_file(&trace->mmap_header,&trace->config.header) ||
              td_trace_s_mmap_file(&trace->mmap_trace,&trace->config.trace) ||
              (trace->config.timestamp_div > 0 ? td_trace_s_mmap_file(&trace->mmap_timestamp,&trace->config.timestamp) : 0))
          {
            throw_error(TD_DAEMON_TRACE_FILE_MMAP_ERROR);
          }

          // - create trace -
          if (trace_s_create(&trace->trace,
                trace->mmap_header.address,trace->config.header.size,
                trace->mmap_trace.address,trace->config.trace.size,
                trace->mmap_timestamp.address,trace->config.timestamp.size,
                trace->config.record.size,
                trace->config.timestamp_div))
          {
            throw_error(TD_DAEMON_TRACE_CREATE_ERROR);
          }
        }
      }
    } while(++tcttn_ptr < tcttn_ptr_end);
  }

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

int td_daemon_s_channel_callback(void *a_td_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  td_daemon_s *this = (td_daemon_s *)a_td_daemon;

  switch (a_type)
  {
  case td_channel_cbreq_NEW:
    {/*{{{*/
    }/*}}}*/
    break;
  case td_channel_cbreq_DROP:
    {/*{{{*/
    }/*}}}*/
    break;
  case td_channel_cbreq_TRACE_INFO:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);

      td_trace_s search_trace = {{*trace_id,},};
      unsigned trace_idx = td_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(TD_DAEMON_TRACE_NOT_EXIST);
      }

      td_trace_s *trace = td_trace_tree_s_at(&this->traces,trace_idx);

      this->buffer.used = 0;
      bc_array_s_append_td_traced_msg_header(&this->buffer,id,td_channel_msg_type_RESPONSE,a_type,trace_id);

      bc_array_s_append_be_lli(&this->buffer,trace_s_head(&trace->trace));
      bc_array_s_append_be_lli(&this->buffer,trace_s_tail(&trace->trace));

      if (td_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(TD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case td_channel_cbreq_TRACE_WRITE:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);
      time_s time = va_arg(a_ap,time_s);
      const bc_array_s *data = va_arg(a_ap,const bc_array_s *);

      td_trace_s search_trace = {{*trace_id,},};
      unsigned trace_idx = td_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(TD_DAEMON_TRACE_NOT_EXIST);
      }

      td_trace_s *trace = td_trace_tree_s_at(&this->traces,trace_idx);

      // - check write data size -
      if (data->used >= trace->trace.data_size)
      {
        throw_error(TD_DAEMON_TRACE_INVALID_WRITE_SIZE);
      }

      // - write record to trace -
      if (trace_s_write_record(&trace->trace,time,data->used,data->data))
      {
        throw_error(TD_DAEMON_TRACE_WRITE_ERROR);
      }

      this->buffer.used = 0;
      bc_array_s_append_td_traced_msg_header(&this->buffer,id,td_channel_msg_type_RESPONSE,a_type,trace_id);

      bc_array_s_append_be_lli(&this->buffer,trace_s_head(&trace->trace));

      if (td_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(TD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case td_channel_cbreq_TRACE_READ:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);
      lli record_id = va_arg(a_ap,lli);

      td_trace_s search_trace = {{*trace_id,},};
      unsigned trace_idx = td_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(TD_DAEMON_TRACE_NOT_EXIST);
      }

      td_trace_s *trace = td_trace_tree_s_at(&this->traces,trace_idx);

      this->buffer.used = 0;
      bc_array_s_append_td_traced_msg_header(&this->buffer,id,td_channel_msg_type_RESPONSE,a_type,trace_id);

      // - read trace record to message -
      td_trace_s_read_to_message(trace,record_id,&this->buffer);

      if (td_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(TD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case td_channel_cbreq_TRACE_HEAD:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);

      td_trace_s search_trace = {{*trace_id,},};
      unsigned trace_idx = td_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(TD_DAEMON_TRACE_NOT_EXIST);
      }

      td_trace_s *trace = td_trace_tree_s_at(&this->traces,trace_idx);
      lli record_id = trace_s_head(&trace->trace);

      this->buffer.used = 0;
      bc_array_s_append_td_traced_msg_header(&this->buffer,id,td_channel_msg_type_RESPONSE,a_type,trace_id);

      // - read trace record to message -
      td_trace_s_read_to_message(trace,record_id,&this->buffer);

      if (td_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(TD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case td_channel_cbreq_TRACE_TAIL:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);

      td_trace_s search_trace = {{*trace_id,},};
      unsigned trace_idx = td_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(TD_DAEMON_TRACE_NOT_EXIST);
      }

      td_trace_s *trace = td_trace_tree_s_at(&this->traces,trace_idx);
      lli record_id = trace_s_tail(&trace->trace);

      this->buffer.used = 0;
      bc_array_s_append_td_traced_msg_header(&this->buffer,id,td_channel_msg_type_RESPONSE,a_type,trace_id);

      // - read trace record to message -
      td_trace_s_read_to_message(trace,record_id,&this->buffer);

      if (td_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(TD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
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

