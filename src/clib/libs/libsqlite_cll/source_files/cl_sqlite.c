
@begin
include "cl_sqlite.h"
@end

// === methods of structure sqlite_s ===========================================

int sqlite_s_execute(sqlite_s *this,
    const char *a_query,int a_query_size,var_s *a_trg)
{/*{{{*/
  sqlite3_stmt *stmt;

  // - ERROR -
  if (sqlite3_prepare_v2(*this,a_query,a_query_size,&stmt,NULL) != SQLITE_OK)
  {
    throw_error(SQLITE_CONN_EXECUTE_ERROR);
  }

  // - create result array -
  VAR_CLEAR(res_array,loc_s_array());

  int step_res = sqlite3_step(stmt);
  if (step_res == SQLITE_ROW)
  {
    int column_cnt = sqlite3_column_count(stmt);

    do {
      // - create row array -
      VAR_CLEAR(row_array,loc_s_array());
      loc_s_array_push(res_array,row_array);

      if (column_cnt > 0)
      {
        var_array_s_copy_resize(loc_s_array_value(row_array),column_cnt);

        int col_idx = 0;
        do {
          switch (sqlite3_column_type(stmt,col_idx))
          {
          case SQLITE_INTEGER:
            {
              sqlite3_int64 value = sqlite3_column_int64(stmt,col_idx);
              loc_s_array_push(row_array,loc_s_int(value));
            }
            break;
          case SQLITE_FLOAT:
            {
              double value = sqlite3_column_double(stmt,col_idx);
              loc_s_array_push(row_array,loc_s_float(value));
            }
            break;
          case SQLITE_TEXT:
            {
              const char *data = (const char *)sqlite3_column_text(stmt,col_idx);
              unsigned length = sqlite3_column_bytes(stmt,col_idx);
              loc_s_array_push(row_array,loc_s_string(length,data));
            }
            break;
          case SQLITE_BLOB:
            {
              const char *data = (const char *)sqlite3_column_blob(stmt,col_idx);
              unsigned length = sqlite3_column_bytes(stmt,col_idx);
              loc_s_array_push(row_array,loc_s_string(length,data));
            }
            break;
          case SQLITE_NULL:
            loc_s_array_push(row_array,loc_s_blank());
            break;
          }
        } while(++col_idx < column_cnt);
      }

      step_res = sqlite3_step(stmt);
    } while(step_res == SQLITE_ROW);
  }

  sqlite3_finalize(stmt);

  // - ERROR -
  if (step_res != SQLITE_DONE)
  {
    throw_error(SQLITE_CONN_EXECUTE_ERROR);
  }

  // - retrieve parsed value -
  var_s_copy(a_trg,&res_array);

  return 0;
}/*}}}*/

int sqlite_s_prepare(sqlite_s *this,
    const char *a_query,int a_query_size,sqlite_stmt_s *a_trg)
{/*{{{*/
  sqlite3_stmt *stmt;

  // - ERROR -
  if (sqlite3_prepare_v2(*this,a_query,a_query_size,&stmt,NULL) != SQLITE_OK)
  {
    throw_error(SQLITE_CONN_PREPARE_ERROR);
  }

  // - create sqlite statement object -
  sqlite_stmt_s_clear(a_trg);
  a_trg->stmt = stmt;

  return 0;
}/*}}}*/

