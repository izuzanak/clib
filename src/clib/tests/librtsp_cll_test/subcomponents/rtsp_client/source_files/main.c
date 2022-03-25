
@begin
include "main.h"
@end

epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- rtsp_client_list_s --
@begin
methods rtsp_client_list_s
@end

// === program entry function ==================================================

int client_authenticate(void *a_client_list,unsigned a_index)
{/*{{{*/
  debug_message_1(fprintf(stderr,"client_authenticate\n"));

  rtsp_client_list_s *client_list = (rtsp_client_list_s *)a_client_list;
  rtsp_client_s *client = rtsp_client_list_s_at(client_list,a_index);
  rtsp_digest_s *digest = &client->digest;

  //const char *user = "jirka";
  //const char *pass = "heslo";
  ////const char *pass = "test";

  const char *user = "root";
  const char *pass = "kolotoc";
  //const char *pass = "test";

  // - verify digest authentication -
  char separator = ':';

  CONT_INIT_CLEAR(crypto_digest_info_s,digest_info);
  CONT_INIT_CLEAR(crypto_digest_s,ha1);
  CONT_INIT_CLEAR(crypto_digest_s,ha2);
  CONT_INIT_CLEAR(crypto_digest_s,response);
  CONT_INIT_CLEAR(bc_array_s,md5_data);
  CONT_INIT_CLEAR(bc_array_s,md5_hexa);

  if (crypto_digest_info_s_get_by_name(&digest_info,"MD5") ||
      crypto_digest_s_create(&ha1,&digest_info) ||
      crypto_digest_s_update(&ha1,user,strlen(user)) ||
      crypto_digest_s_update(&ha1,&separator,1) ||
      crypto_digest_s_update(&ha1,digest->realm.data,digest->realm.size - 1) ||
      crypto_digest_s_update(&ha1,&separator,1) ||
      crypto_digest_s_update(&ha1,pass,strlen(pass)) ||
      crypto_digest_s_create(&ha2,&digest_info) ||
      crypto_digest_s_update(&ha2,digest->method.data,digest->method.size - 1) ||
      crypto_digest_s_update(&ha2,&separator,1) ||
      crypto_digest_s_update(&ha2,digest->uri.data,digest->uri.size - 1) ||
      crypto_digest_s_create(&response,&digest_info) ||
      crypto_digest_s_value(&ha1,&md5_data) ||
      (crypto_encode_base16(md5_data.data,md5_data.used,&md5_hexa),0) ||
      crypto_digest_s_update(&response,md5_hexa.data,md5_hexa.used) ||
      crypto_digest_s_update(&response,&separator,1) ||
      crypto_digest_s_update(&response,digest->nonce.data,digest->nonce.size - 1) ||
      crypto_digest_s_update(&response,&separator,1) ||
      (md5_data.used = 0,md5_hexa.used = 0,0) ||
      crypto_digest_s_value(&ha2,&md5_data) ||
      (crypto_encode_base16(md5_data.data,md5_data.used,&md5_hexa),0) ||
      crypto_digest_s_update(&response,md5_hexa.data,md5_hexa.used) ||
      (md5_data.used = 0,md5_hexa.used = 0,0) ||
      crypto_digest_s_value(&response,&md5_data) ||
      (crypto_encode_base16(md5_data.data,md5_data.used,&md5_hexa),0))
  {
    throw_error(RTSP_CLIENT_DIGEST_ERROR);
  }

  // - set digest username and response -
  string_s_set_ptr(&digest->username,user);
  string_s_set(&digest->response,md5_hexa.used,md5_hexa.data);

  return 0;
}/*}}}*/

int client_recv_sdp(void *a_client_list,unsigned a_index,const string_s *a_server_ip,const bc_array_s *a_src)
{/*{{{*/
  (void)a_client_list;
  (void)a_index;
  (void)a_server_ip;
  (void)a_src;

  debug_message_1(fprintf(stderr,"client_recv_sdp\n"));

  return 0;
}/*}}}*/

int client_recv_packet(void *a_client_list,unsigned a_index,time_s a_time,const bc_array_s *a_src)
{/*{{{*/
  (void)a_client_list;
  (void)a_index;
  (void)a_time;
  (void)a_src;

  // - packet received indication -
  fputc('.',stderr);

  return 0;
}/*}}}*/

int client_fd_event(void *a_client_list,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  rtsp_client_list_s *client_list = (rtsp_client_list_s *)a_client_list;
  rtsp_client_s *client = &client_list->data[a_index].object;

  if (rtsp_client_s_fd_event(client,a_index,a_epoll_event))
  {
    rtsp_client_s_clear(client);
    rtsp_client_list_s_remove(client_list,a_index);
  }

  return 0;
}/*}}}*/

int epoll_fd_update(int a_fd,unsigned a_evts,int a_update_cb,const epoll_callback_s *a_callback)
{/*{{{*/
  return epoll_s_fd_update(g_epoll,a_fd,a_evts,a_update_cb,a_callback);
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  {
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
    //const char *media = "local/test/56043001/9200/9300/single";
    ////const char *media = "local/test/56043001/9200/9300/burst";

    //const char *server_ip = "192.168.3.29";
    //const unsigned short port = 554;
    //const char *media = "axis-media/media.amp";

    //const char *server_ip = "10.177.3.105";
    //const unsigned short port = 554;
    ////const char *media = "onvif/profile2/media.smp";
    //const char *media = "onvif/profile3/media.smp";

    //const char *server_ip = "10.177.3.106";
    //const unsigned short port = 554;
    //const char *media = "h264";

    //const char *server_ip = "127.0.0.1";
    //const unsigned short port = 8006;
    //const char *media = "onvif/http_0/56043006/last/all/single";

    const char *server_ip = "10.2.1.174";
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

    CONT_INIT_CLEAR(string_s,server_ip_str);
    string_s_set_ptr(&server_ip_str,server_ip);

    CONT_INIT_CLEAR(string_s,media_str);
    string_s_set_ptr(&media_str,media);

    CONT_INIT_CLEAR(epoll_s,epoll);
    cassert(epoll_s_create(&epoll,EPOLL_CLOEXEC) == 0);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(rtsp_client_list_s,client_list);

    unsigned client_cnt = 1;
    unsigned idx = 0;
    do {
      unsigned client_idx = rtsp_client_list_s_append_blank(&client_list);
      rtsp_client_s *client = &client_list.data[client_idx].object;
      cassert(rtsp_client_s_create(client,&server_ip_str,port,&media_str,
          client_authenticate,
          client_recv_sdp,
          client_recv_packet,
          &client_list,client_idx) == 0);

      //cassert(rtsp_client_s_init_ssl(client) == 0);
      cassert(epoll_s_fd_callback(&client->epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,client_fd_event,&client_list,idx) == 0);
    } while(++idx < client_cnt);

    //unsigned count = 1000;
    do {

      // - wait on events -
      cassert(epoll_s_wait(g_epoll,-1) == 0);
    } while(
    client_list.first_idx != c_idx_not_exist
    //--count != 0
    );
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

