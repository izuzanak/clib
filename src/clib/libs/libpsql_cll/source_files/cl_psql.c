
@begin
include "cl_psql.h"
@end

// === methods of structure psql_conn_s ========================================

int psql_conn_s_create(psql_conn_s *this,
    const char *a_conninfo,
    psql_result_cb_t a_result_cb,
    psql_notify_cb_t a_notify_cb,
    void *a_cb_object,
    unsigned a_cb_index)
{/*{{{*/
  psql_conn_s_clear(this);

  // - connect to database -
  PGconn *conn_ptr = PQconnectdb(a_conninfo);

  // - ERROR -
  if (PQstatus(conn_ptr) != CONNECTION_OK)
  {
    //PQerrorMessage(conn_ptr);
    PQfinish(conn_ptr);

    throw_error(PSQL_CONN_DB_CONNECT_ERROR);
  }

  this->conn_ptr = conn_ptr;
  this->result_cb = a_result_cb;
  this->notify_cb = a_notify_cb;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;
  this->epoll_fd.fd = PQsocket(this->conn_ptr);

  // - ERROR -
  if (this->epoll_fd.fd == -1)
  {
    throw_error(PSQL_CONN_DB_CONNECT_ERROR);
  }

#ifdef LIBPQ_HAS_PIPELINING
  // - set nonblocking and pipeline mode -
  if (psql_conn_s_nonblocking(this,1) ||
      psql_conn_s_pipeline_mode(this,1))
  {
    throw_error(PSQL_CONN_DB_CONNECT_ERROR);
  }
#endif

  // - set fd callback -
  if (epoll_s_fd_callback(&this->epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,psql_conn_s_fd_event,this,0))
  {
    throw_error(PSQL_CONN_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int psql_conn_s_process(psql_conn_s *this)
{/*{{{*/

  // - ERROR -
  if (PQconsumeInput(this->conn_ptr) != 1)
  {
    //PQerrorMessage(conn_ptr);
    throw_error(PSQL_CONN_CONSUME_INPUT_ERROR);
  }

  // - ERROR -
  int flush_res = PQflush(this->conn_ptr);
  if (flush_res == -1)
  {
    throw_error(PSQL_CONN_FLUSH_ERROR);
  }

  if (epoll_fd_s_modify_events(&this->epoll_fd,EPOLLIN | EPOLLPRI | (flush_res ? EPOLLOUT : 0)))
  {
    throw_error(PSQL_CONN_EPOLL_ERROR);
  }

  // - retrieve results (before notifications) -
  while (!PQisBusy(this->conn_ptr))
  {
    CONT_INIT_CLEAR(psql_result_s,result);

    // - retrieve result -
    result.res_ptr = PQgetResult(this->conn_ptr);

    if (result.res_ptr)
    {
      // - call result callback -
      this->result_cb(this->cb_object,this->cb_index,&result);
      this->last_blank = 0;
    }
    else
    {
      if (this->last_blank)
      {
        break;
      }

      // - call result callback -
      this->result_cb(this->cb_object,this->cb_index,NULL);
      this->last_blank = 1;
    }
  }

  // - retrieve notifications (after results) -
  while (!PQisBusy(this->conn_ptr))
  {
    CONT_INIT_CLEAR(psql_notify_s,notify);

    // - retrieve notification -
    notify.ntf_ptr = PQnotifies(this->conn_ptr);

    if (notify.ntf_ptr == NULL) { break; }

    // - call notify callback -
    this->notify_cb(this->cb_object,this->cb_index,&notify);
  }

  return 0;
}/*}}}*/

int psql_conn_s_fd_event(void *a_psql_conn,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;
  (void)a_epoll_event;

  psql_conn_s *this = (psql_conn_s *)a_psql_conn;

  // - ERROR -
  if (psql_conn_s_process(this))
  {
    throw_error(PSQL_CONN_PROCESS_ERROR);
  }

  return 0;
}/*}}}*/

// === methods of structure psql_result_s ======================================

int psql_result_s_tuples(psql_result_s *this,var_arrays_s *a_tuples)
{/*{{{*/
  int tuple_cnt = PQntuples(this->res_ptr);
  if (tuple_cnt > 0)
  {
    var_arrays_s_reserve(a_tuples,tuple_cnt);

    int field_cnt = PQnfields(this->res_ptr);
    if (field_cnt > 0)
    {
      int tuple_idx = 0;
      do
      {
        var_arrays_s_push_blank(a_tuples);
        var_array_s *fields = var_arrays_s_last(a_tuples);
        var_array_s_reserve(fields,field_cnt);
        var_s value_var;

        int field_idx = 0;
        do
        {
          switch (PQftype(this->res_ptr,field_idx))
          {
          case BOOLOID:
            {
              long long int value = PQgetvalue(this->res_ptr,tuple_idx,field_idx)[0] == 't';
              value_var = loc_s_int(value);
            }
            break;
          case INT8OID:
          case INT2OID:
          case INT4OID:
            {
              char *data = PQgetvalue(this->res_ptr,tuple_idx,field_idx);
              long long int value = strtoll(data,NULL,10);
              value_var = loc_s_int(value);
            }
            break;
          case FLOAT4OID:
          case FLOAT8OID:
            {
              char *data = PQgetvalue(this->res_ptr,tuple_idx,field_idx);
              double value = strtod(data,NULL);
              value_var = loc_s_float(value);
            }
            break;
          default:
            {
              int length = PQgetlength(this->res_ptr,tuple_idx,field_idx);
              char *data = PQgetvalue(this->res_ptr,tuple_idx,field_idx);
              value_var = loc_s_string(length,data);
            }
            break;
          }

          var_array_s_push(fields,&value_var);
        }
        while(++field_idx < field_cnt);
      }
      while(++tuple_idx < tuple_cnt);
    }
  }

  return 0;
}/*}}}*/

