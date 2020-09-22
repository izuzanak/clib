
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- sd_segment_tree_s --
@begin
methods sd_segment_tree_s
@end

// -- sd_trace_tree_s --
@begin
methods sd_trace_tree_s
@end

// -- id_tree_s --
@begin
methods id_tree_s
@end

// -- sd_channel_watch_s --
@begin
methods sd_channel_watch_s
@end

// -- sd_channel_watches_s --
@begin
methods sd_channel_watches_s
@end

// -- sd_daemon_s --
@begin
methods sd_daemon_s
@end

int sd_daemon_s_create(sd_daemon_s *this)
{/*{{{*/
  sd_daemon_s_clear(this);

  // - reset update watches -
  this->update_watches = 0;

  return 0;
}/*}}}*/

int sd_daemon_s_process_config(sd_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("process configuration");

  // - if communication channel configuration changed -
  if (!sd_conf_ip_port_s_compare(&this->config.channel,&this->last_config.channel))
  {
    sd_conf_ip_port_s *channel_cfg = &this->config.channel;

    // - create communication channel -
    if (sd_channel_s_create(&this->channel,channel_cfg->ip.data,channel_cfg->port,
          sd_daemon_s_channel_callback,
          this))
    {
      throw_error(SD_DAEMON_CHANNEL_CREATE_ERROR);
    }
  }

  CONT_INIT_CLEAR(ui_array_s,segments_updated);

  // - if segments configuration changed -
  if (!sd_conf_segment_tree_s_compare(&this->config.segments,&this->last_config.segments))
  {
    // - update segments -
    if (sd_daemon_s_update_segments(this,&segments_updated))
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

  // - some segments were updated -
  if (segments_updated.used != 0)
  {
    // - update watches -
    this->update_watches = 1;
    sd_daemon_s_update_watches(this);

    unsigned *usi_ptr = segments_updated.data;
    unsigned *usi_ptr_end = usi_ptr + segments_updated.used;
    do {
      sd_segment_descr_s *segment = sd_segment_tree_s_at(&this->segments,*usi_ptr);

      // - some channels are watching segment -
      if (segment->channel_idxs.used != 0)
      {
        this->buffer.used = 0;
        bc_array_s_append_sd_segmentd_msg_header(&this->buffer,0,
            sd_channel_msg_type_EVENT,sd_channel_cbreq_SEGMENT_UPDATE,&segment->config.segment_id);

        // - get record from segment -
        time_s time;
        bc_array_s data;
        if (sd_segment_handle_s_get_record(&segment->handle,&time,&data))
        {
          // - read error -
          bc_array_s_append_be_ulli(&this->buffer,0);
        }
        else
        {
          bc_array_s_append_be_ulli(&this->buffer,time);
          bc_array_s_append(&this->buffer,data.used,data.data);
        }

        if (sd_channel_s_send_multi_message(&this->channel,&segment->channel_idxs,&this->buffer))
        {
          throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
        }
      }
    } while(++usi_ptr < usi_ptr_end);
  }

  // - update last configuration -
  sd_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int sd_daemon_s_update_segments(sd_daemon_s *this,ui_array_s *a_updated)
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
          sd_segment_tree_s_at(&this->segments,segment_idx);

          // - store updated segment -
          ui_array_s_push(a_updated,segment_idx);
        }
        else
        {
          segment = sd_segment_tree_s_at(&this->segments,segment_idx);

          // - segment configuration changed -
          if (!sd_conf_segment_s_compare(&segment->config,segment_config))
          {
            // - update old segment -
            if (sd_segment_descr_s_create(segment,segment_config))
            {
              throw_error(SD_DAEMON_SEGMENT_CREATE_ERROR);
            }

            // - store updated segment -
            ui_array_s_push(a_updated,segment_idx);
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
          sd_trace_tree_s_at(&this->traces,trace_idx);

          // - set update watches -
          this->update_watches = 1;
        }
        else
        {
          trace = sd_trace_tree_s_at(&this->traces,trace_idx);

          // - trace configuration changed -
          if (!sd_conf_trace_s_compare(&trace->config,trace_config))
          {
            // - update old trace -
            if (sd_trace_descr_s_create(trace,trace_config))
            {
              throw_error(SD_DAEMON_TRACE_CREATE_ERROR);
            }

            // - set update watches -
            this->update_watches = 1;
          }
        }
      }
    } while(++scttn_ptr < tcttn_ptr_end);
  }

  return 0;
}/*}}}*/

