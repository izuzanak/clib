
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libpsql_cll_test";

const char *test_names[] =
{/*{{{*/
  "create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_create,
};/*}}}*/

// === test execution functions ================================================

int psql_result(void *a_psql_conn,unsigned a_index,psql_result_s *a_result)
{/*{{{*/
  (void)a_psql_conn;
  (void)a_index;
  (void)a_result;

  return 0;
}/*}}}*/

int psql_notify(void *a_psql_conn,unsigned a_index,psql_notify_s *a_notify)
{/*{{{*/
  (void)a_psql_conn;
  (void)a_index;
  (void)a_notify;

  return 0;
}/*}}}*/

void test_create()
{/*{{{*/
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

