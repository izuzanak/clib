
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libconf_sdl_test";

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

  // - sd_config_s_read_file -
  CONT_INIT(sd_config_s,sd_config);
  cassert(sd_config_s_read_file(&sd_config,
    "tests/libconf_sdl_test/resources/sd_config.json") == 0);
  DEBUG_PRINT(sd_config_s,&sd_config);

  sd_config_s_clear(&sd_config);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libconf_sdl_init(),libconf_sdl_clear());

  return 0;
}/*}}}*/

