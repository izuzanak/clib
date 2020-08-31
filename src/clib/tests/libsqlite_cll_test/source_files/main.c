
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libsqlite_cll_test";

const char *test_names[] =
{/*{{{*/
  "sqlite",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_sqlite,
};/*}}}*/

// === methods of generated structures =========================================

// -- sqlite_array_s --
@begin
methods sqlite_array_s
@end

// === test execution functions ================================================

void test_sqlite()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define DB_PATH "/dev/shm/db.sql"
//#define DB_PATH "tests/libsqlite_cll_test/sqlite/db.sql"

  cassert(system("rm -f " DB_PATH) == 0); // NOLINT

  // - sqlite_s_open -
  CONT_INIT(sqlite_s,sqlite_0);
  cassert(sqlite_s_open(&sqlite_0,DB_PATH) == 0);
  cassert(sqlite_s_compare(&sqlite_0,&sqlite_0));

  // - sqlite_s_open_v2 -
  cassert(sqlite_s_open_v2(&sqlite_0,DB_PATH,
        SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX) == 0);

  CONT_INIT_CLEAR(var_s,result)

  // - create table -
  buffer.used = 0;
  bc_array_s_append_ptr(&buffer,"create table 'table_0' (idx int primary key,name string,age int);");
  cassert(sqlite_s_execute(&sqlite_0,buffer.data,buffer.used,&result) == 0);

  unsigned idx = 0;
  do {
    // - insert into table -
    buffer.used = 0;
    bc_array_s_append_format(&buffer,"insert into table_0(idx,name,age) values(%u,\"Someone Somebody\",37);",idx);
    cassert(sqlite_s_execute(&sqlite_0,buffer.data,buffer.used,&result) == 0);
  } while(++idx < 5);

  // - select from table -
  buffer.used = 0;
  bc_array_s_append_ptr(&buffer,"select * from table_0;");
  cassert(sqlite_s_execute(&sqlite_0,buffer.data,buffer.used,&result) == 0);

  buffer.used = 0;
  var_s_to_string(&result,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[[0,Someone Somebody,37],[1,Someone Somebody,37],[2,Someone Somebody,37],[3,Someone Somebody,37],[4,Someone Somebody,37]]") == 0);

  // - select from table -
  {
    CONT_INIT_CLEAR(sqlite_stmt_s,stmt_0);

    buffer.used = 0;
    bc_array_s_append_ptr(&buffer,"select * from table_0;");
    cassert(sqlite_s_prepare(&sqlite_0,buffer.data,buffer.used,&stmt_0) == 0);

    buffer.used = 0;
    sqlite_stmt_s_to_string(&stmt_0,&buffer);
    fprintf(stderr,"%.*s\n",buffer.used,buffer.data);
  }

  // - drop table -
  buffer.used = 0;
  bc_array_s_append_ptr(&buffer,"drop table table_0;");
  cassert(sqlite_s_execute(&sqlite_0,buffer.data,buffer.used,&result) == 0);

  sqlite_s_clear(&sqlite_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

