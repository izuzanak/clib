
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- mqtt_client_s --
@begin
methods mqtt_client_s
@end

int mqtt_client_s_create(mqtt_client_s *this,
    const char *a_client_id,const char *a_server_ip,usi a_server_port)
{/*{{{*/

  // - create mqtt connection -
  if (mqtt_conn_s_create(&this->connection,
        a_client_id,a_server_ip,a_server_port,
        mqtt_client_s_mqtt_callback,
        this,0))
  {
    throw_error(MQTT_CLIENT_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

int mqtt_client_s_run(mqtt_client_s *this)
{/*{{{*/
  (void)this;

  while(!g_terminate)
  {
    // - wait on events -
    int err;
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(MQTT_CLIENT_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int mqtt_client_s_mqtt_callback(void *a_mqtt_client,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  (void)a_ap;

  mqtt_client_s *this = (mqtt_client_s *)a_mqtt_client;

  switch (a_type)
  {
  case cl_mqtt_cbreq_ERROR:
    fprintf(stderr,"mqtt_client_s_mqtt_callback: cl_mqtt_cbreq_ERROR\n");
    break;
  case cl_mqtt_cbreq_CONNECTED:
    {/*{{{*/
      fprintf(stderr,"mqtt_client_s_mqtt_callback: cl_mqtt_cbreq_CONNECTED\n");

      CONT_INIT_CLEAR(bc_array_s,props);

      // - subscribe -
      CONT_INIT_CLEAR(string_array_s,sub_filters);
      string_array_s_push_blanks(&sub_filters,2);
      string_s_set_ptr(sub_filters.data + 0,"test/sub0");
      string_s_set_ptr(sub_filters.data + 1,"test/sub1");

      uint16_t sub_packet_id;
      if (mqtt_conn_s_subscribe(&this->connection,&sub_filters,&props,2,&sub_packet_id))
      {
        throw_error(MQTT_CLIENT_SUBSCRIBE_ERROR);
      }

      //// - unsubscribe -
      //CONT_INIT_CLEAR(string_array_s,unsub_filters);
      //string_array_s_push_blanks(&unsub_filters,2);
      //string_s_set_ptr(unsub_filters.data + 0,"test/sub0");
      //string_s_set_ptr(unsub_filters.data + 1,"test/sub1");

      //uint16_t unsub_packet_id;
      //if (mqtt_conn_s_unsubscribe(&this->connection,&unsub_filters,&props,&unsub_packet_id))
      //{
      //  throw_error(MQTT_CLIENT_UNSUBSCRIBE_ERROR);
      //}

      // - publish -
      CONT_INIT_CLEAR(string_s,topic);
      CONT_INIT_CLEAR(bc_array_s,payload);

      string_s_set_ptr(&topic,"test/sub0");
      bc_array_s_append_ptr(&payload,"Some really important data");

      uint16_t pub_packet_id;
      if (mqtt_conn_s_publish(&this->connection,&topic,&payload,&props,2,0,&pub_packet_id))
      {
        throw_error(MQTT_CLIENT_PUBLISH_ERROR);
      }
    }/*}}}*/
    break;
  case cl_mqtt_cbreq_DISCONNECTED:
    fprintf(stderr,"mqtt_client_s_mqtt_callback: cl_mqtt_cbreq_DISCONNECTED\n");
    break;
  case cl_mqtt_cbreq_DROPPED:
    fprintf(stderr,"mqtt_client_s_mqtt_callback: cl_mqtt_cbreq_DROPPED\n");
    break;
  case cl_mqtt_cbreq_PUBLISHED:
    fprintf(stderr,"mqtt_client_s_mqtt_callback: cl_mqtt_cbreq_PUBLISHED\n");
    break;
  case cl_mqtt_cbreq_SUBSCRIBED:
    fprintf(stderr,"mqtt_client_s_mqtt_callback: cl_mqtt_cbreq_SUBSCRIBED\n");
    break;
  case cl_mqtt_cbreq_UNSUBSCRIBED:
    fprintf(stderr,"mqtt_client_s_mqtt_callback: cl_mqtt_cbreq_UNSUBSCRIBED\n");
    break;
  case cl_mqtt_cbreq_RECEIVED:
    fprintf(stderr,"mqtt_client_s_mqtt_callback: cl_mqtt_cbreq_RECEIVED\n");
    break;
  default:
    {/*{{{*/

      // - log message -
      log_info_2("mqtt client %u, unknown message",a_index);
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
  libtcp_cll_init();
  //libmqtt_cll_init();

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,0);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(mqtt_client_s,mqtt_client);

    cassert(mqtt_client_s_create(&mqtt_client,"mqtt_cll","127.0.0.1",1883) == 0);
    cassert(mqtt_client_s_run(&mqtt_client) == 0);
  }

  //libmqtt_cll_clear();
  libtcp_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

