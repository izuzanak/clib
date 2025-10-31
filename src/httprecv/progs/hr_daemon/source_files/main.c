
@begin
include "main.h"
@end

const char *g_name;
const char *g_conf;

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- hr_recv_data_s --
@begin
methods hr_recv_data_s
@end

// -- hr_recv_data_list_s --
@begin
methods hr_recv_data_list_s
@end

// -- hr_daemon_s --
@begin
methods hr_daemon_s
@end

int hr_daemon_s_create(hr_daemon_s *this)
{/*{{{*/
  hr_daemon_s_clear(this);

  // - create url regex -
  if (regex_s_create(&this->url_regex,
        "^/uploaded/"
        "([A-Z]+_[A-Z]+_[0-9]{13})/"
        "(CEBDBlock|EventSet|Heartbeat|ReadingBlock)$"))
  {
    throw_error(HR_DAEMON_URL_REGEX_CREATE_ERROR);
  }

  // - reset configuration changed flag -
  this->config_changed_flag = 0;

  return 0;
}/*}}}*/

int hr_daemon_s_process_config(hr_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("process configuration");

  // - if recv_http server configuration changed -
  if (!hr_conf_ip_port_s_compare(
        &this->config.server,
        &this->last_config.server))
  {
    // - create socket address -
    socket_address_s address;
    if (socket_address_s_create(&address,
      this->config.server.ip.data,this->config.server.port))
    {
      throw_error(RECV_HTTP_SERVER_INVALID_ADDRESS);
    }

    // - prepare mhd options -
    const struct MHD_OptionItem mhd_options[] = {
      { MHD_OPTION_SOCK_ADDR,0,&address },
      { MHD_OPTION_END,0,NULL }
    };

    // - create recv_http server -
    if (http_epoll_s_create(&this->recv_http,
          this->config.server.port,
          hr_recv_http_connection_callback,
          hr_recv_http_completed_callback,
          this,
          mhd_options))
    {
      throw_error(RECV_HTTP_SERVER_CREATE_ERROR);
    }

    // - create dummy recv data record -
    hr_recv_data_list_s_append_blank(&this->recv_data_list);
  }

  // - update last configuration -
  hr_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int hr_daemon_s_run(hr_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_0("running");

  while(!g_terminate)
  {
    if (this->config_changed_flag)
    {
      if (hr_daemon_s_process_config(this))
      {
        throw_error(HR_DAEMON_CONFIG_DATA_ERROR);
      }

      if (g_terminate)
      {
        break;
      }

      // - reset configuration changed flag -
      this->config_changed_flag = 0;
    }

    // - reset recv_http process flag -
    this->recv_http.process = 0;

    // - wait on events -
    int err;
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(HR_DAEMON_EPOLL_WAIT_ERROR);
      }
    }

    // - recv_http server check -
    if (http_epoll_s_check(&this->recv_http))
    {
      throw_error(RECV_HTTP_SERVER_CHECK_ERROR);
    }
  }

  return 0;
}/*}}}*/

