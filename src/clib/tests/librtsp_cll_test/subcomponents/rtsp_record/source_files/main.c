
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- rtsp_client_list_s --
@begin
methods rtsp_client_list_s
@end

// -- rtsp_record_s --
@begin
methods rtsp_record_s
@end

// -- rtsp_records_s --
@begin
methods rtsp_records_s
@end

// -- rtsp_recorder_s --
@begin
methods rtsp_recorder_s
@end

int rtsp_recorder_s_create(rtsp_recorder_s *this,const char *a_base_dir,unsigned a_count,
  const char **a_server_ips,const usi *a_server_ports,const char **a_media,const char **a_file_names)
{/*{{{*/
  string_s_set_ptr(&this->base_dir,a_base_dir);

  if (a_count != 0)
  {
    unsigned record_idx = 0;
    do {
      rtsp_records_s_push_blank(&this->records);

      rtsp_record_s *record = rtsp_records_s_last(&this->records);
      string_s_set_ptr(&record->server_ip,a_server_ips[record_idx]);
      record->server_port = a_server_ports[record_idx];
      string_s_set_ptr(&record->media,a_media[record_idx]);
      string_s_set_ptr(&record->file_name,a_file_names[record_idx]);
      record->client_idx = c_idx_not_exist;

      // - open stream file -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"%s/%s.stream",this->base_dir.data,record->file_name.data);
      bc_array_s_push(&this->buffer,'\0');
      if (file_s_open(&record->stream_file,this->buffer.data,"wb"))
      {
        throw_error(RECORDER_FILE_OPEN_ERROR);
      }

      // - schedule reconnect timer-
      struct itimerspec itimerspec = {{0,0},{0,1}};
      if (epoll_s_timer_callback(&record->epoll_timer,&itimerspec,0,rtsp_recorder_s_record_time_event,this,record_idx))
      {
        throw_error(RECORDER_EPOLL_ERROR);
      }

    } while(++record_idx < a_count);
  }

  return 0;
}/*}}}*/

void rtsp_recorder_s_run(rtsp_recorder_s *this)
{/*{{{*/
  (void)this;

  do {

    // - wait on events -
    int err;
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      cassert(err == ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED);
    }
  } while(g_terminate == 0);
}/*}}}*/

int rtsp_recorder_s_record_time_event(void *a_rtsp_recorder,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_recorder_s_record_time_event: %u\n",a_index));

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(RECORDER_TIMER_READ_ERROR);
  }

  rtsp_recorder_s *this = (rtsp_recorder_s *)a_rtsp_recorder;
  rtsp_record_s *record = this->records.data + a_index;

  // - if record client does not exist -
  if (record->client_idx == c_idx_not_exist)
  {
    unsigned client_idx = rtsp_client_list_s_append_blank(&this->client_list);
    rtsp_client_s *client = &this->client_list.data[client_idx].object;

    if (rtsp_client_s_create(client,&record->server_ip,record->server_port,&record->media,
        rtsp_recorder_s_recv_sdp,
        rtsp_recorder_s_recv_packet,
        this,a_index))
    {
      rtsp_client_list_s_remove(&this->client_list,client_idx);

      // - schedule next reconnect try timer -
      struct itimerspec itimerspec = {{0,0},{5,0}};
      if (epoll_s_timer_callback(&record->epoll_timer,&itimerspec,0,rtsp_recorder_s_record_time_event,this,a_index))
      {
        throw_error(RECORDER_EPOLL_ERROR);
      }

      return 0;
    }

    // - register client fd -
    if (epoll_s_fd_callback(&client->epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,rtsp_recorder_s_client_fd_event,this,a_index))
    {
      throw_error(RECORDER_EPOLL_ERROR);
    }

    // - set record client index -
    record->client_idx = client_idx;

    // - update last packet time -
    if (clock_s_gettime(CLOCK_MONOTONIC,&record->last_pkt_time))
    {
      throw_error(RECORDER_GET_TIME_ERROR);
    }
  }

  return 0;
}/*}}}*/

int rtsp_recorder_s_client_fd_event(void *a_rtsp_recorder,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_recorder_s_client_fd_event: %u\n",a_index));

  rtsp_recorder_s *this = (rtsp_recorder_s *)a_rtsp_recorder;
  rtsp_record_s *record = this->records.data + a_index;
  rtsp_client_s *client = &this->client_list.data[record->client_idx].object;

  if (rtsp_client_s_fd_event(client,a_index,a_epoll_event))
  {
    rtsp_client_s_clear(client);
    rtsp_client_list_s_remove(&this->client_list,a_index);

    record->client_idx = c_idx_not_exist;

    // - schedule reconnect timer-
    struct itimerspec itimerspec = {{0,0},{5,0}};
    if (epoll_s_timer_callback(&record->epoll_timer,&itimerspec,0,rtsp_recorder_s_record_time_event,this,a_index))
    {
      throw_error(RECORDER_EPOLL_ERROR);
    }
  }

  return 0;
}/*}}}*/

int rtsp_recorder_s_recv_sdp(void *a_rtsp_recorder,unsigned a_index,const string_s *a_server_ip,const bc_array_s *a_src)
{/*{{{*/
  (void)a_server_ip;

  debug_message_1(fprintf(stderr,"rtsp_recorder_s_recv_sdp: %u\n",a_index));

  rtsp_recorder_s *this = (rtsp_recorder_s *)a_rtsp_recorder;
  rtsp_record_s *record = this->records.data + a_index;

  this->buffer.used = 0;
  bc_array_s_append_format(&this->buffer,"%s/%s.sdp",this->base_dir.data,record->file_name.data);
  bc_array_s_push(&this->buffer,'\0');

  CONT_INIT(file_s,sdp_file);
  if (file_s_open(&sdp_file,this->buffer.data,"wb"))
  {
    throw_error(RECORDER_FILE_OPEN_ERROR);
  }

  if (file_s_write_close(&sdp_file,a_src->data,a_src->used))
  {
    throw_error(RECORDER_FILE_WRITE_ERROR);
  }

  return 0;
}/*}}}*/

int rtsp_recorder_s_recv_packet(void *a_rtsp_recorder,unsigned a_index,time_s a_time,const bc_array_s *a_src)
{/*{{{*/
  rtsp_recorder_s *this = (rtsp_recorder_s *)a_rtsp_recorder;
  rtsp_record_s *record = this->records.data + a_index;

  rtsp_pkt_delay_t delay = RTSP_DELAY_FROM_NANOSEC(a_time - record->last_pkt_time);

  if (stream_s_write(&record->stream_file,&delay,sizeof(rtsp_pkt_delay_t)) ||
      stream_s_write(&record->stream_file,a_src->data,a_src->used))
  {
    throw_error(RECORDER_FILE_WRITE_ERROR);
  }

  // - update last packet time -
  record->last_pkt_time = a_time;

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

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(rtsp_recorder_s,recorder);

    const char *server_ips[] = {
      //"10.2.35.6",
      "192.168.3.29",
    };

    const usi server_ports[] = {
      //554,
      554,
    };

    const char *media[] = {
      //"udpstream_ch1",
      "axis-media/media.amp",
    };

    const char *file_names[] = {
      //"10.2.35.6",
      "192.168.3.29",
    };

    cassert(rtsp_recorder_s_create(&recorder,"recordings",1,server_ips,server_ports,media,file_names) == 0);
    rtsp_recorder_s_run(&recorder);
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

