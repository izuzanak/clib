
@begin
include "main.h"
@end

// === program entry function ==================================================

void conn_new(void *a_rtsp_server,unsigned a_index)
{/*{{{*/
  (void)a_rtsp_server;
  (void)a_index;

  debug_message_6(fprintf(stderr,"conn_new\n"));
}/*}}}*/

void conn_drop(void *a_rtsp_server,unsigned a_index)
{/*{{{*/
  (void)a_rtsp_server;
  (void)a_index;

  debug_message_6(fprintf(stderr,"conn_drop\n"));
}/*}}}*/

int conn_get_sdp(void *a_rtsp_server,unsigned a_index,const char *a_url,bc_array_s *a_trg)
{/*{{{*/
  (void)a_index;

  rtsp_server_s *server = (rtsp_server_s *)a_rtsp_server;

  bc_array_s_append_format(a_trg,
"v=0\r\n"
"o=- 946696689382405 1 IN IP4 %s\r\n"
"s=Moxa Streaming Server\r\n"
"i=/dev/Streaming\r\n"
"t=0 0\r\n"
"a=tool:MOXA VPort P06-1MP-M12 IP Camera\r\n"
"a=type:broadcast\r\n"
"a=control:*\r\n"
"a=range:npt=0-\r\n"
"a=x-qt-text-nam:Moxa Streaming Server\r\n"
"a=x-qt-text-inf:/dev/Streaming\r\n"
"m=video 0 RTP/AVP 97\r\n"
"c=IN IP4 0.0.0.0\r\n"
"a=rtpmap:97 H264/90000\r\n"
"a=fmtp:97 packetization-mode=1; profile-level-id=640020; sprop-parameter-sets=Z2QAIK2EBUViuKxUdCAqKxXFYqOhAVFYrisVHQgKisVxWKjoQFRWK4rFR0ICorFcVio6ECSFITk8nyfk/k/J8nm5s00IEkKQnJ5Pk/J/J+T5PNzZprQCgDLSpAAAAwHgAABwgYEAABMS0AAAmJaL3vheEQjU,aO48sA==\r\n"
"a=control:%s/video\r\n"
"a=recvonly\r\n"
"m=audio 0 RTP/AVP 0\r\n"
"c=IN IP4 0.0.0.0\r\n"
"a=rtpmap:0 PCMU/8035/1\r\n"
"a=control:%s/audio\r\n"
"a=recvonly\r\n",server->ip.data,a_url,a_url);

  return 0;
}/*}}}*/

int conn_check_media(void *a_rtsp_server,unsigned a_index,const char *a_url)
{/*{{{*/
  (void)a_rtsp_server;
  (void)a_index;
  (void)a_url;

  return 0;
}/*}}}*/

int conn_get_packet(void *a_rtsp_server,unsigned a_index,ulli *a_delay,bc_array_s *a_trg)
{/*{{{*/
  (void)a_rtsp_server;
  (void)a_index;
  (void)a_delay;
  (void)a_trg;

  return 1;
}/*}}}*/

int server_fd_event(void *a_server,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  rtsp_server_s *server = (rtsp_server_s *)a_server;

  if (rtsp_server_s_fd_event(server,a_index,a_epoll_event,a_epoll))
  {
    throw_error(RTSP_TEST_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  const char *address = "127.0.0.1";
  const unsigned short port = 8000;

  //const char *address = "10.2.35.6";
  //const unsigned short port = 554;

  CONT_INIT(epoll_s,epoll);
  cassert(epoll_s_create(&epoll,0) == 0);

  CONT_INIT(string_s,ip);
  string_s_set_ptr(&ip,address);

  CONT_INIT(rtsp_server_s,server);
  cassert(rtsp_server_s_create(&server,&ip,port,
        conn_new,
        conn_drop,
        conn_get_sdp,
        conn_check_media,
        conn_get_packet,
        &server) == 0);

  cassert(epoll_s_fd_callback(&epoll,&server.epoll_fd,EPOLLIN | EPOLLPRI,server_fd_event,&server,0) == 0)

  do {

    // - wait on events -
    cassert(epoll_s_wait(&epoll,32,-1) == 0);
  } while(
  //server.conn_list.first_idx != c_idx_not_exist
  1
  );

  rtsp_server_s_clear(&server);
  epoll_s_clear(&epoll);
  string_s_clear(&ip);

  memcheck_release_assert();

  return 0;
}/*}}}*/

