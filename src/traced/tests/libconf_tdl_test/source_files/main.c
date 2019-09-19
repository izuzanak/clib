
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libconf_tdl_test";

const char *test_names[] =
{/*{{{*/
  "conf_parse",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_conf_parse,
};/*}}}*/

// === test execution functions ================================================

void test_conf_parse()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED

  // - td_config_s_read_file -
  CONT_INIT(td_config_s,td_config);
  cassert(td_config_s_read_file(&td_config,
    "tests/libconf_tdl_test/resources/td_config.json") == 0);
  DEBUG_PRINT(td_config_s,&td_config);

  td_config_s_clear(&td_config);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libconf_tdl_init(),libconf_tdl_clear());

  return 0;
}/*}}}*/

