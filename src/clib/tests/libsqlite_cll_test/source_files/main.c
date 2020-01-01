
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

  // - sqlite_s_open -
  CONT_INIT(sqlite_s,sqlite_0);
  cassert(sqlite_s_open(&sqlite_0,"tests/libsqlite_cll_test/sqlite/db.sql") == 0);

  cassert(sqlite_s_compare(&sqlite_0,&sqlite_0));

  buffer.used = 0;
  sqlite_s_to_string(&sqlite_0,&buffer);

  // - sqlite_s_open_v2 -
  cassert(sqlite_s_open_v2(&sqlite_0,"tests/libsqlite_cll_test/sqlite/db.sql",
        SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX) == 0);

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

