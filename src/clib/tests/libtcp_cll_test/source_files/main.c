
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libtcp_cll_test";

const char *test_names[] =
{/*{{{*/
  "tcp",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_tcp,
};/*}}}*/

// === methods of generated structures =========================================

// === test execution functions ================================================

void test_tcp()
{/*{{{*/
  CONT_INIT(string_array_s,arguments);
  string_array_s_push_ptr(&arguments,"./libtcp_cll_test_tcp_server");

  CONT_INIT(pid_s,server_pid);
  cassert(pid_s_execute(&server_pid,&arguments) == 0);
  usleep(100000);

  arguments.used = 0;
  string_array_s_push_ptr(&arguments,"./libtcp_cll_test_tcp_client");

  CONT_INIT(pid_s,client_pid);
  cassert(pid_s_execute(&client_pid,&arguments) == 0);
  usleep(1000000);

  cassert(pid_s_kill(&server_pid,SIGTERM) == 0);

  int status;
  cassert(waitpid(client_pid,&status,0) != -1 && status == 0);
  cassert(waitpid(server_pid,&status,0) != -1 && status == 0);

  pid_s_clear(&client_pid);
  pid_s_clear(&server_pid);
  string_array_s_clear(&arguments);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

