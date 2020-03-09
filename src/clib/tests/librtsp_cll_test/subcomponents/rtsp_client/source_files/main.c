
@begin
include "main.h"
@end

// === methods of generated structures =========================================

// -- rtsp_client_list_s --
@begin
methods rtsp_client_list_s
@end

// === program entry function ==================================================

int client_recv_sdp(void *a_client_list,unsigned a_index,const string_s *a_server_ip,const bc_array_s *a_src)
{/*{{{*/
  (void)a_client_list;
  (void)a_index;
  (void)a_server_ip;
  (void)a_src;

  debug_message_1(fprintf(stderr,"client_recv_sdp\n"));

  return 0;
}/*}}}*/

int client_recv_packet(void *a_client_list,unsigned a_index,const bc_array_s *a_src)
{/*{{{*/
  (void)a_client_list;
  (void)a_index;
  (void)a_src;

  // - packet received indication -
  fputc('.',stderr);

  return 0;
}/*}}}*/

int client_fd_event(void *a_client_list,unsigned a_index,epoll_event_s *a_epoll_event,epoll_s *a_epoll)
{/*{{{*/
  rtsp_client_list_s *client_list = (rtsp_client_list_s *)a_client_list;
  rtsp_client_s *client = &client_list->data[a_index].object;

  if (rtsp_client_s_fd_event(client,a_index,a_epoll_event,a_epoll))
  {
    rtsp_client_s_clear(client);
    rtsp_client_list_s_remove(client_list,a_index);
  }

  return 0;
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  //const char *server_ip = "10.2.35.1";
  //const unsigned short port = 554;
  //const char *media = "udpstream_ch1";

  //const char *server_ip = "10.2.35.6";
  //const unsigned short port = 554;
  //const char *media = "udpstream_ch1";

  //const char *server_ip = "192.168.3.3";
  //const unsigned short port = 8080;
  //const char *media = "video/h264";

  //const char *server_ip = "127.0.0.1";
  //const unsigned short port = 8006;
  //const char *media = "mpv/56043001/last/all/single";

  //const char *server_ip = "192.168.3.29";
  const char *server_ip = "192.168.0.90";
  const unsigned short port = 554;
  const char *media = "axis-media/media.amp";

  //const char *server_ip = "10.2.1.18";
  //const unsigned short port = 554;
  //const char *media = "11";

  //const char *server_ip = "10.2.1.176";
  ////const char *server_ip = "127.0.0.1";
  //const unsigned short port = 8006;
  //const char *media = "56035002/3480";
  ////const char *media = "56035002/1390";
  ////const char *media = "56035002/time_20150101103015";

  //const char *server_ip = "10.2.1.10";
  //const unsigned short port = 8080;
  //const char *media = "video";

  //const char *server_ip = "10.2.43.5";
  //const unsigned short port = 554;
  //const char *media = "udpstream_ch1";

  CONT_INIT(string_s,server_ip_str);
  string_s_set_ptr(&server_ip_str,server_ip);

  CONT_INIT(string_s,media_str);
  string_s_set_ptr(&media_str,media);

  CONT_INIT(epoll_s,epoll);
  cassert(epoll_s_create(&epoll,0) == 0);

  CONT_INIT(rtsp_client_list_s,client_list);

  unsigned client_cnt = 1;
  unsigned idx = 0;
  do {
    unsigned client_idx = rtsp_client_list_s_append_blank(&client_list);
    rtsp_client_s *client = &client_list.data[client_idx].object;
    cassert(rtsp_client_s_create(client,&server_ip_str,port,&media_str,
        client_recv_sdp,
        client_recv_packet,
        &client_list,client_idx) == 0);

    //cassert(rtsp_client_s_init_ssl(client) == 0);
    cassert(epoll_s_fd_callback(&epoll,&client->epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,client_fd_event,&client_list,idx) == 0);
  } while(++idx < client_cnt);

  //unsigned count = 1000;
  do {

    // - wait on events -
    cassert(epoll_s_wait(&epoll,32,-1) == 0);
  } while(
  client_list.first_idx != c_idx_not_exist
  //--count != 0
  );

  rtsp_client_list_s_clear(&client_list);
  epoll_s_clear(&epoll);
  string_s_clear(&media_str);
  string_s_clear(&server_ip_str);

  memcheck_release_assert();

  return 0;
}/*}}}*/

