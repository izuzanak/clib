
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libvalidator_cll_test";

const char *test_names[] =
{/*{{{*/
  "type",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_type,
};/*}}}*/

// === test execution functions ================================================

void test_type()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  VAR_CLEAR(str_test_type_var,loc_s_string_ptr("test_type"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test_type = loc_s_array();
  loc_s_dict_set(schema,str_test_type_var,test_type);

  loc_s_array_push(test_type,loc_s_string_ptr("type"));
  loc_s_array_push(test_type,loc_s_int(c_bi_type_integer));

  VAR_CLEAR(value_0,loc_s_int(10));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_type_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_type_var,value_1));

  buffer.used = 0;
  var_array_s_to_string(&validator.props_stack,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[test_type,type]") == 0);

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

