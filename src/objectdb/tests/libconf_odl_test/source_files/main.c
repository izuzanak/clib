
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libconf_odl_test";

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

  // - od_config_s_read_file -
  CONT_INIT(od_config_s,od_config);
  cassert(od_config_s_read_file(&od_config,
    "tests/libconf_odl_test/resources/od_config.json") == 0);
  DEBUG_PRINT(od_config_s,&od_config);

  od_config_s_clear(&od_config);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libconf_odl_init(),libconf_odl_clear());

  return 0;
}/*}}}*/

