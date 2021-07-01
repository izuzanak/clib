
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libconf_wdl_test";

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

  // - wd_config_s_read_file -
  CONT_INIT(wd_config_s,wd_config);
  cassert(wd_config_s_read_file(&wd_config,
    "tests/libconf_wdl_test/resources/wd_config.json") == 0);
  DEBUG_PRINT(wd_config_s,&wd_config);

  wd_config_s_clear(&wd_config);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libconf_wdl_init(),libconf_wdl_clear());

  return 0;
}/*}}}*/

