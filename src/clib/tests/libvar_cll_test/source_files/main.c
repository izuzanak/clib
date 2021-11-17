
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libvar_cll_test";

const char *test_names[] =
{/*{{{*/
  "var_blank",
  "var_integer",
  "var_float",
  "var_string",
  "var_array",
  "var_dict",
  "register_type",
  "from_var",
  "at_path",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_var_blank,
  test_var_integer,
  test_var_float,
  test_var_string,
  test_var_array,
  test_var_dict,
  test_register_type,
  test_from_var,
  test_at_path,
};/*}}}*/

// === methods of generated structures =========================================

// -- basic_tree_s --
@begin
methods basic_tree_s
@end

// -- basic_choice_s --
@begin
methods basic_choice_s
@end

// -- static_s --
@begin
methods static_s
@end

// -- static_array_s --
@begin
methods static_array_s
@end

// -- static_queue_s --
@begin
methods static_queue_s
@end

// -- static_list_s --
@begin
methods static_list_s
@end

// -- static_tree_s --
@begin
methods static_tree_s
@end

// -- dynamic_s --
@begin
methods dynamic_s
@end

// -- dynamic_array_s --
@begin
methods dynamic_array_s
@end

// -- dynamic_queue_s --
@begin
methods dynamic_queue_s
@end

// -- dynamic_list_s --
@begin
methods dynamic_list_s
@end

// -- dynamic_tree_s --
@begin
methods dynamic_tree_s
@end

// -- dynamic_choice_s --
@begin
methods dynamic_choice_s
@end

// -- person_s --
@begin
methods person_s
@end

unsigned g_type_person = c_idx_not_exist;

// === test execution functions ================================================

#define VAR_S_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  var_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

