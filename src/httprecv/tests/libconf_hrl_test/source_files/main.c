
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libconf_hrl_test";

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

  // - hr_config_s_read_file -
  CONT_INIT(hr_config_s,hr_config);
  cassert(hr_config_s_read_file(&hr_config,
    "tests/libconf_hrl_test/resources/hr_config.json") == 0);
  DEBUG_PRINT(hr_config_s,&hr_config);

  hr_config_s_clear(&hr_config);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libconf_hrl_init(),libconf_hrl_clear());

  return 0;
}/*}}}*/

