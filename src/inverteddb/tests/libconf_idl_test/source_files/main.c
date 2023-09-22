
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libconf_idl_test";

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

  // - id_config_s_read_file -
  CONT_INIT(id_config_s,id_config);
  cassert(id_config_s_read_file(&id_config,
    "tests/libconf_idl_test/resources/id_config.json") == 0);
  DEBUG_PRINT(id_config_s,&id_config);

  id_config_s_clear(&id_config);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libconf_idl_init(),libconf_idl_clear());

  return 0;
}/*}}}*/

