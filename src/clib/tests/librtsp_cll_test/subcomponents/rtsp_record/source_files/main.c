
@begin
include "main.h"
@end

volatile int g_terminate = 0;

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
  epoll_s_create(&this->epoll,0);

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
      cassert(epoll_s_timer_delay(&this->epoll,0,rtsp_recorder_s_record_time_event,this,record_idx,&record->epoll_timer) == 0);

    } while(++record_idx < a_count);
  }

  return 0;
}/*}}}*/

void rtsp_recorder_s_run(rtsp_recorder_s *this)
{/*{{{*/
  do {

    // - wait on events -
    int err;
    if ((err = epoll_s_wait(&this->epoll,32,-1)))
    {
      cassert(err == ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED);
    }
  } while(g_terminate == 0);
}/*}}}*/

void rtsp_recorder_s_record_time_event(void *a_rtsp_recorder,unsigned a_index,unsigned a_timer,epoll_s *a_epoll)
{/*{{{*/
  (void)a_timer;
  (void)a_epoll;

  debug_message_6(fprintf(stderr,"rtsp_recorder_s_record_time_event: %u\n",a_index));

  rtsp_recorder_s *this = (rtsp_recorder_s *)a_rtsp_recorder;
  rtsp_record_s *record = this->records.data + a_index;

  // - drop timer identifier -
  record->epoll_timer.timer = c_idx_not_exist;

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
      cassert(epoll_s_timer_delay(&this->epoll,5000000000ULL,rtsp_recorder_s_record_time_event,this,a_index,&record->epoll_timer) == 0);

      return;
    }

    // - register client fd -
    cassert(epoll_s_fd_callback(&this->epoll,&client->epoll_fd,EPOLLIN | EPOLLPRI,rtsp_recorder_s_client_fd_event,this,a_index) == 0);

    // - set record client index -
    record->client_idx = client_idx;

    // - update last packet time -
    cassert(clock_s_gettime(CLOCK_MONOTONIC,&record->last_pkt_time) == 0);
  }
}/*}}}*/

void rtsp_recorder_s_client_fd_event(void *a_rtsp_recorder,unsigned a_index,int a_fd,epoll_s *a_epoll)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_recorder_s_client_fd_event: %u\n",a_index));

  rtsp_recorder_s *this = (rtsp_recorder_s *)a_rtsp_recorder;
  rtsp_record_s *record = this->records.data + a_index;
  rtsp_client_s *client = &this->client_list.data[record->client_idx].object;

  if (rtsp_client_s_fd_event(client,a_index,a_fd,a_epoll))
  {
    rtsp_client_s_clear(client);
    rtsp_client_list_s_remove(&this->client_list,a_index);

    record->client_idx = c_idx_not_exist;

    // - schedule reconnect timer-
    cassert(epoll_s_timer_delay(&this->epoll,0,rtsp_recorder_s_record_time_event,this,a_index,&record->epoll_timer) == 0);
  }
}/*}}}*/

int rtsp_recorder_s_recv_sdp(void *a_rtsp_recorder,unsigned a_index,const bc_array_s *a_src)
{/*{{{*/
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

int rtsp_recorder_s_recv_packet(void *a_rtsp_recorder,unsigned a_index,const bc_array_s *a_src)
{/*{{{*/
  rtsp_recorder_s *this = (rtsp_recorder_s *)a_rtsp_recorder;
  rtsp_record_s *record = this->records.data + a_index;

  ulli time;
  if (clock_s_gettime(CLOCK_MONOTONIC,&time))
  {
    throw_error(RECORDER_GET_TIME_ERROR);
  }

  rtsp_pkt_delay_t delay = (time - record->last_pkt_time)/1000000ULL;

  if (stream_s_write(&record->stream_file,&delay,sizeof(rtsp_pkt_delay_t)) ||
      stream_s_write(&record->stream_file,a_src->data,a_src->used))
  {
    throw_error(RECORDER_FILE_WRITE_ERROR);
  }

  // - update last packet time -
  record->last_pkt_time = time;

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

  cassert(signal_s_simple_handler(signal_handler) == 0);

  CONT_INIT(rtsp_recorder_s,recorder);

  const char *server_ips[] = {
    //"10.2.35.6",
    "10.2.1.18",
  };

  const usi server_ports[] = {
    //554,
    554,
  };

  const char *media[] = {
    //"udpstream_ch1",
    "11",
  };

  const char *file_names[] = {
    //"10.2.35.6",
    "10.2.1.18",
  };

  cassert(rtsp_recorder_s_create(&recorder,"../recordings",1,server_ips,server_ports,media,file_names) == 0);
  rtsp_recorder_s_run(&recorder);
  rtsp_recorder_s_clear(&recorder);

  memcheck_release_assert();

  return 0;
}/*}}}*/

