
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === methods of generated structures =========================================

// -- rtsp_reader_s --
@begin
methods rtsp_reader_s
@end

// -- rtsp_readers_s --
@begin
methods rtsp_readers_s
@end

// -- rtsp_player_s --
@begin
methods rtsp_player_s
@end

int rtsp_player_s_create(rtsp_player_s *this,const char *a_base_dir,const char *a_ip,usi a_port)
{/*{{{*/
  cassert(regex_s_create(&this->url_path_regex,"^rtsp://[0-9.]\\+:[0-9]\\+/\\(.*\\)$") == 0);

  string_s_set_ptr(&this->base_dir,a_base_dir);
  string_s_set_ptr(&this->ip,a_ip);
  this->port = a_port;
  epoll_s_create(&this->epoll,0);

  if (rtsp_server_s_create(&this->server,&this->ip,this->port,
        rtsp_player_s_conn_new,
        rtsp_player_s_conn_drop,
        rtsp_player_s_conn_get_sdp,
        rtsp_player_s_conn_check_media,
        rtsp_player_s_conn_playing,
        rtsp_player_s_conn_get_packet,
        this))
  {
    throw_error(PLAYER_RTSP_SERVER_CREATE_ERROR);
  }

  if (epoll_s_fd_callback(&this->epoll,&this->server.epoll_fd,EPOLLIN | EPOLLPRI,rtsp_player_s_server_fd_event,this,0))
  {
    throw_error(PLAYER_RTSP_SERVER_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

void rtsp_player_s_run(rtsp_player_s *this)
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

int rtsp_player_s_server_fd_event(void *a_rtsp_player,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  rtsp_player_s *this = (rtsp_player_s *)a_rtsp_player;

  if (rtsp_server_s_fd_event(&this->server,a_index,a_epoll_event,a_epoll))
  {
    throw_error(PLAYER_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

int rtsp_player_s_conn_new(void *a_rtsp_player,unsigned a_index)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_player_s_conn_new: %u\n",a_index));

  rtsp_player_s *this = (rtsp_player_s *)a_rtsp_player;

  while (this->readers.used <= a_index)
  {
    rtsp_readers_s_push_blank(&this->readers);
  }

  return 0;
}/*}}}*/

int rtsp_player_s_conn_drop(void *a_rtsp_player,unsigned a_index)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_player_s_conn_drop: %u\n",a_index));

  rtsp_player_s *this = (rtsp_player_s *)a_rtsp_player;

  rtsp_reader_s_clear(this->readers.data + a_index);

  return 0;
}/*}}}*/

int rtsp_player_s_conn_get_sdp(void *a_rtsp_player,unsigned a_index,const char *a_url,bc_array_s *a_trg)
{/*{{{*/
  (void)a_index;

  debug_message_6(fprintf(stderr,"rtsp_player_s_conn_get_sdp: %u\n",a_index));

  rtsp_player_s *this = (rtsp_player_s *)a_rtsp_player;

  // - retrieve media name from url -
  if (!regex_s_match_n(&this->url_path_regex,a_url,2,&this->match_array) ||
      this->match_array.used != 2)
  {
    throw_error(PLAYER_INVALID_MEDIA_URL);
  }

  this->buffer.used = 0;
  bc_array_s_append_format(&this->buffer,"%s/%s.sdp",this->base_dir.data,a_url + this->match_array.data[1].rm_so);
  bc_array_s_push(&this->buffer,'\0');

  struct stat statbuf;
  if (stat(this->buffer.data,&statbuf))
  {
    throw_error(PLAYER_INVALID_MEDIA_URL);
  }

  CONT_INIT(file_s,sdp_file);
  if (file_s_open(&sdp_file,this->buffer.data,"rb"))
  {
    throw_error(PLAYER_FILE_OPEN_ERROR);
  }

  if (file_s_read_close(&sdp_file,a_trg))
  {
    throw_error(PLAYER_FILE_READ_ERROR);
  }

  return 0;
}/*}}}*/

int rtsp_player_s_conn_check_media(void *a_rtsp_player,unsigned a_index,const char *a_url)
{/*{{{*/
  debug_message_6(fprintf(stderr,"rtsp_player_s_conn_check_media: %u\n",a_index));

  rtsp_player_s *this = (rtsp_player_s *)a_rtsp_player;
  rtsp_reader_s *reader = this->readers.data + a_index;

  if (reader->stream_file == NULL)
  {
    // - retrieve media name from url -
    if (!regex_s_match_n(&this->url_path_regex,a_url,2,&this->match_array) ||
        this->match_array.used != 2)
    {
      throw_error(PLAYER_INVALID_MEDIA_URL);
    }

    this->buffer.used = 0;
    bc_array_s_append_format(&this->buffer,"%s/%s.stream",this->base_dir.data,a_url + this->match_array.data[1].rm_so);
    bc_array_s_push(&this->buffer,'\0');

    struct stat statbuf;
    if (stat(this->buffer.data,&statbuf))
    {
      throw_error(PLAYER_INVALID_MEDIA_URL);
    }

    if (file_s_open(&reader->stream_file,this->buffer.data,"rb"))
    {
      throw_error(PLAYER_FILE_OPEN_ERROR);
    }
  }

  return 0;
}/*}}}*/

int rtsp_player_s_conn_playing(void *a_rtsp_player,unsigned a_index,ulli a_session)
{/*{{{*/
  (void)a_rtsp_player;
  (void)a_index;
  (void)a_session;

  debug_message_6(fprintf(stderr,"rtsp_player_s_conn_playing: %u\n",a_index));

  return 0;
}/*}}}*/

int rtsp_player_s_conn_get_packet(void *a_rtsp_player,unsigned a_index,ulli *a_delay,static_buffer_s *a_trg)
{/*{{{*/
  (void)a_rtsp_player;
  (void)a_index;
  (void)a_delay;
  (void)a_trg;

  debug_message_6(fprintf(stderr,"rtsp_player_s_conn_get_packet: %u\n",a_index));

  return 1;
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

  CONT_INIT(rtsp_player_s,player);
  cassert(rtsp_player_s_create(&player,"../recordings","127.0.0.1",8000) == 0);
  rtsp_player_s_run(&player);
  rtsp_player_s_clear(&player);

  memcheck_release_assert();

  return 0;
}/*}}}*/

