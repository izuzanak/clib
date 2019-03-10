
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
};/*}}}*/

// === methods of generated structures =========================================

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
#if OPTION_TO_STRING == ENABLED
    loc_s_person_to_string
#else
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

  var_array_s_clear(&array_0);
  var_s_clear(&person);
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

