
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  __sync_add_and_fetch(&g_terminate,1);
}/*}}}*/

int http_connection(http_conn_s *a_conn)
{/*{{{*/
  switch (a_conn->conn_type)
  {
  case c_http_conn_type_GET:
    {
      // - mhd response modes: -
      //
      // MHD_RESPMEM_PERSISTENT,
      // MHD_RESPMEM_MUST_FREE,
      // MHD_RESPMEM_MUST_COPY

      // - init response -
      CONT_INIT_CLEAR(http_resp_s,resp);

      // - file response -
      if (strcmp(a_conn->url,"/file") == 0)
      {
        int fd = open("tests/libmicrohttp_cll_test/resources/http_file.xml",O_RDONLY);
        cassert(fd != -1);

        // - retrieve file size -
        struct stat file_stat;
        cassert(fstat(fd,&file_stat) == 0);
        cassert(http_resp_s_create_from_fd(&resp,file_stat.st_size,fd) == 0);
      }

      // - dummy response -
      else
      {
        cassert(http_resp_s_create_from_buffer(&resp,
              strlen("GET Response\n"),"GET Response\n",MHD_RESPMEM_PERSISTENT) == 0);
      }

      cassert(http_conn_s_queue_response(a_conn,MHD_HTTP_OK,&resp) == 0);
    }
    break;
  case c_http_conn_type_POST:
    {
      // - upload data are available -
      if (a_conn->upload_data != NULL)
      {
        // - user data not exist yet -
        if (a_conn->user_data == NULL)
        {
          // - initialize user data buffer -
          a_conn->user_data = cmalloc(sizeof(bc_array_s));
          bc_array_s_init(a_conn->user_data);
        }

        // - append upload data to user data buffer -
        bc_array_s_append(a_conn->user_data,*a_conn->upload_data_size,a_conn->upload_data);
        *a_conn->upload_data_size = 0;
      }

      // - all upload data received -
      else
      {
        //bc_array_s *buffer = a_conn->user_data;

        // - dummy response -
        CONT_INIT_CLEAR(http_resp_s,resp);
        cassert(http_resp_s_create_from_buffer(&resp,
              strlen("POST Response\n"),"POST Response\n",MHD_RESPMEM_PERSISTENT) == 0);

        cassert(http_conn_s_queue_response(a_conn,MHD_HTTP_OK,&resp) == 0);
      }
    }
    break;
  }

  return 0;
}/*}}}*/

int http_completed(http_conn_s *a_conn)
{/*{{{*/

  // - user data exists -
  if (a_conn->user_data != NULL)
  {
    bc_array_s *buffer = a_conn->user_data;

    // - release user buffer -
    bc_array_s_clear(buffer);
    cfree(buffer);
  }

  return 0;
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  cassert(signal_s_simple_handler(signal_handler) == 0);

  {
    CONT_INIT_CLEAR(http_server_s,server);
    cassert(http_server_s_create(&server,8888,http_connection,http_completed,NULL) == 0);

    CONT_INIT_CLEAR(pollfd_array_s,pollfds);

    while (!g_terminate)
    {
      pollfds.used = 0;
      cassert(http_server_s_fds(&server,&pollfds) == 0);

      int timeout = -1;
      ulli nano_timeout;
      if (http_server_s_timeout(&server,&nano_timeout))
      {
        timeout = nano_timeout / 1000000ULL;
      }

      int res = poll(pollfds.data,pollfds.used,timeout);
      if (res >= 0)
      {
        cassert(http_server_s_process(&server) == 0);
      }
    }
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

