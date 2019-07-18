
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libuuid_cll_test";

const char *test_names[] =
{/*{{{*/
  "create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_create,
};/*}}}*/

// === methods of generated structures =========================================

// -- uuid_array_s --
@begin
methods uuid_array_s
@end

// === test execution functions ================================================

void test_create()
{/*{{{*/
  uuid_s uuid;
  uuid_s_generate(&uuid);

  CONT_INIT_CLEAR(uuid_array_s,array);
  unsigned idx = 0;
  do {
    uuid_array_s_push(&array,&uuid);
  } while(++idx < 10);

  CONT_INIT_CLEAR(bc_array_s,uuid_str);
  uuid_s_unparse(&uuid,&uuid_str);
  bc_array_s_push(&uuid_str,'\0');
  printf("%s\n",uuid_str.data);

  uuid_s uuid_parsed;
  cassert(uuid_s_parse(&uuid_parsed,uuid_str.data) == 0);

  CONT_INIT_CLEAR(bc_array_s,uuid_parsed_str);
  uuid_s_unparse(&uuid_parsed,&uuid_parsed_str);
  bc_array_s_push(&uuid_parsed_str,'\0');
  printf("%s\n",uuid_parsed_str.data);

  cassert(strcmp(uuid_str.data,uuid_parsed_str.data) == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

