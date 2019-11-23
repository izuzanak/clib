
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libvalidator_cll_test";

const char *test_names[] =
{/*{{{*/
  "prop_type",
  "prop_equal",
  "prop_not_equal",
  "prop_lesser",
  "prop_greater",
  "prop_lesser_equal",
  "prop_greater_equal",
  "prop_length_equal",
  "prop_length_not_equal",
  "prop_length_lesser",
  "prop_length_greater",
  "prop_length_lesser_equal",
  "prop_length_greater_equal",
  "prop_reference",
  "prop_regex",
  "prop_items",
  "prop_opt_items",
  "prop_all_items",
  "prop_all_keys",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_prop_type,
  test_prop_equal,
  test_prop_not_equal,
  test_prop_lesser,
  test_prop_greater,
  test_prop_lesser_equal,
  test_prop_greater_equal,
  test_prop_length_equal,
  test_prop_length_not_equal,
  test_prop_length_lesser,
  test_prop_length_greater,
  test_prop_length_lesser_equal,
  test_prop_length_greater_equal,
  test_prop_reference,
  test_prop_regex,
  test_prop_items,
  test_prop_opt_items,
  test_prop_all_items,
  test_prop_all_keys,
};/*}}}*/

// === test execution functions ================================================

#define VAR_ARRAY_S_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  var_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

void test_prop_type()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("type"));
  loc_s_array_push(test,loc_s_int(c_bi_type_integer));

  VAR_CLEAR(value_0,loc_s_int(10));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_equal()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("=="));
  loc_s_array_push(test,loc_s_int(10));

  VAR_CLEAR(value_0,loc_s_int(10));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_not_equal()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("!="));
  loc_s_array_push(test,loc_s_int(10));

  VAR_CLEAR(value_0,loc_s_string_ptr("Hello"));
  VAR_CLEAR(value_1,loc_s_int(10));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_lesser()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("<"));
  loc_s_array_push(test,loc_s_int(10));

  VAR_CLEAR(value_0,loc_s_int(9));
  VAR_CLEAR(value_1,loc_s_int(10));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_greater()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr(">"));
  loc_s_array_push(test,loc_s_int(10));

  VAR_CLEAR(value_0,loc_s_int(11));
  VAR_CLEAR(value_1,loc_s_int(10));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_lesser_equal()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("<="));
  loc_s_array_push(test,loc_s_int(10));

  VAR_CLEAR(value_0,loc_s_int(10));
  VAR_CLEAR(value_1,loc_s_int(11));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_greater_equal()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr(">="));
  loc_s_array_push(test,loc_s_int(10));

  VAR_CLEAR(value_0,loc_s_int(10));
  VAR_CLEAR(value_1,loc_s_int(9));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_length_equal()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("length =="));
  loc_s_array_push(test,loc_s_int(11));

  VAR_CLEAR(value_0,loc_s_string_ptr("Hello world"));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello world!"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_length_not_equal()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("length !="));
  loc_s_array_push(test,loc_s_int(11));

  VAR_CLEAR(value_0,loc_s_string_ptr("Hello world!"));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello world"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_length_lesser()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("length <"));
  loc_s_array_push(test,loc_s_int(11));

  VAR_CLEAR(value_0,loc_s_string_ptr("Hello worl"));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello world"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_length_greater()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("length >"));
  loc_s_array_push(test,loc_s_int(11));

  VAR_CLEAR(value_0,loc_s_string_ptr("Hello world!"));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello world"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_length_lesser_equal()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("length <="));
  loc_s_array_push(test,loc_s_int(11));

  VAR_CLEAR(value_0,loc_s_string_ptr("Hello worl"));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello world"));
  VAR_CLEAR(value_2,loc_s_string_ptr("Hello world!"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_2));
}/*}}}*/

void test_prop_length_greater_equal()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("length >="));
  loc_s_array_push(test,loc_s_int(11));

  VAR_CLEAR(value_0,loc_s_string_ptr("Hello world!"));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello world"));
  VAR_CLEAR(value_2,loc_s_string_ptr("Hello worl"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_2));
}/*}}}*/

