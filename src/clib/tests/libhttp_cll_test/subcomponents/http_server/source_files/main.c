
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
  CONT_INIT_CLEAR(http_header_tree_s,headers);
  http_conn_s_values(a_conn,MHD_HEADER_KIND,&headers);

  DEBUG_PRINT_LINES(http_header_tree_s,&headers);

  CONT_INIT_CLEAR(bc_array_s,data);
  bc_array_s_append_ptr(&data,"Hello world!!!");

  CONT_INIT_CLEAR(http_resp_s,resp);
  cassert(http_resp_s_create_from_buffer_copy(&resp,&data) == 0);

  cassert(http_conn_s_queue_response(a_conn,MHD_HTTP_OK,&resp) == 0);

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
    cassert(http_server_s_create(&server,8888,http_connection) == 0);

    CONT_INIT_CLEAR(pollfd_array_s,pollfd_array);

    while (!g_terminate)
    {
      cassert(http_server_s_fds(&server,&pollfd_array) == 0);
      
      int res = poll(pollfd_array.data,pollfd_array.used,100);
      if (res > 0)
      {
        cassert(http_server_s_process(&server) == 0);
      }
    }
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