void sd_daemon_s_do_update_watches(sd_daemon_s *this)
{/*{{{*/

  // - reset segments channel indexes -
  if (this->segments.root_idx != c_idx_not_exist)
  {
    sd_segment_tree_s_node *sstn_ptr = this->segments.data;
    sd_segment_tree_s_node *sstn_ptr_end = sstn_ptr + this->segments.used;
    do {
      if (sstn_ptr->valid)
      {
        sstn_ptr->object.channel_idxs.used = 0;
      }
    } while(++sstn_ptr < sstn_ptr_end);
  }

  // - reset traces channel indexes -
  if (this->traces.root_idx != c_idx_not_exist)
  {
    sd_trace_tree_s_node *sttn_ptr = this->traces.data;
    sd_trace_tree_s_node *sttn_ptr_end = sttn_ptr + this->traces.used;
    do {
      if (sttn_ptr->valid)
      {
        sttn_ptr->object.channel_idxs.used = 0;
      }
    } while(++sttn_ptr < sttn_ptr_end);
  }

  // - process channel watches -
  if (this->channel_watches.used != 0)
  {
    unsigned cw_idx = 0;
    do {
      sd_channel_watch_s *channel_watch = sd_channel_watches_s_at(&this->channel_watches,cw_idx);

      // - process segment indexes -
      if (channel_watch->segment_ids.root_idx != c_idx_not_exist)
      {
        id_tree_s_node *stn_ptr = channel_watch->segment_ids.data;
        id_tree_s_node *stn_ptr_end = stn_ptr + channel_watch->segment_ids.used;
        do {
          if (stn_ptr->valid)
          {
            sd_segment_descr_s search_segment = {{stn_ptr->object,},};
            unsigned segment_idx = sd_segment_tree_s_get_idx(&this->segments,&search_segment);

            // - segment exists -
            if (segment_idx != c_idx_not_exist)
            {
              ui_array_s_push(&sd_segment_tree_s_at(&this->segments,segment_idx)->channel_idxs,cw_idx);
            }
          }
        } while(++stn_ptr < stn_ptr_end);
      }

      // - process trace indexes -
      if (channel_watch->trace_ids.root_idx != c_idx_not_exist)
      {
        id_tree_s_node *stn_ptr = channel_watch->trace_ids.data;
        id_tree_s_node *stn_ptr_end = stn_ptr + channel_watch->trace_ids.used;
        do {
          if (stn_ptr->valid)
          {
            sd_trace_descr_s search_trace = {{stn_ptr->object,},};
            unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

            // - trace exists -
            if (trace_idx != c_idx_not_exist)
            {
              ui_array_s_push(&sd_trace_tree_s_at(&this->traces,trace_idx)->channel_idxs,cw_idx);
            }
          }
        } while(++stn_ptr < stn_ptr_end);
      }
    } while(++cw_idx < this->channel_watches.used);
  }

  // - reset update watches -
  this->update_watches = 0;
}/*}}}*/

