
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libjson_cll_test";

const char *test_names[] =
{/*{{{*/
  "json_parse",
  "json_parse_comments",
  "json_create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_json_parse,
  test_json_parse_comments,
  test_json_create,
};/*}}}*/

// === test execution functions ================================================

void test_json_parse()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  const char *data =
"{\n"
"  \"null\": null,\n"
"  \"integer\": 123,\n"
"  \"float\": 123.400000,\n"
"  \"string\": \"Hello world!\",\n"
"  \"array\": [\n"
"    0,\n"
"    1,\n"
"    2,\n"
"    3,\n"
"    4,\n"
"    5,\n"
"    6,\n"
"    7,\n"
"    8,\n"
"    9\n"
"  ],\n"
"  \"object\": {\n"
"    \"first\": 0,\n"
"    \"second\": 1,\n"
"    \"third\": 2\n"
"  }\n"
"}";

  // - json_parse -
  var_s object = NULL;
  bc_array_s data_buffer = {strlen(data),strlen(data),(char *)data};
  cassert(json_parse(&data_buffer,&object) == 0);
  loc_s_to_string(object,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[null:<blank>,array:[0,1,2,3,4,5,6,7,8,9],float:123.400000,object:[first:0,third:2,second:1],string:Hello world!,integer:123]") == 0);

  CONT_INIT(string_s,tab);
  string_s_set_ptr(&tab,"  ");

  CONT_INIT(string_s,indent);
  string_s_set_ptr(&indent,"");

  // - json_create -
  cassert(json_create_nice(object,&tab,&indent,&buffer) == 0);
  bc_array_s_push(&buffer,'\0');

  cassert(strcmp(buffer.data,data) == 0);

  string_s_clear(&tab);
  string_s_clear(&indent);
  var_s_clear(&object);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_json_parse_comments()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  const char *data =
"{\n"
"  \"null\": null,\n"
"  # - First type of comment -\n"
"  \"integer\": 123,\n"
"  // - Second type of comment -\n"
"  \"float\": 123.400000,\n"
"  /* - Third type of comment - */\n"
"  \"string\": \"Hello world!\",\n"
"  \"array\": [\n"
"    0,\n"
"    1,\n"
"    2,\n"
"    3,\n"
"    4,\n"
"    5,\n"
"    6,\n"
"    7,\n"
"    8,\n"
"    9\n"
"  ],\n"
"  \"object\": {\n"
"    \"first\": 0,\n"
"    \"second\": 1,\n"
"    \"third\": 2\n"
"  }\n"
"}";

  // - json_parse -
  var_s object = NULL;
  bc_array_s data_buffer = {strlen(data),strlen(data),(char *)data};
  cassert(json_parse(&data_buffer,&object) == 0);
  loc_s_to_string(object,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[null:<blank>,array:[0,1,2,3,4,5,6,7,8,9],float:123.400000,object:[first:0,third:2,second:1],string:Hello world!,integer:123]") == 0);

  var_s_clear(&object);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_json_create()
{/*{{{*/
  CONT_INIT(bc_array_s,buffer);

#define JSON_CREATE() \
{/*{{{*/\
  cassert(json_create(dict,&buffer) == 0);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define JSON_CREATE_NICE() \
{/*{{{*/\
  cassert(json_create_nice(dict,&tab,&indent,&buffer) == 0);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  VAR(dict,loc_s_dict());

  CONT_INIT(string_s,tab);
  string_s_set_ptr(&tab,"  ");

  CONT_INIT(string_s,indent);
  string_s_set_ptr(&indent,"");

  JSON_CREATE_NICE();
  cassert(strcmp(buffer.data,"{}") == 0);

  VAR(key,loc_s_string_ptr("integer"));
  loc_s_dict_set(dict,key,loc_s_blank());
  var_s_clear(&key);

  JSON_CREATE();
  cassert(strcmp(buffer.data,"{\"integer\":null}") == 0);

  JSON_CREATE_NICE();
  cassert(strcmp(buffer.data,"{\n  \"integer\": null\n}") == 0);

  string_s_clear(&tab);
  var_s_clear(&dict);
  bc_array_s_clear(&buffer);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