void test_var_blank()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define BLANK_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  loc_s_blank_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - loc_s_blank -
  // - loc_s_blank_clear -
  VAR(blank_0,loc_s_blank());
  BLANK_TO_STRING(blank_0);
  cassert(strcmp(buffer.data,"<blank>") == 0);

  // - loc_s_blank_order -
  VAR(blank_1,blank_0);
  BLANK_TO_STRING(blank_1);
  cassert(
    loc_s_blank_order(blank_0,blank_1) == 0 &&
    var_s_compare(&blank_0,&blank_1) &&
    strcmp(buffer.data,"<blank>") == 0);

  var_s_clear(&blank_1);
  var_s_clear(&blank_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_var_integer()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define INTEGER_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  loc_s_int_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - loc_s_int -
  // - loc_s_int_clear -
  VAR(integer_0,loc_s_int(10));
  INTEGER_TO_STRING(integer_0);
  cassert(strcmp(buffer.data,"10") == 0);

  // - loc_s_int_order -
  CONT_INIT(var_s,integer_1);
  var_s_copy(&integer_1,&integer_0);
  INTEGER_TO_STRING(integer_1);
  cassert(
    loc_s_int_order(integer_0,integer_1) == 0 &&
    var_s_compare(&integer_0,&integer_1) &&
    strcmp(buffer.data,"10") == 0);

  // - loc_s_int_value -
  cassert(loc_s_int_value(integer_0) == 10);

  // - loc_s_int_order -
  VAR(integer_2,loc_s_int(9));
  VAR(integer_3,loc_s_int(11));
  cassert(
    loc_s_int_order(integer_2,integer_0) < 0 &&
    loc_s_int_order(integer_3,integer_0) > 0);

  var_s_clear(&integer_3);
  var_s_clear(&integer_2);
  var_s_clear(&integer_1);
  var_s_clear(&integer_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_var_float()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define FLOAT_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  loc_s_float_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - loc_s_float -
  // - loc_s_float_clear -
  VAR(float_0,loc_s_float(1.234));
  FLOAT_TO_STRING(float_0);
  cassert(strcmp(buffer.data,"1.234000") == 0);

  // - loc_s_float_order -
  CONT_INIT(var_s,float_1);
  var_s_copy(&float_1,&float_0);
  FLOAT_TO_STRING(float_1);
  cassert(
    loc_s_float_order(float_0,float_1) == 0 &&
    var_s_compare(&float_0,&float_1) &&
    strcmp(buffer.data,"1.234000") == 0);

  // - loc_s_float_value -
  cassert(loc_s_float_value(float_0) == 1.234);

  // - loc_s_float_order -
  VAR(float_2,loc_s_float(1.230));
  VAR(float_3,loc_s_float(1.235));
  cassert(
    loc_s_float_order(float_2,float_0) < 0 &&
    loc_s_float_order(float_3,float_0) > 0);

  var_s_clear(&float_3);
  var_s_clear(&float_2);
  var_s_clear(&float_1);
  var_s_clear(&float_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_var_string()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define STRING_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  loc_s_string_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - loc_s_string -
  // - loc_s_string_clear -
  VAR(string_0,loc_s_string_ptr("Hello world!"));
  STRING_TO_STRING(string_0);
  cassert(strcmp(buffer.data,"Hello world!") == 0);

  // - loc_s_string_order -
  CONT_INIT(var_s,string_1);
  var_s_copy(&string_1,&string_0);
  STRING_TO_STRING(string_1);
  cassert(
    loc_s_string_order(string_0,string_1) == 0 &&
    var_s_compare(&string_0,&string_1) &&
    strcmp(buffer.data,"Hello world!") == 0);

  // - loc_s_string_value -
  const string_s *string = loc_s_string_value(string_0);
  cassert(string->size == 13 &&
    strcmp(string->data,"Hello world!") == 0);

  // - loc_s_string_order -
  VAR(string_2,loc_s_string_ptr("Hello world"));
  VAR(string_3,loc_s_string_ptr("Hello there world"));
  cassert(
    loc_s_string_order(string_2,string_0) < 0 &&
    loc_s_string_order(string_3,string_0) > 0);

  var_s_clear(&string_3);
  var_s_clear(&string_2);
  var_s_clear(&string_1);
  var_s_clear(&string_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_var_array()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define ARRAY_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  loc_s_array_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - loc_s_array -
  // - loc_s_array_clear -
  // - loc_s_array_push -
  VAR(array_0,loc_s_array());

  int idx = -5;
  do {
    loc_s_array_push(array_0,loc_s_int(idx));
  } while(++idx < 5);

  ARRAY_TO_STRING(array_0);
  cassert(strcmp(buffer.data,"[-5,-4,-3,-2,-1,0,1,2,3,4]") == 0);

  // - loc_s_array_order -
  CONT_INIT(var_s,array_1);
  var_s_copy(&array_1,&array_0);
  ARRAY_TO_STRING(array_1);
  cassert(
    loc_s_array_order(array_0,array_1) == 0 &&
    var_s_compare(&array_0,&array_1) &&
    strcmp(buffer.data,"[-5,-4,-3,-2,-1,0,1,2,3,4]") == 0);

  // - loc_s_array_length -
  cassert(loc_s_array_length(array_0) == 10);

  // - loc_s_array_at -
  // - loc_s_array_push -
  VAR(array_2,loc_s_array());
  unsigned uidx = 0;
  do {
    loc_s_array_push(array_2,*loc_s_array_at(array_0,uidx));
  } while(++uidx < loc_s_array_length(array_0));
  ARRAY_TO_STRING(array_2);
  cassert(strcmp(buffer.data,"[-5,-4,-3,-2,-1,0,1,2,3,4]") == 0);

  // - loc_s_array_pop -
  // - loc_s_array_push -
  VAR(array_3,loc_s_array());
  do {
    loc_s_array_push(array_3,loc_s_array_pop(array_0));
  } while(loc_s_array_length(array_0));
  ARRAY_TO_STRING(array_0);
  cassert(strcmp(buffer.data,"[]") == 0);
  ARRAY_TO_STRING(array_3);
  cassert(strcmp(buffer.data,"[4,3,2,1,0,-1,-2,-3,-4,-5]") == 0);

#if OPTION_TO_JSON == ENABLED
  // - loc_s_array_to_json -
  buffer.used = 0;
  loc_s_array_to_json(array_3,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[4,3,2,1,0,-1,-2,-3,-4,-5]") == 0);

  // - loc_s_array_to_json_nice -
  CONT_INIT(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

  buffer.used = 0;
  loc_s_array_to_json_nice(array_3,&json_nice,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
"[\n"
"==--4,\n"
"==--3,\n"
"==--2,\n"
"==--1,\n"
"==--0,\n"
"==---1,\n"
"==---2,\n"
"==---3,\n"
"==---4,\n"
"==---5\n"
"==]") == 0);

  json_nice_s_clear(&json_nice);
#endif

  // - loc_s_array_order -
  cassert(
    loc_s_array_order(array_2,array_3) < 0 &&
    loc_s_array_order(array_3,array_2) > 0);

  var_s_clear(&array_3);
  var_s_clear(&array_2);
  var_s_clear(&array_1);
  var_s_clear(&array_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_var_dict()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define DICT_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  loc_s_dict_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - loc_s_dict -
  // - loc_s_dict_clear -
  // - loc_s_dict_set -
  VAR(dict_0,loc_s_dict());
  int idx = 0;
  do {
    char ch = 'A' + idx;

    VAR(key,loc_s_int(idx));
    loc_s_dict_set(dict_0,key,loc_s_string(1,&ch));
    var_s_clear(&key);
  } while(++idx < 10);
  DICT_TO_STRING(dict_0);
  cassert(strcmp(buffer.data,"[0:A,1:B,2:C,3:D,4:E,5:F,6:G,7:H,8:I,9:J]") == 0);

  // - loc_s_dict_get -
  buffer.used = 0;
  idx = 10;
  while(--idx >= 0)
  {
    VAR(key,loc_s_int(idx));
    loc_s_to_string(loc_s_dict_get(dict_0,key),&buffer);
    var_s_clear(&key);
  }
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"JIHGFEDCBA") == 0);

  // - loc_s_dict_has_key -
  VAR(zero,loc_s_int(0));
  VAR(one,loc_s_int(1));
  idx = 5;
  do {
    VAR(key,loc_s_int(idx));

    if (loc_s_dict_has_key(dict_0,key)) {
      loc_s_dict_set(dict_0,key,one);
    }
    else {
      loc_s_dict_set(dict_0,key,zero);
    }

    var_s_clear(&key);
  } while(++idx < 15);
  DICT_TO_STRING(dict_0);
  cassert(strcmp(buffer.data,"[0:A,1:B,2:C,3:D,4:E,5:1,6:1,7:1,8:1,9:1,10:0,11:0,12:0,13:0,14:0]") == 0);
  var_s_clear(&one);
  var_s_clear(&zero);

  // - loc_s_dict_remove_key -
  idx = 5;
  do {
    VAR(key,loc_s_int(idx));
    loc_s_dict_remove_key(dict_0,key);
    var_s_clear(&key);
  } while(++idx < 10);
  DICT_TO_STRING(dict_0);
  cassert(strcmp(buffer.data,"[0:A,1:B,2:C,3:D,4:E,10:0,11:0,12:0,13:0,14:0]") == 0);

  VAR_CLEAR(dict_1,loc_s_dict());
  loc_s_dict_set(dict_1,loc_s_string_ptr("first"),loc_s_int(1));
  loc_s_dict_set(dict_1,loc_s_string_ptr("second"),loc_s_int(2));
  loc_s_dict_set(dict_1,loc_s_string_ptr("third"),loc_s_int(3));

  // - loc_s_dict_first_key -
  var_s dict_0_first_key = loc_s_dict_first_key(dict_0);
  cassert(loc_s_int_value(dict_0_first_key) == 0);

  var_s dict_1_first_key = loc_s_dict_first_key(dict_1);
  cassert(strcmp(loc_s_string_value(dict_1_first_key)->data,"first") == 0);

#if OPTION_TO_JSON == ENABLED
  VAR_CLEAR(dict_2,loc_s_dict());
  loc_s_dict_set(dict_2,loc_s_string_ptr("first"),loc_s_int(1));
  loc_s_dict_set(dict_2,loc_s_string_ptr("second"),loc_s_int(2));
  loc_s_dict_set(dict_2,loc_s_string_ptr("third"),loc_s_int(3));

  loc_s_dict_set(dict_1,loc_s_string_ptr("fourth"),dict_2);

  // - loc_s_dict_to_json -
  buffer.used = 0;
  loc_s_dict_to_json(dict_1,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"{\"first\":1,\"second\":2,\"third\":3,\"fourth\":{\"first\":1,\"second\":2,\"third\":3}}") == 0);

  // - loc_s_dict_to_json_nice -
  CONT_INIT(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

  buffer.used = 0;
  loc_s_dict_to_json_nice(dict_1,&json_nice,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
"{\n"
"==--\"first\": 1,\n"
"==--\"second\": 2,\n"
"==--\"third\": 3,\n"
"==--\"fourth\": {\n"
"==----\"first\": 1,\n"
"==----\"second\": 2,\n"
"==----\"third\": 3\n"
"==--}\n"
"==}") == 0);

  json_nice_s_clear(&json_nice);
#endif

  var_s_clear(&dict_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_register_type()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define VAR_ARRAY_S_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  var_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - loc_s_register_type -
  g_type_person = loc_s_register_type(
    loc_s_person_clear,
    loc_s_person_order,
#if OPTION_TO_STRING == ENABLED // NOLINT(readability-redundant-preprocessor)
    loc_s_person_to_string,
#else
    NULL,
#endif
#if OPTION_TO_JSON == ENABLED
    loc_s_person_to_json,
    loc_s_person_to_json_nice
#else
    NULL,
    NULL
#endif
    );

  VAR(person,loc_s_person("Frank","Sobotka",45));
  VAR_S_TO_STRING(&person);
  cassert(strcmp(buffer.data,"{name:Frank,surname:Sobotka,age:45}") == 0);

  CONT_INIT(var_array_s,array_0);
  var_array_s_copy_resize(&array_0,4);
  var_array_s_fill(&array_0,&person);
  VAR_ARRAY_S_TO_STRING(&array_0);
  cassert(strcmp(buffer.data,"[{name:Frank,surname:Sobotka,age:45},{name:Frank,surname:Sobotka,age:45},{name:Frank,surname:Sobotka,age:45},{name:Frank,surname:Sobotka,age:45}]") == 0);

#if OPTION_TO_JSON == ENABLED
  // - var_array_s_to_json -
  buffer.used = 0;
  var_array_s_to_json(&array_0,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[{\"name\":\"Frank\",\"surname\":\"Sobotka\",\"age\":45},{\"name\":\"Frank\",\"surname\":\"Sobotka\",\"age\":45},{\"name\":\"Frank\",\"surname\":\"Sobotka\",\"age\":45},{\"name\":\"Frank\",\"surname\":\"Sobotka\",\"age\":45}]") == 0);

  // - var_array_s_to_json_nice -
  CONT_INIT(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

  buffer.used = 0;
  var_array_s_to_json_nice(&array_0,&json_nice,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
"[\n"
"==--{\n"
"==----\"name\": \"Frank\",\n"
"==----\"surname\": \"Sobotka\",\n"
"==----\"age\": 45\n"
"==--},\n"
"==--{\n"
"==----\"name\": \"Frank\",\n"
"==----\"surname\": \"Sobotka\",\n"
"==----\"age\": 45\n"
"==--},\n"
"==--{\n"
"==----\"name\": \"Frank\",\n"
"==----\"surname\": \"Sobotka\",\n"
"==----\"age\": 45\n"
"==--},\n"
"==--{\n"
"==----\"name\": \"Frank\",\n"
"==----\"surname\": \"Sobotka\",\n"
"==----\"age\": 45\n"
"==--}\n"
"==]") == 0);

  json_nice_s_clear(&json_nice);
#endif

  var_array_s_clear(&array_0);
  var_s_clear(&person);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_from_var()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
#if OPTION_FROM_VAR == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

  VAR_CLEAR(integer_var,loc_s_int(125));
  VAR_CLEAR(float_var,loc_s_float(125.123));

#define TEST_INTEGER_FROM_VAR(TYPE) \
  TYPE TYPE ## _value;\
  cassert(TYPE ## _from_var(&TYPE ## _value,integer_var) == 0);\
  cassert(TYPE ## _value == 125);

  TEST_INTEGER_FROM_VAR(bc);
  TEST_INTEGER_FROM_VAR(uc);
  TEST_INTEGER_FROM_VAR(si);
  TEST_INTEGER_FROM_VAR(usi);
  TEST_INTEGER_FROM_VAR(bi);
  TEST_INTEGER_FROM_VAR(ui);
  TEST_INTEGER_FROM_VAR(lli);
  TEST_INTEGER_FROM_VAR(ulli);

#define TEST_FLOAT_FROM_VAR(TYPE) \
  TYPE TYPE ## _value;\
  cassert(TYPE ## _from_var(&TYPE ## _value,integer_var) == 0);\
  cassert(TYPE ## _value == 125);\
  cassert(TYPE ## _from_var(&TYPE ## _value,float_var) == 0);\
  cassert(TYPE ## _value == (TYPE)125.123);

  TEST_FLOAT_FROM_VAR(bf);
  TEST_FLOAT_FROM_VAR(bd);

  // - var_s_from_var -
  CONT_INIT_CLEAR(var_s,var);
  cassert(var_s_from_var(&var,float_var) == 0);

  buffer.used = 0;
  var_s_to_string(&var,&buffer);
  cassert(strncmp(buffer.data,"125.123000",buffer.used) == 0);

  // - string_s_from_var -
  VAR_CLEAR(string_var,loc_s_string_ptr("Hello world!!!"));

  CONT_INIT_CLEAR(string_s,string);
  cassert(string_s_from_var(&string,string_var) == 0);
  cassert(strcmp(string.data,"Hello world!!!") == 0);

  // - basic_tree_s_from_var -
  VAR_CLEAR(basic_array_var,loc_s_array());

  unsigned idx = 0;
  do {
    loc_s_array_push(basic_array_var,loc_s_int(idx));
  } while(++idx < 10);

  CONT_INIT_CLEAR(basic_tree_s,basic_tree);
  cassert(basic_tree_s_from_var(&basic_tree,basic_array_var) == 0);

  buffer.used = 0;
  basic_tree_s_to_string(&basic_tree,&buffer);
  cassert(strncmp(buffer.data,"[0,1,2,3,4,5,6,7,8,9]",buffer.used) == 0);

  // - basic_choice_s from_var -
  VAR_CLEAR(basic_choice_var_0,loc_s_dict());
  loc_s_dict_set(basic_choice_var_0,loc_s_string_ptr("integer"),loc_s_int(11));

  CONT_INIT_CLEAR(basic_choice_s,basic_choice_0);
  cassert(basic_choice_s_from_var(&basic_choice_0,basic_choice_var_0) == 0);

  buffer.used = 0;
  basic_choice_s_to_string(&basic_choice_0,&buffer);
  cassert(strncmp(buffer.data,"{integer:11}",buffer.used) == 0);

  // - static_s_from_var -
  VAR_CLEAR(static_var,loc_s_dict());
  loc_s_dict_set(static_var,loc_s_string_ptr("first"),loc_s_int(11));
  loc_s_dict_set(static_var,loc_s_string_ptr("second"),loc_s_int(12));
  loc_s_dict_set(static_var,loc_s_string_ptr("third"),loc_s_int(13));

  CONT_INIT_CLEAR(static_s,static_0);
  cassert(static_s_from_var(&static_0,static_var) == 0);

  buffer.used = 0;
  static_s_to_string(&static_0,&buffer);
  cassert(strncmp(buffer.data,"{first:11,second:12,third:13}",buffer.used) == 0);

  // - static_array_s_from_var -
  VAR_CLEAR(static_array_var,loc_s_array());

  idx = 0;
  do {
    loc_s_array_push(static_array_var,static_var);
  } while(++idx < 3);

  CONT_INIT_CLEAR(static_array_s,static_array);
  cassert(static_array_s_from_var(&static_array,static_array_var) == 0);

  buffer.used = 0;
  static_array_s_to_string(&static_array,&buffer);
  cassert(strncmp(buffer.data,
        "[{first:11,second:12,third:13},{first:11,second:12,third:13},{first:11,second:12,third:13}]",buffer.used) == 0);

  // - static_queue_s_from_var -
  CONT_INIT_CLEAR(static_queue_s,static_queue);
  cassert(static_queue_s_from_var(&static_queue,static_array_var) == 0);

  buffer.used = 0;
  static_queue_s_to_string(&static_queue,&buffer);
  cassert(strncmp(buffer.data,
        "[{first:11,second:12,third:13},{first:11,second:12,third:13},{first:11,second:12,third:13}]",buffer.used) == 0);

  // - static_list_s_from_var -
  CONT_INIT_CLEAR(static_list_s,static_list);
  cassert(static_list_s_from_var(&static_list,static_array_var) == 0);

  buffer.used = 0;
  static_list_s_to_string(&static_list,&buffer);
  cassert(strncmp(buffer.data,
        "[{first:11,second:12,third:13},{first:11,second:12,third:13},{first:11,second:12,third:13}]",buffer.used) == 0);

  // - static_tree_s_from_var -
  CONT_INIT_CLEAR(static_tree_s,static_tree);
  cassert(static_tree_s_from_var(&static_tree,static_array_var) == 0);

  buffer.used = 0;
  static_tree_s_to_string(&static_tree,&buffer);
  cassert(strncmp(buffer.data,
        "[{first:11,second:12,third:13},{first:11,second:12,third:13},{first:11,second:12,third:13}]",buffer.used) == 0);

  // - dynamic_s_from_var -
  loc_s_array_pop(static_array_var);
  loc_s_array_pop(static_array_var);

  VAR_CLEAR(dynamic_var,loc_s_dict());
  loc_s_dict_set(dynamic_var,loc_s_string_ptr("first"),loc_s_int(11));
  loc_s_dict_set(dynamic_var,loc_s_string_ptr("second"),loc_s_int(12));
  loc_s_dict_set(dynamic_var,loc_s_string_ptr("array"),static_array_var);

  CONT_INIT_CLEAR(dynamic_s,dynamic);
  cassert(dynamic_s_from_var(&dynamic,dynamic_var) == 0);

  buffer.used = 0;
  dynamic_s_to_string(&dynamic,&buffer);
  cassert(strncmp(buffer.data,"{first:11,second:12,array:[{first:11,second:12,third:13}]}",buffer.used) == 0);

  // - dynamic_array_s_from_var -
  VAR_CLEAR(dynamic_array_var,loc_s_array());

  idx = 0;
  do {
    loc_s_array_push(dynamic_array_var,dynamic_var);
  } while(++idx < 3);

  CONT_INIT_CLEAR(dynamic_array_s,dynamic_array);
  cassert(dynamic_array_s_from_var(&dynamic_array,dynamic_array_var) == 0);

  buffer.used = 0;
  dynamic_array_s_to_string(&dynamic_array,&buffer);
  cassert(strncmp(buffer.data,
        "[{first:11,second:12,array:[{first:11,second:12,third:13}]},"
        "{first:11,second:12,array:[{first:11,second:12,third:13}]},"
        "{first:11,second:12,array:[{first:11,second:12,third:13}]}]",
        buffer.used) == 0);

  // - dynamic_queue_s_from_var -
  CONT_INIT_CLEAR(dynamic_queue_s,dynamic_queue);
  cassert(dynamic_queue_s_from_var(&dynamic_queue,dynamic_array_var) == 0);

  buffer.used = 0;
  dynamic_queue_s_to_string(&dynamic_queue,&buffer);
  cassert(strncmp(buffer.data,
        "[{first:11,second:12,array:[{first:11,second:12,third:13}]},"
        "{first:11,second:12,array:[{first:11,second:12,third:13}]},"
        "{first:11,second:12,array:[{first:11,second:12,third:13}]}]",
        buffer.used) == 0);

  // - dynamic_list_s_from_var -
  CONT_INIT_CLEAR(dynamic_list_s,dynamic_list);
  cassert(dynamic_list_s_from_var(&dynamic_list,dynamic_array_var) == 0);

  buffer.used = 0;
  dynamic_list_s_to_string(&dynamic_list,&buffer);
  cassert(strncmp(buffer.data,
        "[{first:11,second:12,array:[{first:11,second:12,third:13}]},"
        "{first:11,second:12,array:[{first:11,second:12,third:13}]},"
        "{first:11,second:12,array:[{first:11,second:12,third:13}]}]",
        buffer.used) == 0);

  // - dynamic_tree_s_from_var -
  CONT_INIT_CLEAR(dynamic_tree_s,dynamic_tree);
  cassert(dynamic_tree_s_from_var(&dynamic_tree,dynamic_array_var) == 0);

  buffer.used = 0;
  dynamic_tree_s_to_string(&dynamic_tree,&buffer);
  cassert(strncmp(buffer.data,
        "[{first:11,second:12,array:[{first:11,second:12,third:13}]},"
        "{first:11,second:12,array:[{first:11,second:12,third:13}]},"
        "{first:11,second:12,array:[{first:11,second:12,third:13}]}]",
        buffer.used) == 0);

  // - dynamic_choice_s from_var -
  VAR_CLEAR(dynamic_choice_var_0,loc_s_dict());
  loc_s_dict_set(dynamic_choice_var_0,loc_s_string_ptr("data"),dynamic_var);

  CONT_INIT_CLEAR(dynamic_choice_s,dynamic_choice_0);
  cassert(dynamic_choice_s_from_var(&dynamic_choice_0,dynamic_choice_var_0) == 0);

  buffer.used = 0;
  dynamic_choice_s_to_string(&dynamic_choice_0,&buffer);
  cassert(strncmp(buffer.data,"{data:{first:11,second:12,array:[{first:11,second:12,third:13}]}}",
        buffer.used) == 0);
#endif
#endif
}/*}}}*/

void test_at_path()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

  VAR_CLEAR(dict_1,loc_s_dict());
  loc_s_dict_set(dict_1,loc_s_string_ptr("first"),loc_s_int(1));
  loc_s_dict_set(dict_1,loc_s_string_ptr("second"),loc_s_int(2));
  loc_s_dict_set(dict_1,loc_s_string_ptr("third"),loc_s_int(3));

  VAR_CLEAR(dict_2,loc_s_dict());
  loc_s_dict_set(dict_2,loc_s_string_ptr("first"),loc_s_int(4));
  loc_s_dict_set(dict_2,loc_s_string_ptr("second"),loc_s_int(5));
  loc_s_dict_set(dict_2,loc_s_string_ptr("third"),loc_s_int(6));

  loc_s_dict_set(dict_1,loc_s_string_ptr("fourth"),dict_2);

  VAR_CLEAR(array_1,loc_s_array());
  loc_s_array_push(array_1,loc_s_string_ptr("one"));
  loc_s_array_push(array_1,loc_s_string_ptr("two"));
  loc_s_array_push(array_1,loc_s_string_ptr("three"));

  loc_s_dict_set(dict_1,loc_s_string_ptr("fifth"),array_1);

  cassert(loc_s_int_value(loc_s_at_path(dict_1,"first")) == 1);
  cassert(loc_s_int_value(loc_s_at_path(dict_1,"second")) == 2);
  cassert(loc_s_int_value(loc_s_at_path(dict_1,"third")) == 3);

  cassert(loc_s_int_value(loc_s_at_path(dict_1,"fourth/first")) == 4);
  cassert(loc_s_int_value(loc_s_at_path(dict_1,"fourth/second")) == 5);
  cassert(loc_s_int_value(loc_s_at_path(dict_1,"fourth/third")) == 6);

  cassert(strcmp(loc_s_string_value(loc_s_at_path(dict_1,"fifth/0"))->data,"one") == 0);
  cassert(strcmp(loc_s_string_value(loc_s_at_path(dict_1,"fifth/1"))->data,"two") == 0);
  cassert(strcmp(loc_s_string_value(loc_s_at_path(dict_1,"fifth/2"))->data,"three") == 0);
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

