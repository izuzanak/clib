
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === methods of generated structures =========================================

// -- sd_segment_tree_s --
@begin
methods sd_segment_tree_s
@end

// -- sd_trace_tree_s --
@begin
methods sd_trace_tree_s
@end

// -- sd_daemon_s --
@begin
methods sd_daemon_s
@end

int sd_daemon_s_create(sd_daemon_s *this)
{/*{{{*/
  sd_daemon_s_clear(this);

  // - reset configuration changed flag -
  this->config_changed_flag = 0;

  epoll_s_create(&this->epoll,0);

  return 0;
}/*}}}*/

int sd_daemon_s_process_config(sd_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("process configuration");

  // - if segments configuration changed -
  if (!sd_conf_segment_tree_s_compare(&this->config.segments,&this->last_config.segments))
  {
    // - update segments -
    if (sd_daemon_s_update_segments(this))
    {
      throw_error(SD_DAEMON_CONFIG_DATA_ERROR);
    }
  }

  // - if traces configuration changed -
  if (!sd_conf_trace_tree_s_compare(&this->config.traces,&this->last_config.traces))
  {
    // - update traces -
    if (sd_daemon_s_update_traces(this))
    {
      throw_error(SD_DAEMON_CONFIG_DATA_ERROR);
    }
  }

  // - if communication channel configuration changed -
  if (!sd_conf_ip_port_s_compare(&this->config.channel,&this->last_config.channel))
  {
    sd_conf_ip_port_s *channel_cfg = &this->config.channel;

    // - create communication channel -
    if (sd_channel_s_create(&this->channel,channel_cfg->ip.data,channel_cfg->port,&this->epoll,
          sd_daemon_s_channel_callback,
          this))
    {
      throw_error(SD_DAEMON_CHANNEL_CREATE_ERROR);
    }
  }

  // - update last configuration -
  sd_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int sd_daemon_s_update_segments(sd_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("sd_daemon_s_update_segments");

  // - remove segments not present in configuration -
  if (this->segments.root_idx != c_idx_not_exist)
  {
    sd_segment_tree_s_node *sstn_ptr = this->segments.data;
    sd_segment_tree_s_node *sstn_ptr_end = sstn_ptr + this->segments.used;
    do {
      if (sstn_ptr->valid)
      {
        sd_segment_descr_s *segment = &sstn_ptr->object;

        // - retrieve segment configuration index -
        unsigned segment_conf_idx = sd_conf_segment_tree_s_get_idx(&this->config.segments,&segment->config);

        // - remove segment not present in configuration -
        if (segment_conf_idx == c_idx_not_exist)
        {
          // - log message -
          log_info_2("remove segment %s",segment->config.segment_id.data);

          sd_segment_descr_s_clear(segment);
          sd_segment_tree_s_remove(&this->segments,sstn_ptr - this->segments.data);
        }
      }
    } while(++sstn_ptr < sstn_ptr_end);
  }

  // - add or update segments according to configuration -
  if (this->config.segments.root_idx != c_idx_not_exist)
  {
    sd_conf_segment_tree_s_node *scstn_ptr = this->config.segments.data;
    sd_conf_segment_tree_s_node *scstn_ptr_end = scstn_ptr + this->config.segments.used;
    do {
      if (scstn_ptr->valid)
      {
        sd_conf_segment_s *segment_config = &scstn_ptr->object;

        // - segment to create or update -
        sd_segment_descr_s *segment = NULL;

        sd_segment_descr_s search_segment = {*segment_config,};
        unsigned segment_idx = sd_segment_tree_s_get_idx(&this->segments,&search_segment);

        if (segment_idx == c_idx_not_exist)
        {
          // - create new segment -
          CONT_INIT_CLEAR(sd_segment_descr_s,insert_segment);
          if (sd_segment_descr_s_create(&insert_segment,segment_config))
          {
            throw_error(SD_DAEMON_SEGMENT_CREATE_ERROR);
          }

          segment_idx = sd_segment_tree_s_swap_insert(&this->segments,&insert_segment);
          segment = sd_segment_tree_s_at(&this->segments,segment_idx);
        }
        else
        {
          segment = sd_segment_tree_s_at(&this->segments,segment_idx);

          // - segment configuration without change -
          if (sd_conf_segment_s_compare(&segment->config,segment_config))
          {
            segment = NULL;
          }
          else
          {
            // - update old segment -
            if (sd_segment_descr_s_create(segment,segment_config))
            {
              throw_error(SD_DAEMON_SEGMENT_CREATE_ERROR);
            }
          }
        }
      }
    } while(++scstn_ptr < scstn_ptr_end);
  }

  return 0;
}/*}}}*/

int sd_daemon_s_update_traces(sd_daemon_s *this)
{/*{{{*/
  
  // - log message -
  log_info_2("sd_daemon_s_update_traces");

  // - remove traces not present in configuration -
  if (this->traces.root_idx != c_idx_not_exist)
  {
    sd_trace_tree_s_node *sttn_ptr = this->traces.data;
    sd_trace_tree_s_node *sttn_ptr_end = sttn_ptr + this->traces.used;
    do {
      if (sttn_ptr->valid)
      {
        sd_trace_descr_s *trace_descr = &sttn_ptr->object;

        // - retrieve trace configuration index -
        unsigned trace_conf_idx = sd_conf_trace_tree_s_get_idx(&this->config.traces,&trace_descr->config);

        // - remove trace not present in configuration -
        if (trace_conf_idx == c_idx_not_exist)
        {
          // - log message -
          log_info_2("remove trace %s",trace_descr->config.trace_id.data);

          sd_trace_descr_s_clear(trace_descr);
          sd_trace_tree_s_remove(&this->traces,sttn_ptr - this->traces.data);
        }
      }
    } while(++sttn_ptr < sttn_ptr_end);
  }

  // - add or update traces according to configuration -
  if (this->config.traces.root_idx != c_idx_not_exist)
  {
    sd_conf_trace_tree_s_node *scttn_ptr = this->config.traces.data;
    sd_conf_trace_tree_s_node *tcttn_ptr_end = scttn_ptr + this->config.traces.used;
    do {
      if (scttn_ptr->valid)
      {
        sd_conf_trace_s *trace_config = &scttn_ptr->object;

        // - trace to create or update -
        sd_trace_descr_s *trace = NULL;

        sd_trace_descr_s search_trace = {*trace_config,};
        unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

        if (trace_idx == c_idx_not_exist)
        {
          CONT_INIT_CLEAR(sd_trace_descr_s,insert_trace);

          // - create new trace -
          if (sd_trace_descr_s_create(&insert_trace,trace_config))
          {
            throw_error(SD_DAEMON_TRACE_CREATE_ERROR);
          }

          sd_conf_trace_s_copy(&insert_trace.config,trace_config);

          trace_idx = sd_trace_tree_s_swap_insert(&this->traces,&insert_trace);
          trace = sd_trace_tree_s_at(&this->traces,trace_idx);
        }
        else
        {
          trace = sd_trace_tree_s_at(&this->traces,trace_idx);

          // - trace configuration without change -
          if (sd_conf_trace_s_compare(&trace->config,trace_config))
          {
            trace = NULL;
          }
          else
          {
            // - update old trace -
            if (sd_trace_descr_s_create(trace,trace_config))
            {
              throw_error(SD_DAEMON_TRACE_CREATE_ERROR);
            }
          }
        }
      }
    } while(++scttn_ptr < tcttn_ptr_end);
  }

  return 0;
}/*}}}*/

int sd_daemon_s_run(sd_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_0("running");

  while(!g_terminate)
  {
    if (this->config_changed_flag)
    {
      if (sd_daemon_s_process_config(this))
      {
        throw_error(SD_DAEMON_CONFIG_DATA_ERROR);
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
        throw_error(SD_DAEMON_EPOLL_WAIT_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int sd_daemon_s_channel_callback(void *a_sd_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  sd_daemon_s *this = (sd_daemon_s *)a_sd_daemon;

  switch (a_type)
  {
  case sd_channel_cbreq_NEW:
    {/*{{{*/
    }/*}}}*/
    break;
  case sd_channel_cbreq_DROP:
    {/*{{{*/
    }/*}}}*/
    break;
  case sd_channel_cbreq_SEGMENT_INFO:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *segment_id = va_arg(a_ap,const string_s *);

      sd_segment_descr_s search_segment = {{*segment_id,},};
      unsigned segment_idx = sd_segment_tree_s_get_idx(&this->segments,&search_segment);

      // - check if segment exists -
      if (segment_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_SEGMENT_NOT_EXIST);
      }

      sd_segment_descr_s *segment = sd_segment_tree_s_at(&this->segments,segment_idx);

      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,segment_id);

      bc_array_s_append_be_ui(&this->buffer,segment->config.size);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_SEGMENT_WRITE:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *segment_id = va_arg(a_ap,const string_s *);
      time_s time = va_arg(a_ap,time_s);
      const bc_array_s *data = va_arg(a_ap,const bc_array_s *);

      sd_segment_descr_s search_segment = {{*segment_id,},};
      unsigned segment_idx = sd_segment_tree_s_get_idx(&this->segments,&search_segment);

      // - check if segment exists -
      if (segment_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_SEGMENT_NOT_EXIST);
      }

      sd_segment_descr_s *segment = sd_segment_tree_s_at(&this->segments,segment_idx);

      // - check write data size -
      if (data->used >= segment->config.size)
      {
        throw_error(SD_DAEMON_SEGMENT_INVALID_WRITE_SIZE);
      }

      // - write record to segment -
      if (sd_segment_handle_s_write_record(&segment->handle,time,data->used,data->data))
      {
        throw_error(SD_DAEMON_SEGMENT_WRITE_ERROR);
      }

      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,segment_id);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_SEGMENT_READ:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *segment_id = va_arg(a_ap,const string_s *);

      sd_segment_descr_s search_segment = {{*segment_id,},};
      unsigned segment_idx = sd_segment_tree_s_get_idx(&this->segments,&search_segment);

      // - check if segment exists -
      if (segment_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_SEGMENT_NOT_EXIST);
      }

      sd_segment_descr_s *segment = sd_segment_tree_s_at(&this->segments,segment_idx);

      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,segment_id);

      // - get record from trace -
      time_s time;
      bc_array_s record;
      if (sd_segment_handle_s_get_record(&segment->handle,&time,&record))
      {
        // - read error -
        bc_array_s_append_be_ulli(&this->buffer,0);
      }
      else
      {
        bc_array_s_append_be_ulli(&this->buffer,time);
        bc_array_s_append(&this->buffer,record.used,record.data);
      }

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_INFO:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);

      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      bc_array_s_append_be_ui(&this->buffer,trace->config.record.size);
      bc_array_s_append_be_lli(&this->buffer,sd_trace_s_head(&trace->trace));
      bc_array_s_append_be_lli(&this->buffer,sd_trace_s_tail(&trace->trace));

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_WRITE:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);
      time_s time = va_arg(a_ap,time_s);
      const bc_array_s *data = va_arg(a_ap,const bc_array_s *);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);

      // - check write data size -
      if (data->used >= trace->trace.data_size)
      {
        throw_error(SD_DAEMON_TRACE_INVALID_WRITE_SIZE);
      }

      // - write record to trace -
      if (sd_trace_s_write_record(&trace->trace,time,data->used,data->data))
      {
        throw_error(SD_DAEMON_TRACE_WRITE_ERROR);
      }

      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      bc_array_s_append_be_lli(&this->buffer,sd_trace_s_head(&trace->trace));

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_READ:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);
      lli record_id = va_arg(a_ap,lli);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);

      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      // - read trace record to message -
      sd_trace_descr_s_read_to_message(trace,record_id,&this->buffer);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_HEAD:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);
      lli record_id = sd_trace_s_head(&trace->trace);

      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      // - read trace record to message -
      sd_trace_descr_s_read_to_message(trace,record_id,&this->buffer);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_TAIL:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);
      lli record_id = sd_trace_s_tail(&trace->trace);

      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      // - read trace record to message -
      sd_trace_descr_s_read_to_message(trace,record_id,&this->buffer);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
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
  libconf_sdl_init();
  librecord_sdl_init();

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(process_s,process);
    cassert(process_s_create(&process,"sd_daemon") == 0);

    do {
      CONT_INIT_CLEAR(sd_daemon_s,daemon);

      if (sd_daemon_s_create(&daemon) ||
          sd_config_s_read_file(&daemon.config,SD_JSON_CONFIG_FILE))
      {
        break;
      }

      // - set configuration changed flag -
      daemon.config_changed_flag = 1;

      if (sd_daemon_s_run(&daemon)) {}
    } while(0);
  }

  librecord_sdl_clear();
  libconf_sdl_clear();
  libchannel_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