int hr_recv_http_connection_callback(http_conn_s *a_conn)
{/*{{{*/
  debug_message_6(fprintf(stderr,"hr_recv_http_connection_callback\n"));

  hr_daemon_s *this = a_conn->server->user_data;

  // - log message -
  if (a_conn->user_data == NULL)
  {
    this->ip_buffer.used = 0;
    if (http_conn_s_client_ip(a_conn,&this->ip_buffer))
    {
      throw_error(RECV_HTTP_CONN_CLIENT_IP_ERROR);
    }

    log_info_2("recv_http %p, new connection, ip: %s, method: %s",
        a_conn,this->ip_buffer.data,a_conn->method);
  }

  CONT_INIT_CLEAR(http_resp_s,resp);

#define HR_RECV_HTTP_CONNECTION_CALLBACK_BAD_REQUEST() \
{/*{{{*/\
\
  /* - log message - */\
  log_info_2("recv_http %p, response, Bad request",a_conn);\
\
  if (http_resp_s_create_from_buffer(&resp,strlen("Bad request\n"),"Bad request\n",MHD_RESPMEM_PERSISTENT) ||\
      MHD_add_response_header(resp.response,"Connection","close") != MHD_YES ||\
      http_conn_s_queue_response(a_conn,MHD_HTTP_BAD_REQUEST,&resp))\
  {\
    throw_error(RECV_HTTP_CONN_QUEUE_RESPONSE_ERROR);\
  }\
}/*}}}*/

  switch (a_conn->conn_type)
  {
  case c_http_conn_type_GET:
    {/*{{{*/
      HR_RECV_HTTP_CONNECTION_CALLBACK_BAD_REQUEST();
    }/*}}}*/
    break;
  case c_http_conn_type_POST:
  case c_http_conn_type_PUT:
    {/*{{{*/
      unsigned recv_data_idx;

      if (a_conn->user_data == NULL)
      {
        recv_data_idx = hr_recv_data_list_s_append_blank(&this->recv_data_list);

        // - initialize recv data -
        hr_recv_data_s *recv_data = hr_recv_data_list_s_at(&this->recv_data_list,recv_data_idx);
        bc_array_s_swap(&recv_data->ip,&this->ip_buffer);

        a_conn->user_data = (void *)(uintptr_t)recv_data_idx; // NOLINT(performance-no-int-to-ptr)
      }
      else
      {
        recv_data_idx = (unsigned)(uintptr_t)a_conn->user_data;
      }

      hr_recv_data_s *recv_data = hr_recv_data_list_s_at(&this->recv_data_list,recv_data_idx);

      if (a_conn->upload_data != NULL)
      {
        // - append data to message buffer -
        bc_array_s_append(&recv_data->data,*a_conn->upload_data_size,a_conn->upload_data);
        *a_conn->upload_data_size = 0;
      }
      else
      {
        do
        {
          if (a_conn->conn_type == c_http_conn_type_POST)
          {
            // - some data are available -
            if (recv_data->data.used > 0)
            {
              if (regex_s_match_n(&this->url_regex,a_conn->url,3,&this->url_match_array) &&
                  this->url_match_array.used == 3)
              {
                regmatch_s *rm_cpid = regmatch_array_s_at(&this->url_match_array,1);
                regmatch_s *rm_type = regmatch_array_s_at(&this->url_match_array,2);

                // - retrieve upload time -
                time_s upload_time;
                if (clock_s_gettime(CLOCK_REALTIME,&upload_time))
                {
                  throw_error(RECV_GET_TIME_ERROR);
                }

                datetime_s dt;
                time_s_datetime(&upload_time,&dt);

                // - upload directory path -
                this->dir_buffer.used = 0;
                bc_array_s_append_format(&this->dir_buffer,"%s/%.*s",
                    this->last_config.upload_dir.data,
                    rm_cpid->rm_eo - rm_cpid->rm_so,a_conn->url + rm_cpid->rm_so,
                    dt.year,dt.month,dt.day,dt.hour,dt.min,dt.sec);

                // - create upload directory -
                if (mkdir(this->dir_buffer.data,0777) && errno != EEXIST)
                {
                  throw_error(RECV_UPLOAD_MAKE_DIR_ERROR);
                }

                // - upload file path -
                this->file_buffer.used = 0;
                bc_array_s_append_format(&this->file_buffer,
                    "%s/%.*s_%4.4hu%2.2hu%2.2huT%2.2hu%2.2hu%2.2hu_Event_%.*s.xml",
                    this->dir_buffer.data,
                    rm_cpid->rm_eo - rm_cpid->rm_so,a_conn->url + rm_cpid->rm_so,
                    dt.year,dt.month,dt.day,dt.hour,dt.min,dt.sec,
                    rm_type->rm_eo - rm_type->rm_so,a_conn->url + rm_type->rm_so);

                // - write upload file -
                CONT_INIT_CLEAR(file_s,file);
                if (file_s_open(&file,this->file_buffer.data,"w") ||
                    stream_s_write(&file,recv_data->data.data,recv_data->data.used) ||
                    stream_s_flush(&file) || (file_s_clear(&file), 0))
                {
                  throw_error(RECV_UPLOAD_FILE_WRITE_ERROR);
                }

                // - log message -
                log_info_2("recv_http %p, received, ip: %.*s, cpid: %.*s",
                    a_conn,recv_data->ip.used,recv_data->ip.data,
                    rm_cpid->rm_eo - rm_cpid->rm_so,a_conn->url + rm_cpid->rm_so);

                // - http response -
                if (http_resp_s_create_from_buffer(&resp,0,"",MHD_RESPMEM_PERSISTENT) ||
                    MHD_add_response_header(resp.response,"Connection","close") != MHD_YES ||
                    http_conn_s_queue_response(a_conn,MHD_HTTP_OK,&resp))
                {
                  throw_error(RECV_HTTP_CONN_QUEUE_RESPONSE_ERROR);
                }

                break;
              }
            }
          }

          HR_RECV_HTTP_CONNECTION_CALLBACK_BAD_REQUEST();
        } while(0);
      }
    }/*}}}*/
    break;
  default:
    {/*{{{*/
      HR_RECV_HTTP_CONNECTION_CALLBACK_BAD_REQUEST();
    }/*}}}*/
    break;
  }

  return 0;
}/*}}}*/

int hr_recv_http_completed_callback(http_conn_s *a_conn)
{/*{{{*/
  debug_message_6(fprintf(stderr,"hr_recv_http_completed_callback\n"));

  // - log message -
  log_info_2("recv_http %p, drop connection",a_conn);

  hr_daemon_s *this = a_conn->server->user_data;

  if (a_conn->user_data != NULL)
  {
    unsigned recv_data_idx = (unsigned)(uintptr_t)a_conn->user_data;

    hr_recv_data_s *recv_data = hr_recv_data_list_s_at(&this->recv_data_list,recv_data_idx);

    // - reset recv data -
    recv_data->ip.used = 0;
    recv_data->data.used = 0;

    hr_recv_data_list_s_remove(&this->recv_data_list,recv_data_idx);

    a_conn->user_data = NULL;
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
  libconf_hrl_init();

  g_name = "hr_daemon";
  g_conf = "hr_config.json";

  // - process command line arguments -
  if (argc > 1)
  {
    int arg_idx = 1;
    do {
      if (strncmp("--name=",argv[arg_idx],7) == 0)
      {
        g_name = argv[arg_idx] + 7;
      }
      else if (strncmp("--conf=",argv[arg_idx],7) == 0)
      {
        g_conf = argv[arg_idx] + 7;
      }
      else
      {
        cassert(0);
      }
    } while(++arg_idx < argc);
  }

  // - create process directories -
  cassert(system("mkdir -p -m 777 " PROCESS_RUN_DIR_PATH " " PROCESS_LOG_DIR_PATH) == 0); // NOLINT

  // - create process -
  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(process_s,process);
    cassert(process_s_create(&process,g_name) == 0);

    do {
      CONT_INIT_CLEAR(hr_daemon_s,daemon);

      if (hr_daemon_s_create(&daemon) ||
          hr_config_s_read_file(&daemon.config,g_conf))
      {
        break;
      }

      // - set configuration changed flag -
      daemon.config_changed_flag = 1;

      if (hr_daemon_s_run(&daemon))
      {
        // - ignore return value, always terminate -
      }

    } while(0);
  }

  libconf_hrl_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