int sd_daemon_s_run(sd_daemon_s *this)
{/*{{{*/
  (void)this;

  // - log message -
  log_info_0("running");

  while(!g_terminate)
  {
    // - wait on events -
    int err;
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(SD_DAEMON_EPOLL_WAIT_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

// debug output
//void sd_debug_print(sd_daemon_s *this)
//{/*{{{*/
//  sd_daemon_s_update_watches(this);
//
//  if (this->segments.root_idx != c_idx_not_exist)
//  {
//    sd_segment_tree_s_node *stn_ptr = this->segments.data;
//    sd_segment_tree_s_node *stn_ptr_end = stn_ptr + this->segments.used;
//    do {
//      if (stn_ptr->valid)
//      {
//        fprintf(stderr,"segment %s: ",stn_ptr->object.config.segment_id.data);
//        DEBUG_PRINT(ui_array_s,&stn_ptr->object.channel_idxs);
//      }
//    } while(++stn_ptr < stn_ptr_end);
//  }
//
//  if (this->traces.root_idx != c_idx_not_exist)
//  {
//    sd_trace_tree_s_node *ttn_ptr = this->traces.data;
//    sd_trace_tree_s_node *ttn_ptr_end = ttn_ptr + this->traces.used;
//    do {
//      if (ttn_ptr->valid)
//      {
//        fprintf(stderr,"trace %s: ",ttn_ptr->object.config.trace_id.data);
//        DEBUG_PRINT(ui_array_s,&ttn_ptr->object.channel_idxs);
//      }
//    } while(++ttn_ptr < ttn_ptr_end);
//  }
//}/*}}}*/

int sd_daemon_s_channel_callback(void *a_sd_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  sd_daemon_s *this = (sd_daemon_s *)a_sd_daemon;

  switch (a_type)
  {
  case sd_channel_cbreq_NEW:
    {/*{{{*/
      while (a_index >= this->channel_watches.used)
      {
        sd_channel_watches_s_push_blank(&this->channel_watches);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_DROP:
    {/*{{{*/
      sd_channel_watch_s *channel_watch = sd_channel_watches_s_at(&this->channel_watches,a_index);

      // - channel watch is not empty -
      if (channel_watch->segment_ids.root_idx != c_idx_not_exist ||
          channel_watch->trace_ids.root_idx != c_idx_not_exist)
      {
        sd_channel_watch_s_clear(channel_watch);

        // - set update watches -
        this->update_watches = 1;
      }

      // debug output
      //sd_debug_print(this);
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

      // - send response -
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

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,segment_id);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      // - some channels are watching segment -
      sd_daemon_s_update_watches(this);
      if (segment->channel_idxs.used != 0)
      {
        this->buffer.used = 0;
        bc_array_s_append_sd_segmentd_msg_header(&this->buffer,0,
            sd_channel_msg_type_EVENT,sd_channel_cbreq_SEGMENT_UPDATE,segment_id);

        bc_array_s_append_be_ulli(&this->buffer,time);
        bc_array_s_append(&this->buffer,data->used,data->data);

        if (sd_channel_s_send_multi_message(&this->channel,&segment->channel_idxs,&this->buffer))
        {
          throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
        }
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

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,segment_id);

      // - get record from segment -
      time_s time;
      bc_array_s data;
      if (sd_segment_handle_s_get_record(&segment->handle,&time,&data))
      {
        // - read error -
        bc_array_s_append_be_ulli(&this->buffer,0);
      }
      else
      {
        bc_array_s_append_be_ulli(&this->buffer,time);
        bc_array_s_append(&this->buffer,data.used,data.data);
      }

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_SEGMENT_WATCH:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *segment_id = va_arg(a_ap,const string_s *);

      sd_segment_descr_s search_segment = {{*segment_id,},};
      unsigned segment_idx = sd_segment_tree_s_get_idx(&this->segments,&search_segment);

      sd_channel_watch_s *channel_watch = sd_channel_watches_s_at(&this->channel_watches,a_index);
      unsigned watch_idx = id_tree_s_get_idx(&channel_watch->segment_ids,segment_id);

      // - insert watch -
      if (watch_idx == c_idx_not_exist)
      {
        id_tree_s_insert(&channel_watch->segment_ids,segment_id);

        // - set update watches -
        this->update_watches = 1;
      }

      // debug output
      //sd_debug_print(this);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,segment_id);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      // - segment exists -
      if (segment_idx != c_idx_not_exist)
      {
        sd_segment_descr_s *segment = sd_segment_tree_s_at(&this->segments,segment_idx);

        // - send update -
        this->buffer.used = 0;
        bc_array_s_append_sd_segmentd_msg_header(&this->buffer,0,
            sd_channel_msg_type_EVENT,sd_channel_cbreq_SEGMENT_UPDATE,segment_id);

        // - get record from segment -
        time_s time;
        bc_array_s data;
        if (sd_segment_handle_s_get_record(&segment->handle,&time,&data))
        {
          // - read error -
          bc_array_s_append_be_ulli(&this->buffer,0);
        }
        else
        {
          bc_array_s_append_be_ulli(&this->buffer,time);
          bc_array_s_append(&this->buffer,data.used,data.data);
        }

        if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
        {
          throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
        }
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_SEGMENT_IGNORE:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *segment_id = va_arg(a_ap,const string_s *);

      sd_channel_watch_s *channel_watch = sd_channel_watches_s_at(&this->channel_watches,a_index);
      unsigned watch_idx = id_tree_s_get_idx(&channel_watch->segment_ids,segment_id);

      // - remove watch -
      if (watch_idx != c_idx_not_exist && a_type == sd_channel_cbreq_SEGMENT_IGNORE)
      {
        id_tree_s_remove(&channel_watch->segment_ids,watch_idx);

        // - set update watches -
        this->update_watches = 1;
      }

      // debug output
      //sd_debug_print(this);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,segment_id);

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

      // - send response -
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

      lli record_id = sd_trace_s_head(&trace->trace);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      bc_array_s_append_be_lli(&this->buffer,record_id);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      // - some channels are watching trace -
      sd_daemon_s_update_watches(this);
      if (trace->channel_idxs.used != 0)
      {
        this->buffer.used = 0;
        bc_array_s_append_sd_segmentd_msg_header(&this->buffer,0,
            sd_channel_msg_type_EVENT,sd_channel_cbreq_TRACE_UPDATE,trace_id);

        bc_array_s_append_be_lli(&this->buffer,record_id);
        bc_array_s_append_be_ulli(&this->buffer,time);
        bc_array_s_append(&this->buffer,data->used,data->data);

        if (sd_channel_s_send_multi_message(&this->channel,&trace->channel_idxs,&this->buffer))
        {
          throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
        }
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

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      sd_trace_descr_s_read_to_message(trace,record_id,&this->buffer);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_HEAD:
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
      lli record_id = a_type == sd_channel_cbreq_TRACE_HEAD ?
        sd_trace_s_head(&trace->trace) : sd_trace_s_tail(&trace->trace);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      sd_trace_descr_s_read_to_message(trace,record_id,&this->buffer);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_RANGE:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);
      lli first_id = va_arg(a_ap,lli);
      lli last_id = va_arg(a_ap,lli);
      lli count = va_arg(a_ap,lli);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);

      lli trace_head = sd_trace_s_head(&trace->trace);
      lli trace_tail = sd_trace_s_tail(&trace->trace);

      if (first_id < trace_tail)
      {
        first_id = trace_tail;
      }

      if (last_id < 0 || last_id > trace_head)
      {
        last_id = trace_head;
      }

      lli range_count = (trace_head != -1 && first_id <= last_id) ? last_id - first_id + 1 : 0;
      lli resp_count = (count < 1 || count > range_count) ? range_count : count;

      // - format response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);
      bc_array_s_append_be_ulli(&this->buffer,(ulli)resp_count);

      if (resp_count > 0)
      {
        // - format all records in range -
        if (resp_count == range_count)
        {
          do {
            sd_trace_descr_s_read_size_to_message(trace,first_id,&this->buffer);
          } while(++first_id <= last_id);
        }

        // - format just first record -
        else if (resp_count == 1)
        {
          sd_trace_descr_s_read_size_to_message(trace,first_id,&this->buffer);
        }

        // - format requested count of records -
        else
        {
          lli range_count_m1 = range_count - 1;
          lli resp_count_m1 = resp_count - 1;

          lli ri_idx = 0;
          lli ri_idx_end = range_count_m1*resp_count_m1;
          do {
            sd_trace_descr_s_read_size_to_message(trace,
                first_id + ri_idx/resp_count_m1,&this->buffer);
          } while((ri_idx += range_count_m1) <= ri_idx_end);
        }
      }

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_STEP_RANGE:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);
      lli first_id = va_arg(a_ap,lli);
      lli last_id = va_arg(a_ap,lli);
      ulli step = va_arg(a_ap,ulli);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);

      lli trace_head = sd_trace_s_head(&trace->trace);
      lli trace_tail = sd_trace_s_tail(&trace->trace);

      lli resp_count = 0;

      if (step > 0)
      {
        // - adjust first id -
        if (first_id < trace_tail)
        {
          first_id += ((trace_tail - first_id) / step + 1)*step;
        }

        // - adjust last id -
        if (last_id < 0 || last_id > trace_head)
        {
          last_id = trace_head;
        }

        lli range_count = (trace_head != -1 && first_id <= last_id) ? last_id - first_id + 1 : 0;
        resp_count = range_count > 0 ? (range_count - 1) / step + 1 : 0;
      }

      // - format response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);
      bc_array_s_append_be_ulli(&this->buffer,(ulli)resp_count);

      if (resp_count > 0)
      {
        // - format records in range -
        do {
          sd_trace_descr_s_read_size_to_message(trace,first_id,&this->buffer);
        } while((first_id += step) <= last_id);
      }

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_TIME_RANGE:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);
      ulli first_time = va_arg(a_ap,ulli);
      ulli last_time = va_arg(a_ap,ulli);
      lli count = va_arg(a_ap,lli);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);

      lli first_id = sd_trace_s_gre_time(&trace->trace,first_time);
      lli last_id = sd_trace_s_lee_time(&trace->trace,last_time);

      lli range_count = (first_id != -1 && last_id != -1 && first_id <= last_id) ? last_id - first_id + 1 : 0;
      lli resp_count = (count < 1 || count > range_count) ? range_count : count;

      // - format response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);
      bc_array_s_append_be_ulli(&this->buffer,(ulli)resp_count);

      if (resp_count > 0)
      {
        // - format all records in range -
        if (resp_count == range_count)
        {
          do {
            sd_trace_descr_s_read_size_to_message(trace,first_id,&this->buffer);
          } while(++first_id <= last_id);
        }

        // - format just first record -
        else if (resp_count == 1)
        {
          sd_trace_descr_s_read_size_to_message(trace,first_id,&this->buffer);
        }

        // - format requested count of records -
        else
        {
          lli range_count_m1 = range_count - 1;
          lli resp_count_m1 = resp_count - 1;

          lli ri_idx = 0;
          lli ri_idx_end = range_count_m1*resp_count_m1;
          do {
            sd_trace_descr_s_read_size_to_message(trace,
                first_id + ri_idx/resp_count_m1,&this->buffer);
          } while((ri_idx += range_count_m1) <= ri_idx_end);
        }
      }

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_WATCH:
  case sd_channel_cbreq_TRACE_IGNORE:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);

      sd_channel_watch_s *channel_watch = sd_channel_watches_s_at(&this->channel_watches,a_index);
      unsigned watch_idx = id_tree_s_get_idx(&channel_watch->trace_ids,trace_id);

      // - insert watch -
      if (watch_idx == c_idx_not_exist && a_type == sd_channel_cbreq_TRACE_WATCH)
      {
        id_tree_s_insert(&channel_watch->trace_ids,trace_id);

        // - set update watches -
        this->update_watches = 1;
      }

      // - remove watch -
      if (watch_idx != c_idx_not_exist && a_type == sd_channel_cbreq_TRACE_IGNORE)
      {
        id_tree_s_remove(&channel_watch->trace_ids,watch_idx);

        // - set update watches -
        this->update_watches = 1;
      }

      // debug output
      //sd_debug_print(this);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_TRACE_LEE_TIME:
  case sd_channel_cbreq_TRACE_GRE_TIME:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const string_s *trace_id = va_arg(a_ap,const string_s *);
      ulli time = va_arg(a_ap,ulli);

      sd_trace_descr_s search_trace = {{*trace_id,},};
      unsigned trace_idx = sd_trace_tree_s_get_idx(&this->traces,&search_trace);

      // - check if trace exists -
      if (trace_idx == c_idx_not_exist)
      {
        throw_error(SD_DAEMON_TRACE_NOT_EXIST);
      }

      sd_trace_descr_s *trace = sd_trace_tree_s_at(&this->traces,trace_idx);
      lli record_id = a_type == sd_channel_cbreq_TRACE_LEE_TIME ?
        sd_trace_s_lee_time(&trace->trace,time) : sd_trace_s_gre_time(&trace->trace,time);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_sd_segmentd_msg_header(&this->buffer,id,sd_channel_msg_type_RESPONSE,a_type,trace_id);

      sd_trace_descr_s_read_to_message(trace,record_id,&this->buffer);

      if (sd_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case sd_channel_cbreq_CONFIG:
    {/*{{{*/
      ulli id = va_arg(a_ap,ulli);
      const bc_array_s *data = va_arg(a_ap,const bc_array_s *);

      // - read configuration from buffer -
      if (sd_config_s_from_buffer(&this->config,data))
      {
        throw_error(SD_DAEMON_CONFIG_DATA_ERROR);
      }

      // - keep channel configuration -
      sd_conf_ip_port_s_copy(&this->config.channel,&this->last_config.channel);

      // - process configuration -
      if (sd_daemon_s_process_config(this))
      {
        // - terminate daemon -
        __sync_add_and_fetch(&g_terminate,1);

        throw_error(SD_DAEMON_CONFIG_DATA_ERROR);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_be_ulli(&this->buffer,id);
      bc_array_s_append_be_usi(&this->buffer,sd_channel_msg_type_RESPONSE);
      bc_array_s_append_be_usi(&this->buffer,a_type);

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
  libconf_sdl_init();
  librecord_sdl_init();

  char *name = "sd_daemon";
  char *conf = "sd_config.json";

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
  cassert(system("mkdir -p " PROCESS_RUN_DIR_PATH " " PROCESS_LOG_DIR_PATH) == 0); // NOLINT

  // - create process -
  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,0);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(process_s,process);
    cassert(process_s_create(&process,name) == 0);

    do {
      CONT_INIT_CLEAR(sd_daemon_s,daemon);

      if (sd_daemon_s_create(&daemon) ||
          sd_config_s_read_file(&daemon.config,conf) ||
          sd_daemon_s_process_config(&daemon))
      {
        break;
      }

      if (sd_daemon_s_run(&daemon))
      {
        // - ignore return value, always terminate -
      }

    } while(0);
  }

  librecord_sdl_clear();
  libconf_sdl_clear();
  libchannel_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

