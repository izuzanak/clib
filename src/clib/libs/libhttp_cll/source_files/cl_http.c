
@begin
include "cl_http.h"
@end

// === methods of generated structures =========================================

// -- pointer_list_s --
@begin
methods pointer_list_s
@end

// -- http_header_s --
@begin
methods http_header_s
@end

// -- http_header_tree_s --
@begin
methods http_header_tree_s
@end

// === methods of structure http_server_s ======================================

int http_server_s_connection_func(void *cls,struct MHD_Connection *connection,
    const char *url,const char *method,const char *version,
    const char *upload_data,size_t *upload_data_size,void **con_cls)
{/*{{{*/
  http_server_s *server = (http_server_s *)cls;

  // - connection pointer -
  http_conn_s *conn_ptr;

  if (*con_cls == NULL)
  {
    // - create http_connection object -
    conn_ptr = (http_conn_s *)cmalloc(sizeof(http_conn_s));
    http_conn_s_init(conn_ptr);

    // - set pointer to server -
    conn_ptr->server = server;

    // - retrieve connection type -
    unsigned conn_type;
    switch (strlen(method))
    {
    // - GET, PUT -
    case 3:
      conn_type = memcmp("GET",method,3) == 0 ?
        c_http_conn_type_GET : c_http_conn_type_PUT;
      break;

    // - HEAD, POST -
    case 4:
      conn_type = memcmp("HEAD",method,4) == 0 ?
        c_http_conn_type_HEAD : c_http_conn_type_POST;
      break;

    // - TRACE -
    case 5:
      conn_type = c_http_conn_type_TRACE;
      break;

    // - DELETE -
    case 6:
      conn_type = c_http_conn_type_DELETE;
      break;

    // - CONNECT, OPTIONS -
    case 7:
      conn_type = memcmp("CONNECT",method,7) == 0 ?
        c_http_conn_type_CONNECT : c_http_conn_type_OPTIONS;
      break;

    // - unrecognized connection type -
    default:
      conn_type = c_http_conn_type_NONE;
    }

    // - set connection properties -
    conn_ptr->conn_type = conn_type;
    conn_ptr->connection = connection;

    // - set user data pointer to blank location -
    conn_ptr->user_data = NULL;

    conn_ptr->url = url;
    conn_ptr->method = method;
    conn_ptr->version = version;

    // - create connection location -
    *con_cls = conn_ptr;

    return MHD_YES;
  }

  // - retrieve connection object -
  conn_ptr = (http_conn_s *)*con_cls;

  // - update upload data variables -
  conn_ptr->upload_data = upload_data;
  conn_ptr->upload_data_size = upload_data_size;

  // -----

  int res = server->connection_cb(conn_ptr);

  // - reset upload data variables -
  conn_ptr->upload_data = NULL;
  conn_ptr->upload_data_size = NULL;

  if (res)
  {
    server->ret_code = 1;
    return MHD_NO;
  }

  return MHD_YES;
}/*}}}*/

int conn_key_value_func(void *cls,enum MHD_ValueKind kind,
    const char *key,const char *value)
{/*{{{*/
  http_conn_s *conn_ptr = (http_conn_s *)cls;

  CONT_INIT_CLEAR(http_header_s,http_header);
  string_s_set(&http_header.key,strlen(key),key);
  string_s_set(&http_header.value,strlen(value),value);

  // - insert header to tree -
  http_header_tree_s_unique_swap_insert(conn_ptr->http_header_tree,&http_header);

  return MHD_YES;
}/*}}}*/

void http_server_s_completed_func(void *cls,struct MHD_Connection *connection,
    void **con_cls,enum MHD_RequestTerminationCode toe)
{/*{{{*/

  // - retrieve connection object -
  http_conn_s *conn_ptr = (http_conn_s *)*con_cls;

  // - if connection object exists -
  if (conn_ptr != NULL)
  {
    http_conn_s_clear(conn_ptr);
    cfree(conn_ptr);
  }
}/*}}}*/

