
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
  "from_json",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_json_parse,
  test_json_parse_comments,
  test_json_create,
  test_from_json,
};/*}}}*/

// === methods of generated structures =========================================

// -- basic_tree_s --
@begin
methods basic_tree_s
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
  cassert(json_create(object,&buffer) == 0);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
        "{\"null\":null,\"integer\":123,\"float\":123.400000,\"string\":\"Hello world!\",\"array\":[0,1,2,3,4,5,6,7,8,9],\"object\":{\"first\":0,\"second\":1,\"third\":2}}"
        ) == 0);

  // - json_create_nice -
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

void test_from_json()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
#if OPTION_FROM_JSON == ENABLED
  CONT_INIT_CLEAR(bc_array_s,source);
  CONT_INIT_CLEAR(bc_array_s,target);
  CONT_INIT_CLEAR(from_json_s,from_json);

  source.used = 0;
  bc_array_s_append_format(&source,"125");

#define TEST_INTEGER_FROM_JSON(TYPE) \
  TYPE TYPE ## _value;\
  from_json.input_idx = 0;\
  cassert(TYPE ## _from_json(&TYPE ## _value,&source,&from_json) == 0);\
  cassert(TYPE ## _value == 125);

  TEST_INTEGER_FROM_JSON(bc);
  TEST_INTEGER_FROM_JSON(uc);
  TEST_INTEGER_FROM_JSON(si);
  TEST_INTEGER_FROM_JSON(usi);
  TEST_INTEGER_FROM_JSON(bi);
  TEST_INTEGER_FROM_JSON(ui);
  TEST_INTEGER_FROM_JSON(lli);

#define TEST_FLOAT_FROM_JSON_INTEGER(TYPE) \
  TYPE TYPE ## _value;\
  from_json.input_idx = 0;\
  cassert(TYPE ## _from_json(&TYPE ## _value,&source,&from_json) == 0);\
  cassert(TYPE ## _value == 125);\

  TEST_FLOAT_FROM_JSON_INTEGER(bf);
  TEST_FLOAT_FROM_JSON_INTEGER(bd);

  source.used = 0;
  bc_array_s_append_format(&source,"125.123");

#define TEST_FLOAT_FROM_JSON_FLOAT(TYPE) \
  from_json.input_idx = 0;\
  cassert(TYPE ## _from_json(&TYPE ## _value,&source,&from_json) == 0);\
  cassert(TYPE ## _value == (TYPE)125.123);

  TEST_FLOAT_FROM_JSON_FLOAT(bf);
  TEST_FLOAT_FROM_JSON_FLOAT(bd);

  // - string_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,"\"Hello world!!!\"");

  CONT_INIT_CLEAR(string_s,string);
  from_json.input_idx = 0;
  cassert(string_s_from_json(&string,&source,&from_json) == 0);

  target.used = 0;
  string_s_to_string(&string,&target);
  cassert(strncmp(target.data,"Hello world!!!",target.used) == 0); // NOLINT

  // - basic_tree_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,"[0,1,2,3,4,5,6,7,8,9]");

  CONT_INIT_CLEAR(basic_tree_s,basic_tree);
  from_json.input_idx = 0;
  cassert(basic_tree_s_from_json(&basic_tree,&source,&from_json) == 0);

  target.used = 0;
  basic_tree_s_to_string(&basic_tree,&target);
  cassert(strncmp(target.data,"[0,1,2,3,4,5,6,7,8,9]",target.used) == 0);

  // - static_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,"{\"first\":0,\"second\":1,\"third\":2}");

  CONT_INIT_CLEAR(static_s,static_0);
  from_json.input_idx = 0;
  cassert(static_s_from_json(&static_0,&source,&from_json) == 0);

  target.used = 0;
  static_s_to_string(&static_0,&target);
  cassert(strncmp(target.data,"{first:0,second:1,third:2}",target.used) == 0);

  // - static_array_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,
"[{\"first\":0,\"second\":1,\"third\":2}"
",{\"first\":0,\"second\":1,\"third\":2}"
",{\"first\":0,\"second\":1,\"third\":2}]");

  CONT_INIT_CLEAR(static_array_s,static_array);
  from_json.input_idx = 0;
  cassert(static_array_s_from_json(&static_array,&source,&from_json) == 0);

  target.used = 0;
  static_array_s_to_string(&static_array,&target);
  cassert(strncmp(target.data,"[{first:0,second:1,third:2},{first:0,second:1,third:2},{first:0,second:1,third:2}]",target.used) == 0);

  // - static_queue_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,
"[{\"first\":0,\"second\":1,\"third\":2}"
",{\"first\":0,\"second\":1,\"third\":2}"
",{\"first\":0,\"second\":1,\"third\":2}]");

  CONT_INIT_CLEAR(static_queue_s,static_queue);
  from_json.input_idx = 0;
  cassert(static_queue_s_from_json(&static_queue,&source,&from_json) == 0);

  target.used = 0;
  static_queue_s_to_string(&static_queue,&target);
  cassert(strncmp(target.data,"[{first:0,second:1,third:2},{first:0,second:1,third:2},{first:0,second:1,third:2}]",target.used) == 0);

  // - static_list_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,
"[{\"first\":0,\"second\":1,\"third\":2}"
",{\"first\":0,\"second\":1,\"third\":2}"
",{\"first\":0,\"second\":1,\"third\":2}]");

  CONT_INIT_CLEAR(static_list_s,static_list);
  from_json.input_idx = 0;
  cassert(static_list_s_from_json(&static_list,&source,&from_json) == 0);

  target.used = 0;
  static_list_s_to_string(&static_list,&target);
  cassert(strncmp(target.data,"[{first:0,second:1,third:2},{first:0,second:1,third:2},{first:0,second:1,third:2}]",target.used) == 0);

  // - static_tree_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,
"[{\"first\":3,\"second\":1,\"third\":2}"
",{\"first\":2,\"second\":1,\"third\":2}"
",{\"first\":1,\"second\":1,\"third\":2}]");

  CONT_INIT_CLEAR(static_tree_s,static_tree);
  from_json.input_idx = 0;
  cassert(static_tree_s_from_json(&static_tree,&source,&from_json) == 0);

  target.used = 0;
  static_tree_s_to_string(&static_tree,&target);
  cassert(strncmp(target.data,"[{first:1,second:1,third:2},{first:2,second:1,third:2},{first:3,second:1,third:2}]",target.used) == 0);

  // - dynamic_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,"{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}");

  CONT_INIT_CLEAR(dynamic_s,dynamic);
  from_json.input_idx = 0;
  cassert(dynamic_s_from_json(&dynamic,&source,&from_json) == 0);

  target.used = 0;
  dynamic_s_to_string(&dynamic,&target);
  cassert(strncmp(target.data,"{first:0,second:1,array:[{first:0,second:1,third:2}]}",target.used) == 0);

  // - dynamic_array_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,
"[{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}"
",{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}"
",{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}]");

  CONT_INIT_CLEAR(dynamic_array_s,dynamic_array);
  from_json.input_idx = 0;
  cassert(dynamic_array_s_from_json(&dynamic_array,&source,&from_json) == 0);

  target.used = 0;
  dynamic_array_s_to_string(&dynamic_array,&target);
  cassert(strncmp(target.data,"[{first:0,second:1,array:[{first:0,second:1,third:2}]},{first:0,second:1,array:[{first:0,second:1,third:2}]},{first:0,second:1,array:[{first:0,second:1,third:2}]}]",target.used) == 0);

  // - dynamic_queue_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,
"[{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}"
",{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}"
",{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}]");

  CONT_INIT_CLEAR(dynamic_queue_s,dynamic_queue);
  from_json.input_idx = 0;
  cassert(dynamic_queue_s_from_json(&dynamic_queue,&source,&from_json) == 0);

  target.used = 0;
  dynamic_queue_s_to_string(&dynamic_queue,&target);
  cassert(strncmp(target.data,"[{first:0,second:1,array:[{first:0,second:1,third:2}]},{first:0,second:1,array:[{first:0,second:1,third:2}]},{first:0,second:1,array:[{first:0,second:1,third:2}]}]",target.used) == 0);

  // - dynamic_list_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,