void test_prop_reference()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));
  VAR_CLEAR(str_test_ref_var,loc_s_string_ptr("test_ref"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test_ref = loc_s_array();
  loc_s_dict_set(schema,str_test_ref_var,test_ref);

  loc_s_array_push(test_ref,loc_s_string_ptr("=="));
  loc_s_array_push(test_ref,loc_s_int(10));

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("ref"));
  loc_s_array_push(test,loc_s_string_ptr("test_ref"));

  VAR_CLEAR(value_0,loc_s_int(10));
  VAR_CLEAR(value_1,loc_s_string_ptr("Hello"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
}/*}}}*/

void test_prop_regex()
{/*{{{*/
  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("regex"));
  loc_s_array_push(test,loc_s_string_ptr("^Hello[123]*$"));
  loc_s_array_push(test,loc_s_string_ptr("regex"));
  loc_s_array_push(test,loc_s_string_ptr("^.*$"));

  VAR_CLEAR(value_0,loc_s_string_ptr("Hello123"));
  VAR_CLEAR(value_1,loc_s_int(10));
  VAR_CLEAR(value_2,loc_s_string_ptr("Hello124"));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
  cassert(validator_s_validate(&validator,str_test_var,value_2));
}/*}}}*/

void test_prop_items()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("items"));
  var_s items = loc_s_array();
  loc_s_array_push(test,items);

  var_s item_0 = loc_s_array();
  loc_s_array_push(item_0,loc_s_string_ptr("=="));
  loc_s_array_push(item_0,loc_s_int(0));

  var_s item_1 = loc_s_array();
  loc_s_array_push(item_1,loc_s_string_ptr("=="));
  loc_s_array_push(item_1,loc_s_string_ptr("Hello"));

  loc_s_array_push(items,loc_s_int(0));
  loc_s_array_push(items,item_0);

  loc_s_array_push(items,loc_s_int(1));
  loc_s_array_push(items,item_0);

  loc_s_array_push(items,loc_s_int(2));
  loc_s_array_push(items,item_1);

  VAR_CLEAR(value_0,loc_s_array());
  loc_s_array_push(value_0,loc_s_int(0));
  loc_s_array_push(value_0,loc_s_int(0));
  loc_s_array_push(value_0,loc_s_string_ptr("Hello"));

  VAR_CLEAR(value_1,loc_s_array());
  loc_s_array_push(value_1,loc_s_int(0));
  loc_s_array_push(value_1,loc_s_int(0));
  loc_s_array_push(value_1,loc_s_int(0));

  VAR_CLEAR(value_2,loc_s_array());
  loc_s_array_push(value_2,loc_s_int(0));
  loc_s_array_push(value_2,loc_s_int(0));

  VAR_CLEAR(value_3,loc_s_dict());
  loc_s_dict_set(value_3,loc_s_int(0),loc_s_int(0));
  loc_s_dict_set(value_3,loc_s_int(1),loc_s_int(0));
  loc_s_dict_set(value_3,loc_s_int(2),loc_s_string_ptr("Hello"));

  VAR_CLEAR(value_4,loc_s_dict());
  loc_s_dict_set(value_4,loc_s_int(0),loc_s_int(0));
  loc_s_dict_set(value_4,loc_s_int(1),loc_s_int(1));
  loc_s_dict_set(value_4,loc_s_int(2),loc_s_string_ptr("Hello"));

  VAR_CLEAR(value_5,loc_s_dict());
  loc_s_dict_set(value_5,loc_s_int(0),loc_s_int(0));
  loc_s_dict_set(value_5,loc_s_int(1),loc_s_int(0));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
  VAR_ARRAY_S_TO_STRING(&validator.value_stack);
  cassert(strcmp(buffer.data,"[2]") == 0);
  VAR_ARRAY_S_TO_STRING(&validator.props_stack);
  cassert(strcmp(buffer.data,"[test,items,2,==]") == 0);

  cassert(validator_s_validate(&validator,str_test_var,value_2));
  VAR_ARRAY_S_TO_STRING(&validator.value_stack);
  cassert(strcmp(buffer.data,"[2]") == 0);
  VAR_ARRAY_S_TO_STRING(&validator.props_stack);
  cassert(strcmp(buffer.data,"[test,items,2]") == 0);

  cassert(validator_s_validate(&validator,str_test_var,value_3) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_4));
  VAR_ARRAY_S_TO_STRING(&validator.value_stack);
  cassert(strcmp(buffer.data,"[1]") == 0);
  VAR_ARRAY_S_TO_STRING(&validator.props_stack);
  cassert(strcmp(buffer.data,"[test,items,1,==]") == 0);

  cassert(validator_s_validate(&validator,str_test_var,value_5));
  VAR_ARRAY_S_TO_STRING(&validator.value_stack);
  cassert(strcmp(buffer.data,"[2]") == 0);
  VAR_ARRAY_S_TO_STRING(&validator.props_stack);
  cassert(strcmp(buffer.data,"[test,items,2]") == 0);

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_prop_opt_items()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("opt-items"));
  var_s opt_items = loc_s_array();
  loc_s_array_push(test,opt_items);

  var_s item_0 = loc_s_array();
  loc_s_array_push(item_0,loc_s_string_ptr("=="));
  loc_s_array_push(item_0,loc_s_int(0));

  var_s item_1 = loc_s_array();
  loc_s_array_push(item_1,loc_s_string_ptr("=="));
  loc_s_array_push(item_1,loc_s_string_ptr("Hello"));

  loc_s_array_push(opt_items,loc_s_int(0));
  loc_s_array_push(opt_items,item_0);

  loc_s_array_push(opt_items,loc_s_int(1));
  loc_s_array_push(opt_items,item_0);

  loc_s_array_push(opt_items,loc_s_int(2));
  loc_s_array_push(opt_items,item_1);

  VAR_CLEAR(value_1,loc_s_dict());
  loc_s_dict_set(value_1,loc_s_int(0),loc_s_int(0));
  loc_s_dict_set(value_1,loc_s_int(1),loc_s_int(0));
  loc_s_dict_set(value_1,loc_s_int(2),loc_s_string_ptr("Hello"));

  VAR_CLEAR(value_2,loc_s_dict());
  loc_s_dict_set(value_2,loc_s_int(0),loc_s_int(0));
  loc_s_dict_set(value_2,loc_s_int(1),loc_s_int(1));
  loc_s_dict_set(value_2,loc_s_int(2),loc_s_string_ptr("Hello"));

  VAR_CLEAR(value_3,loc_s_dict());
  loc_s_dict_set(value_3,loc_s_int(0),loc_s_int(0));
  loc_s_dict_set(value_3,loc_s_int(1),loc_s_int(0));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);

  cassert(validator_s_validate(&validator,str_test_var,value_1) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_2));
  VAR_ARRAY_S_TO_STRING(&validator.value_stack);
  cassert(strcmp(buffer.data,"[1]") == 0);
  VAR_ARRAY_S_TO_STRING(&validator.props_stack);
  cassert(strcmp(buffer.data,"[test,opt-items,1,==]") == 0);

  cassert(validator_s_validate(&validator,str_test_var,value_3) == 0);

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_prop_all_items()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("all-items"));
  var_s all_items = loc_s_array();
  loc_s_array_push(test,all_items);

  loc_s_array_push(all_items,loc_s_string_ptr("=="));
  loc_s_array_push(all_items,loc_s_int(10));

  VAR_CLEAR(value_0,loc_s_array());
  loc_s_array_push(value_0,loc_s_int(10));
  loc_s_array_push(value_0,loc_s_int(10));
  loc_s_array_push(value_0,loc_s_int(10));

  VAR_CLEAR(value_1,loc_s_array());
  loc_s_array_push(value_1,loc_s_int(10));
  loc_s_array_push(value_1,loc_s_int(10));
  loc_s_array_push(value_1,loc_s_int(11));

  VAR_CLEAR(value_2,loc_s_dict());
  loc_s_dict_set(value_2,loc_s_string_ptr("first"),loc_s_int(10));
  loc_s_dict_set(value_2,loc_s_string_ptr("second"),loc_s_int(10));
  loc_s_dict_set(value_2,loc_s_string_ptr("third"),loc_s_int(10));

  VAR_CLEAR(value_3,loc_s_dict());
  loc_s_dict_set(value_3,loc_s_string_ptr("first"),loc_s_int(10));
  loc_s_dict_set(value_3,loc_s_string_ptr("second"),loc_s_int(10));
  loc_s_dict_set(value_3,loc_s_string_ptr("third"),loc_s_int(11));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
  VAR_ARRAY_S_TO_STRING(&validator.value_stack);
  cassert(strcmp(buffer.data,"[2]") == 0);
  VAR_ARRAY_S_TO_STRING(&validator.props_stack);
  cassert(strcmp(buffer.data,"[test,all-items,2,==]") == 0);

  cassert(validator_s_validate(&validator,str_test_var,value_2) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_3));
  VAR_ARRAY_S_TO_STRING(&validator.value_stack);
  cassert(strcmp(buffer.data,"[third]") == 0);
  VAR_ARRAY_S_TO_STRING(&validator.props_stack);
  cassert(strcmp(buffer.data,"[test,all-items,third,==]") == 0);

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_prop_all_keys()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  VAR_CLEAR(str_test_var,loc_s_string_ptr("test"));

  VAR_CLEAR(schema,loc_s_dict());

  var_s test = loc_s_array();
  loc_s_dict_set(schema,str_test_var,test);

  loc_s_array_push(test,loc_s_string_ptr("all-keys"));
  var_s all_items = loc_s_array();
  loc_s_array_push(test,all_items);

  loc_s_array_push(all_items,loc_s_string_ptr("regex"));
  loc_s_array_push(all_items,loc_s_string_ptr("^[a-z]*$"));

  VAR_CLEAR(value_0,loc_s_dict());
  loc_s_dict_set(value_0,loc_s_string_ptr("first"),loc_s_int(10));
  loc_s_dict_set(value_0,loc_s_string_ptr("second"),loc_s_int(10));
  loc_s_dict_set(value_0,loc_s_string_ptr("third"),loc_s_int(10));

  VAR_CLEAR(value_1,loc_s_dict());
  loc_s_dict_set(value_1,loc_s_string_ptr("first"),loc_s_int(10));
  loc_s_dict_set(value_1,loc_s_string_ptr("second"),loc_s_int(10));
  loc_s_dict_set(value_1,loc_s_string_ptr("third1"),loc_s_int(10));

  CONT_INIT_CLEAR(validator_s,validator);
  cassert(validator_s_create(&validator,schema) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_0) == 0);
  cassert(validator_s_validate(&validator,str_test_var,value_1));
  VAR_ARRAY_S_TO_STRING(&validator.value_stack);
  cassert(strcmp(buffer.data,"[third1]") == 0);
  VAR_ARRAY_S_TO_STRING(&validator.props_stack);
  cassert(strcmp(buffer.data,"[test,all-keys,third1,regex]") == 0);

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