int http_server_s_create(http_server_s *this,usi a_port,http_connection_cb_t a_connection_cb)
{/*{{{*/
  http_server_s_clear(this);

  // - start http server -
  struct MHD_Daemon *daemon = MHD_start_daemon(
      MHD_USE_SUSPEND_RESUME,a_port,NULL,NULL,
      http_server_s_connection_func,this,
      MHD_OPTION_NOTIFY_COMPLETED,http_server_s_completed_func,NULL,
      MHD_OPTION_END);

  // - ERROR -
  if (daemon == NULL)
  {
    throw_error(HTTP_SERVER_CANNOT_START_DAEMON);
  }

  // - set server daemon pointer -
  this->daemon = daemon;

  // - set server connection callback -
  this->connection_cb = a_connection_cb;

  return 0;
}/*}}}*/

int http_server_s_fds(http_server_s *this,pollfd_array_s *a_trg)
{/*{{{*/
  a_trg->used = 0;

  // - prepare file descriptor sets -
  fd_set rs; FD_ZERO(&rs);
  fd_set ws; FD_ZERO(&ws);
  fd_set es; FD_ZERO(&es);

  // - ERROR -
#if MHD_VERSION <= 0x00093300
  int max_fd = 0;
  if (MHD_YES != MHD_get_fdset(this->daemon,&rs,&ws,&es,&max_fd))
#else
  MHD_socket max_fd = 0;
  if (MHD_YES != MHD_get_fdset2(this->daemon,&rs,&ws,&es,&max_fd,FD_SETSIZE))
#endif
  {
    throw_error(HTTP_SERVER_INTERNAL_ERROR);
  }

  if (max_fd >= 0)
  {
#if MHD_VERSION <= 0x00093300
    int fd = 0;
#else
    MHD_socket fd = 0;
#endif
    do {
      pollfd_s pollfd = {-1,0,0};

      // - test fd in read fd_set -
      if (FD_ISSET(fd,&rs))
      {
        pollfd.fd = fd;
        pollfd.events |= POLLIN;
      }

      // - test fd in write fd_set -
      if (FD_ISSET(fd,&ws))
      {
        pollfd.fd = fd;
        pollfd.events |= POLLOUT;
      }

      // - test fd in except fd_set -
      if (FD_ISSET(fd,&es))
      {
        pollfd.fd = fd;
        pollfd.events |= POLLERR | POLLHUP | POLLNVAL;
      }

      if (pollfd.fd != -1)
      {
        pollfd_array_s_push(a_trg,&pollfd);
      }

    } while(++fd <= max_fd);
  }

  return 0;
}/*}}}*/

// === methods of structure http_conn_s ========================================

void http_conn_s_values(http_conn_s *this,enum MHD_ValueKind a_value_kind,http_header_tree_s *a_trg)
{/*{{{*/
  http_header_tree_s_clear(a_trg);

  // - retrieve key value locations -
  this->http_header_tree = a_trg;
  MHD_get_connection_values(this->connection,a_value_kind,&conn_key_value_func,this);
  this->http_header_tree = NULL;
}/*}}}*/

int http_conn_s_queue_response(http_conn_s *this,unsigned a_status_code,http_resp_s *a_resp)
{/*{{{*/

  // - queue response to be transmitted to client -
  int result = MHD_queue_response(this->connection,a_status_code,a_resp->response);

  // - ERROR -
  if (result != MHD_YES)
  {
    throw_error(HTTP_CONN_CANNOT_QUEUE_RESPONSE);
  }

  return 0;
}/*}}}*/

// === methods of structure http_resp_s ========================================

int http_resp_s_create_from_buffer_copy(http_resp_s *this,const bc_array_s *a_src)
{/*{{{*/
  http_resp_s_clear(this);

  // - create http_resp object -
  struct MHD_Response *response = MHD_create_response_from_buffer(
      a_src->used,a_src->data,MHD_RESPMEM_MUST_COPY);

  // - ERROR -
  if (response == NULL)
  {
    throw_error(HTTP_RESP_CREATE_ERROR);
  }

  this->response = response;

  return 0;
}/*}}}*/

int http_resp_s_create_from_fd(http_resp_s *this,off_t a_size,int a_fd)
{/*{{{*/

  // - create http_resp object -
  struct MHD_Response *response = MHD_create_response_from_fd(a_size,a_fd);

  // - ERROR -
  if (response == NULL)
  {
    throw_error(HTTP_RESP_CREATE_ERROR);
  }

  this->response = response;

  return 0;
}/*}}}*/