"[{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}"
",{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}"
",{\"first\":0,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}]");

  CONT_INIT_CLEAR(dynamic_list_s,dynamic_list);
  from_json.input_idx = 0;
  cassert(dynamic_list_s_from_json(&dynamic_list,&source,&from_json) == 0);

  target.used = 0;
  dynamic_list_s_to_string(&dynamic_list,&target);
  cassert(strncmp(target.data,"[{first:0,second:1,array:[{first:0,second:1,third:2}]},{first:0,second:1,array:[{first:0,second:1,third:2}]},{first:0,second:1,array:[{first:0,second:1,third:2}]}]",target.used) == 0);

  // - dynamic_tree_s_from_json -
  source.used = 0;
  bc_array_s_append_ptr(&source,
"[{\"first\":3,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}"
",{\"first\":2,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}"
",{\"first\":1,\"second\":1,\"array\":[{\"first\":0,\"second\":1,\"third\":2}]}]");

  CONT_INIT_CLEAR(dynamic_tree_s,dynamic_tree);
  from_json.input_idx = 0;
  cassert(dynamic_tree_s_from_json(&dynamic_tree,&source,&from_json) == 0);

  target.used = 0;
  dynamic_tree_s_to_string(&dynamic_tree,&target);
  cassert(strncmp(target.data,"[{first:1,second:1,array:[{first:0,second:1,third:2}]},{first:2,second:1,array:[{first:0,second:1,third:2}]},{first:3,second:1,array:[{first:0,second:1,third:2}]}]",target.used) == 0);
#endif
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

