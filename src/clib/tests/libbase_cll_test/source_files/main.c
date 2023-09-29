
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libbase_cll_test";

const char *test_names[] =
{/*{{{*/
  "bc_array",
  "ui_array",
  "atomic",
  "static",
  "string",
  "basic_type_array",
  "static_type_array",
  "dynamic_type_array",
  "basic_type_queue",
  "static_type_queue",
  "dynamic_type_queue",
  "basic_type_list",
  "static_type_list",
  "dynamic_type_list",
  "basic_type_tree",
  "static_type_tree",
  "dynamic_type_tree",
  "basic_type_choice",
  "dynamic_type_choice",
  "doc_examples",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_bc_array,
  test_ui_array,
  test_atomic,
  test_static,
  test_string,
  test_basic_type_array,
  test_static_type_array,
  test_dynamic_type_array,
  test_basic_type_queue,
  test_static_type_queue,
  test_dynamic_type_queue,
  test_basic_type_list,
  test_static_type_list,
  test_dynamic_type_list,
  test_basic_type_tree,
  test_static_type_tree,
  test_dynamic_type_tree,
  test_basic_type_choice,
  test_dynamic_type_choice,
  test_doc_examples,
};/*}}}*/

// === methods of generated structures =========================================

// -- static_s --
@begin
methods static_s
@end

// -- atomic_array_s --
@begin
methods atomic_array_s
@end

// -- ui_queue_s --
@begin
methods ui_queue_s
@end

// -- ui_list_s --
@begin
methods ui_list_s
@end

// -- ui_tree_s --
@begin
methods ui_tree_s
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

// -- string_queue_s --
@begin
methods string_queue_s
@end

// -- string_list_s --
@begin
methods string_list_s
@end

// -- basic_choice_s --
@begin
methods basic_choice_s
@end

// -- dynamic_choice_s --
@begin
methods dynamic_choice_s
@end

// === test execution functions ================================================

void test_bc_array()
{/*{{{*/
  CONT_INIT(bc_array_s,buffer_0);
  CONT_INIT(bc_array_s,buffer_1);

  const char *data = "Hello world";

  // - bc_array_s_append -
  // - bc_array_s_append_ptr -
  bc_array_s_append(&buffer_0,strlen(data),data);
  bc_array_s_push(&buffer_0,'\0');
  bc_array_s_append_ptr(&buffer_1,data);
  bc_array_s_push(&buffer_1,'\0');
  cassert(strcmp(buffer_0.data,buffer_1.data) == 0);

  // - bc_array_s_append_format -
  bc_array_s_clear(&buffer_0);
  bc_array_s_append_format(&buffer_0,"integer: %d",100);
  cassert(buffer_0.used == 12);
  bc_array_s_push(&buffer_0,'\0');

  bc_array_s_clear(&buffer_1);
  bc_array_s_copy_resize(&buffer_1,13);
  bc_array_s_append_format(&buffer_1,"integer: %d",100);
  cassert(buffer_1.used == 12 && buffer_1.size == 13);
  bc_array_s_push(&buffer_1,'\0');
  cassert(strcmp(buffer_0.data,buffer_1.data) == 0);

  // - bc_array_s_tail -
  buffer_0.used = 0;
  bc_array_s_append_ptr(&buffer_0,"Hello world");
  bc_array_s_tail(&buffer_0,5);
  bc_array_s_push(&buffer_0,'\0');
  cassert(strcmp(buffer_0.data,"world") == 0);

  buffer_0.used = 0;
  bc_array_s_append_ptr(&buffer_0,"Hi world");
  bc_array_s_tail(&buffer_0,5);
  bc_array_s_push(&buffer_0,'\0');
  cassert(strcmp(buffer_0.data,"world") == 0);

  bc_array_s_clear(&buffer_1);
  bc_array_s_clear(&buffer_0);
}/*}}}*/

void test_ui_array()
{/*{{{*/

  // - fill tree by random numbers -
  CONT_INIT_CLEAR(ui_tree_s,tree);

  unsigned idx = 0;
  do {
    unsigned value = rand(); // NOLINT(cert-msc30-c,cert-msc50-cpp)
    ui_tree_s_insert(&tree,value);
  } while(++idx < 1000);

  // - create sorted ui array -
  CONT_INIT_CLEAR(ui_array_s,sorted);

  unsigned t_idx = ui_tree_s_get_min_value_idx(&tree,tree.root_idx);
  unsigned s_idx = 0;
  do
  {
    ui_array_s_push(&sorted,tree.data[t_idx].object);
    t_idx = ui_tree_s_get_next_idx(&tree,t_idx);
  } while(++s_idx,t_idx != c_idx_not_exist);

  // - test function ui_binary_search -
  idx = 0;
  do {
    cassert(ui_binary_search(sorted.data,sorted.used,sorted.data[idx]) == idx);
  } while(++idx < sorted.used);
}/*}}}*/

void test_atomic()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define ATOMIC_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  atomic_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  atomic_s atomic_0;
  atomic_s_set(&atomic_0,0);
  ATOMIC_S_TO_BUFFER(&atomic_0);
  cassert(strcmp(buffer.data,"atomic_s{0}") == 0);

  atomic_s_inc(&atomic_0);
  atomic_s_inc(&atomic_0);
  ATOMIC_S_TO_BUFFER(&atomic_0);
  cassert(strcmp(buffer.data,"atomic_s{2}") == 0);

  atomic_s_dec(&atomic_0);
  ATOMIC_S_TO_BUFFER(&atomic_0);
  cassert(strcmp(buffer.data,"atomic_s{1}") == 0);

  cassert(atomic_s_dec_and_test(&atomic_0));
  ATOMIC_S_TO_BUFFER(&atomic_0);
  cassert(strcmp(buffer.data,"atomic_s{0}") == 0);

  atomic_s_add(&atomic_0,10);
  cassert(atomic_s_value(&atomic_0) == 10);
  ATOMIC_S_TO_BUFFER(&atomic_0);
  cassert(strcmp(buffer.data,"atomic_s{10}") == 0);

  cassert(atomic_s_inc_and_test(&atomic_0,11));
  cassert(!atomic_s_inc_and_test(&atomic_0,11));
  ATOMIC_S_TO_BUFFER(&atomic_0);
  cassert(strcmp(buffer.data,"atomic_s{12}") == 0);

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_static()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define STATIC_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  static_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - static_s_to_string -
  static_s static_0 = {1,2,3};
  STATIC_S_TO_BUFFER(&static_0);
  cassert(strcmp(buffer.data,"{first:1,second:2,third:3}") == 0);

  // - static_s_to_string_separator -
  buffer.used = 0;
  static_s_to_string_separator(&static_0,&buffer,4," -- ");
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"first:1 -- second:2 -- third:3") == 0);

#if OPTION_TO_JSON == ENABLED
  // - static_s_to_json -
  buffer.used = 0;
  static_s_to_json(&static_0,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"{\"first\":1,\"second\":2,\"third\":3}") == 0);

  // - static_s_to_json_nice -
  CONT_INIT(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

  buffer.used = 0;
  static_s_to_json_nice(&static_0,&json_nice,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
"{\n"
"==--\"first\": 1,\n"
"==--\"second\": 2,\n"
"==--\"third\": 3\n"
"==}") == 0);

  json_nice_s_clear(&json_nice);
#endif

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_string()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  // - string_s_init -
  string_s string_0;
  string_s_init(&string_0);
  cassert(string_0.size == 1);

  // - string_s_create -
  string_s_create(&string_0,125);
  cassert(string_0.size == 126);

  // - string_s_set -
  const char *data_0 = "Hello there world!";
  string_s_set(&string_0,strlen(data_0),data_0);
  cassert(string_0.size == strlen(data_0) + 1 && strcmp(string_0.data,data_0) == 0);

  // - string_s_swap -
  CONT_INIT(string_s,string_1);
  string_s_swap(&string_0,&string_1);
  cassert(string_0.size == 1);
  cassert(string_1.size == strlen(data_0) + 1 && strcmp(string_1.data,data_0) == 0);

  // - string_s_copy -
  string_s_copy(&string_0,&string_1);
  cassert(string_0.size == strlen(data_0) + 1 && strcmp(string_0.data,data_0) == 0);
  cassert(string_s_compare(&string_0,&string_1));

#if OPTION_TO_JSON == ENABLED
  // - string_s_to_json -
  string_s_set_ptr(&string_0,"Hello\nthere\nworld!\n");
  buffer.used = 0;
  string_s_to_json(&string_0,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"\"Hello\\nthere\\nworld!\\n\"") == 0);
#endif

  // - string_s_set_format -
  string_s_set_format(&string_0,
      "Longer than %d bytes ================================================================================================================================",
      128);
  cassert(strcmp(string_0.data,
        "Longer than 128 bytes ================================================================================================================================") == 0);

  // - string_s_clear -
  string_s_clear(&string_1);
  cassert(!string_s_compare(&string_0,&string_1));

  string_s_clear(&string_1);
  string_s_clear(&string_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_basic_type_array()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define BASIC_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  ui_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - ui_array_s_init -
  ui_array_s array_0;
  ui_array_s_init(&array_0);
  cassert(array_0.size == 0 && array_0.used == 0 && array_0.data == NULL);

  // - ui_array_s_init_size -
  ui_array_s array_1;
  ui_array_s_init_size(&array_1,100);
  cassert(array_1.size == 100 && array_1.used == 0 && array_1.data != NULL);

  // - ui_array_s_set -
  const unsigned data_0[] = {1,2,3,4,5,6,7,8,9,10};
  ui_array_s_set(&array_0,10,data_0);

  BASIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.used == 10 && array_0.size == 10 &&
    strcmp(buffer.data,"[1,2,3,4,5,6,7,8,9,10]") == 0);

  // - ui_array_s_push -
  ui_array_s_push(&array_0,11);
  BASIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.used == 11 && array_0.size >= 11 &&
    strcmp(buffer.data,"[1,2,3,4,5,6,7,8,9,10,11]") == 0);

  // - ui_array_s_flush -
  ui_array_s_flush(&array_0);
  cassert(array_0.used == 11 && array_0.size == 11);

  // - ui_array_s_reserve -
  ui_array_s_reserve(&array_0,10);
  cassert(array_0.used == 11 && array_0.size >= 21);

  // - ui_array_s_pop -
  array_1.used = 0;
  unsigned cnt = 0;
  do {
    ui_array_s_push(&array_1,ui_array_s_pop(&array_0));
  } while(++cnt < 5);

  BASIC_ARRAY_S_TO_BUFFER(&array_1);
  cassert(strcmp(buffer.data,"[11,10,9,8,7]") == 0);

  // - ui_array_s_last -
  cassert(*ui_array_s_last(&array_0) == 6);

  // - ui_array_s_copy_resize -
  ui_array_s_copy_resize(&array_0,10);
  BASIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.used == 6 && array_0.size == 10 &&
    strcmp(buffer.data,"[1,2,3,4,5,6]") == 0);

  // - ui_array_s_fill -
  ui_array_s_fill(&array_0,7);
  BASIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.used == 10 && array_0.size == 10 &&
    strcmp(buffer.data,"[7,7,7,7,7,7,7,7,7,7]") == 0);

  // - ui_array_s_at -
  *ui_array_s_at(&array_0,7) = 125;
  BASIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(strcmp(buffer.data,"[7,7,7,7,7,7,7,125,7,7]") == 0);

  // - ui_array_s_get_idx -
  cassert(ui_array_s_get_idx(&array_0,0) == c_idx_not_exist);
  cassert(ui_array_s_get_idx(&array_0,125) == 7);

  // - ui_array_s_compare -
  cassert(ui_array_s_compare(&array_0,&array_0));
  cassert(!ui_array_s_compare(&array_0,&array_1));

  // - ui_array_s_copy -
  ui_array_s_copy(&array_0,&array_1);
  BASIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(ui_array_s_compare(&array_0,&array_1) &&
    strcmp(buffer.data,"[11,10,9,8,7]") == 0);

  ui_array_s_clear(&array_1);
  ui_array_s_clear(&array_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_static_type_array()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define STATIC_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  static_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - static_array_s_init -
  static_array_s array_0;
  static_array_s_init(&array_0);
  cassert(array_0.size == 0 && array_0.used == 0);

  // - static_array_s_set -
  const static_s static_data[] = {{1,2,3},{4,5,6},{7,8,9}};
  static_array_s_set(&array_0,3,static_data);
  STATIC_ARRAY_S_TO_BUFFER(&array_0)
  cassert(strcmp(buffer.data,"[{first:1,second:2,third:3},{first:4,second:5,third:6},{first:7,second:8,third:9}]") == 0);

  // - static_array_s_copy_resize -
  static_array_s_copy_resize(&array_0,10);
  STATIC_ARRAY_S_TO_BUFFER(&array_0)
  cassert(array_0.size == 10 && array_0.used == 3 &&
    strcmp(buffer.data,"[{first:1,second:2,third:3},{first:4,second:5,third:6},{first:7,second:8,third:9}]") == 0);

  // - static_array_s_flush -
  static_array_s_flush(&array_0);
  STATIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.size == 3 && array_0.used == 3 &&
    strcmp(buffer.data,"[{first:1,second:2,third:3},{first:4,second:5,third:6},{first:7,second:8,third:9}]") == 0);

  // - static_array_s_swap -
  CONT_INIT(static_array_s,array_1);
  static_array_s_swap(&array_0,&array_1);
  STATIC_ARRAY_S_TO_BUFFER(&array_1);
  cassert(array_0.size == 0 && array_0.used == 0 && array_1.size == 3 && array_1.used == 3 &&
    strcmp(buffer.data,"[{first:1,second:2,third:3},{first:4,second:5,third:6},{first:7,second:8,third:9}]") == 0);

  // - static_array_s_at -
  STATIC_S_TO_BUFFER(static_array_s_at(&array_1,2));
  cassert(strcmp(buffer.data,"{first:7,second:8,third:9}") == 0);

  // - static_array_s_push -
  // - static_array_s_pop -
  while (array_1.used != 0)
  {
    static_array_s_push(&array_0,static_array_s_pop(&array_1));
  }
  STATIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.used == 3 && array_1.used == 0 &&
    strcmp(buffer.data,"[{first:7,second:8,third:9},{first:4,second:5,third:6},{first:1,second:2,third:3}]") == 0);

  // - static_array_s_last -
  STATIC_S_TO_BUFFER(static_array_s_last(&array_0));
  cassert(strcmp(buffer.data,"{first:1,second:2,third:3}") == 0);

  // - static_array_s_fill -
  array_0.used = 0;
  static_array_s_copy_resize(&array_0,3);
  static_s element = {4,5,6};
  static_array_s_fill(&array_0,&element);
  STATIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.used == 3 && array_0.size == 3 &&
    strcmp(buffer.data,"[{first:4,second:5,third:6},{first:4,second:5,third:6},{first:4,second:5,third:6}]") == 0);

  // - static_array_s_push_blank -
  unsigned idx = 0;
  do {
    static_array_s_push_blank(&array_1);
    static_s_set(static_array_s_last(&array_1),idx,idx,idx);
  } while(++idx < 3);
  STATIC_ARRAY_S_TO_BUFFER(&array_1);
  cassert(strcmp(buffer.data,"[{first:0,second:0,third:0},{first:1,second:1,third:1},{first:2,second:2,third:2}]") == 0);

  // - static_array_s_get_idx -
  cassert(static_array_s_get_idx(&array_1,static_array_s_at(&array_1,1)) == 1);

  // - static_array_s_copy -
  // - static_array_s_compare -
  cassert(!static_array_s_compare(&array_0,&array_1));
  static_array_s_copy(&array_0,&array_1);
  STATIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(static_array_s_compare(&array_0,&array_1) &&
    strcmp(buffer.data,"[{first:0,second:0,third:0},{first:1,second:1,third:1},{first:2,second:2,third:2}]") == 0);

#if OPTION_TO_JSON == ENABLED
  // - static_array_s_to_json -
  buffer.used = 0;
  static_array_s_to_json(&array_0,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[{\"first\":0,\"second\":0,\"third\":0},{\"first\":1,\"second\":1,\"third\":1},{\"first\":2,\"second\":2,\"third\":2}]") == 0);

  // - static_array_s_to_json_nice -
  CONT_INIT(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

  buffer.used = 0;
  static_array_s_to_json_nice(&array_0,&json_nice,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
"[\n"
"==--{\n"
"==----\"first\": 0,\n"
"==----\"second\": 0,\n"
"==----\"third\": 0\n"
"==--},\n"
"==--{\n"
"==----\"first\": 1,\n"
"==----\"second\": 1,\n"
"==----\"third\": 1\n"
"==--},\n"
"==--{\n"
"==----\"first\": 2,\n"
"==----\"second\": 2,\n"
"==----\"third\": 2\n"
"==--}\n"
"==]") == 0);

  json_nice_s_clear(&json_nice);
#endif

  static_array_s_clear(&array_1);
  static_array_s_clear(&array_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_dynamic_type_array()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define DYNAMIC_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  string_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - string_array_s_init -
  string_array_s array_0;
  string_array_s_init(&array_0);
  cassert(array_0.size == 0 && array_0.used == 0);

  // - string_array_s_push -
  const char *data[] = {"One","Two","Three","Four"};
  do {
    CONT_INIT(string_s,string);
    string_s_set_ptr(&string,data[array_0.used]);
    string_array_s_push(&array_0,&string);
    string_s_clear(&string);
  } while(array_0.used < sizeof(data)/sizeof(data[0]));
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_0)
  cassert(array_0.used == 4 && strcmp(buffer.data,"[One,Two,Three,Four]") == 0);

  // - string_array_s_push_blank -
  CONT_INIT(string_array_s,array_1);
  do {
    string_array_s_push_blank(&array_1);
    string_s_set_ptr(string_array_s_last(&array_1),data[4 - array_1.used]);
  } while(array_1.used < sizeof(data)/sizeof(data[0]));
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_1)
  cassert(array_1.used == 4 && strcmp(buffer.data,"[Four,Three,Two,One]") == 0);

  // - string_array_s_copy_resize -
  string_array_s_copy_resize(&array_0,10);
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_0)
  cassert(array_0.size == 10 && array_0.used == 4 &&
    strcmp(buffer.data,"[One,Two,Three,Four]") == 0);

  // - string_array_s_flush -
  string_array_s_flush(&array_0);
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.size == 4 && array_0.used == 4 &&
    strcmp(buffer.data,"[One,Two,Three,Four]") == 0);

  // - string_array_s_clear -
  // - string_array_s_swap -
  string_array_s_clear(&array_1);
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_1);
  cassert(array_0.size == 4 && array_0.used == 4 && array_1.used == 0 &&
    strcmp(buffer.data,"[]") == 0);
  string_array_s_swap(&array_0,&array_1);
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_1);
  cassert(array_0.used == 0 && array_1.size == 4 && array_1.used == 4 &&
    strcmp(buffer.data,"[One,Two,Three,Four]") == 0);

  // - string_array_s_at -
  cassert(strcmp(string_array_s_at(&array_1,2)->data,"Three") == 0);

  // - string_array_s_push -
  // - string_array_s_pop -
  while (array_1.used != 0)
  {
    string_array_s_push(&array_0,string_array_s_pop(&array_1));
  }
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.used == 4 && array_1.used == 0 &&
    strcmp(buffer.data,"[Four,Three,Two,One]") == 0);

  // - string_array_s_last -
  cassert(strcmp(string_array_s_last(&array_0)->data,"One") == 0);

  // - string_array_s_fill -
  array_0.used = 0;
  string_array_s_copy_resize(&array_0,3);
  CONT_INIT(string_s,string);
  string_s_set_ptr(&string,"Hello world!");
  string_array_s_fill(&array_0,&string);
  string_s_clear(&string);
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(array_0.used == 3 && array_0.size == 3 &&
    strcmp(buffer.data,"[Hello world!,Hello world!,Hello world!]") == 0);

  // - string_array_s_to_string_separator -
  buffer.used = 0;
  string_array_s_to_string_separator(&array_0,&buffer,4," -- ");
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"Hello world! -- Hello world! -- Hello world!") == 0);

  // - string_array_s_push_blank -
  unsigned idx = 0;
  do {
    string_array_s_push_blank(&array_1);
    string_s_set_format(string_array_s_last(&array_1),"idx: %d",idx);
  } while(++idx < 3);
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_1);
  cassert(strcmp(buffer.data,"[idx: 0,idx: 1,idx: 2]") == 0);

  // - string_array_s_get_idx -
  cassert(string_array_s_get_idx(&array_1,string_array_s_at(&array_1,1)) == 1);

  // - string_array_s_copy -
  // - string_array_s_compare -
  cassert(!string_array_s_compare(&array_0,&array_1));
  string_array_s_copy(&array_0,&array_1);
  DYNAMIC_ARRAY_S_TO_BUFFER(&array_0);
  cassert(string_array_s_compare(&array_0,&array_1) &&
    strcmp(buffer.data,"[idx: 0,idx: 1,idx: 2]") == 0);

  string_array_s_clear(&array_1);
  string_array_s_clear(&array_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_basic_type_queue()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define BASIC_QUEUE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  ui_queue_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - ui_queue_s_init -
  ui_queue_s queue_0;
  ui_queue_s_init(&queue_0);
  cassert(queue_0.size == 0 && queue_0.used == 0 && queue_0.data == NULL);

  // - ui_queue_s_insert -
  unsigned idx = 0;
  do {
    ui_queue_s_insert(&queue_0,idx);
  } while(++idx < 10);
  BASIC_QUEUE_S_TO_BUFFER(&queue_0);
  cassert(strcmp(buffer.data,"[0,1,2,3,4,5,6,7,8,9]") == 0);

  ui_queue_s_clear(&queue_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_static_type_queue()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define STATIC_QUEUE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  static_queue_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - static_queue_s_init -
  static_queue_s queue_0;
  static_queue_s_init(&queue_0);
  cassert(queue_0.size == 0 && queue_0.used == 0 && queue_0.data == NULL);

  // - static_queue_s_insert -
  unsigned idx = 0;
  do {
    static_s data = {idx,idx,idx};
    static_queue_s_insert(&queue_0,&data);
  } while(++idx < 3);
  STATIC_QUEUE_S_TO_BUFFER(&queue_0);
  cassert(strcmp(buffer.data,"[{first:0,second:0,third:0},{first:1,second:1,third:1},{first:2,second:2,third:2}]") == 0);

#if OPTION_TO_JSON == ENABLED
  // - static_queue_s_to_json -
  buffer.used = 0;
  static_queue_s_to_json(&queue_0,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[{\"first\":0,\"second\":0,\"third\":0},{\"first\":1,\"second\":1,\"third\":1},{\"first\":2,\"second\":2,\"third\":2}]") == 0);

  // - static_queue_s_to_json_nice -
  CONT_INIT(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

  buffer.used = 0;
  static_queue_s_to_json_nice(&queue_0,&json_nice,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
"[\n"
"==--{\n"
"==----\"first\": 0,\n"
"==----\"second\": 0,\n"
"==----\"third\": 0\n"
"==--},\n"
"==--{\n"
"==----\"first\": 1,\n"
"==----\"second\": 1,\n"
"==----\"third\": 1\n"
"==--},\n"
"==--{\n"
"==----\"first\": 2,\n"
"==----\"second\": 2,\n"
"==----\"third\": 2\n"
"==--}\n"
"==]") == 0);

  json_nice_s_clear(&json_nice);
#endif

  static_queue_s_clear(&queue_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_dynamic_type_queue()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define DYNAMIC_QUEUE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  string_queue_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - string_queue_s_init -
  string_queue_s queue_0;
  string_queue_s_init(&queue_0);
  cassert(queue_0.size == 0 && queue_0.used == 0 && queue_0.data == NULL);

  // - string_queue_s_insert -
  const char *data[] = {"One","Two","Three","Four"};
  do {
    CONT_INIT(string_s,string);
    string_s_set_ptr(&string,data[queue_0.used]);
    string_queue_s_insert(&queue_0,&string);
    string_s_clear(&string);
  } while(queue_0.used < sizeof(data)/sizeof(data[0]));
  DYNAMIC_QUEUE_S_TO_BUFFER(&queue_0);
  cassert(strcmp(buffer.data,"[One,Two,Three,Four]") == 0);

  // - string_queue_s_to_string_separator -
  buffer.used = 0;
  string_queue_s_to_string_separator(&queue_0,&buffer,4," -- ");
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"One -- Two -- Three -- Four") == 0);

  string_queue_s_clear(&queue_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_basic_type_list()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define BASIC_LIST_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  ui_list_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - ui_list_s_init -
  ui_list_s list_0;
  ui_list_s_init(&list_0);
  cassert(list_0.size == 0 && list_0.used == 0 && list_0.data == NULL);

  // - ui_list_s_append -
  unsigned idx = 0;
  do {
    ui_list_s_append(&list_0,idx);
  } while(++idx < 10);
  BASIC_LIST_S_TO_BUFFER(&list_0);
  cassert(strcmp(buffer.data,"[0,1,2,3,4,5,6,7,8,9]") == 0);

  ui_list_s_clear(&list_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_static_type_list()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define STATIC_LIST_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  static_list_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - static_list_s_init -
  static_list_s list_0;
  static_list_s_init(&list_0);
  cassert(list_0.size == 0 && list_0.used == 0 && list_0.data == NULL);

  // - static_list_s_append -
  unsigned idx = 0;
  do {
    static_s data = {idx,idx,idx};
    static_list_s_append(&list_0,&data);
  } while(++idx < 3);
  STATIC_LIST_S_TO_BUFFER(&list_0);
  cassert(strcmp(buffer.data,"[{first:0,second:0,third:0},{first:1,second:1,third:1},{first:2,second:2,third:2}]") == 0);

#if OPTION_TO_JSON == ENABLED
  // - static_list_s_to_json -
  buffer.used = 0;
  static_list_s_to_json(&list_0,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[{\"first\":0,\"second\":0,\"third\":0},{\"first\":1,\"second\":1,\"third\":1},{\"first\":2,\"second\":2,\"third\":2}]") == 0);

  // - static_list_s_to_json_nice -
  CONT_INIT(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

  buffer.used = 0;
  static_list_s_to_json_nice(&list_0,&json_nice,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
"[\n"
"==--{\n"
"==----\"first\": 0,\n"
"==----\"second\": 0,\n"
"==----\"third\": 0\n"
"==--},\n"
"==--{\n"
"==----\"first\": 1,\n"
"==----\"second\": 1,\n"
"==----\"third\": 1\n"
"==--},\n"
"==--{\n"
"==----\"first\": 2,\n"
"==----\"second\": 2,\n"
"==----\"third\": 2\n"
"==--}\n"
"==]") == 0);

  json_nice_s_clear(&json_nice);
#endif

  static_list_s_clear(&list_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_dynamic_type_list()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define DYNAMIC_LIST_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  string_list_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - string_list_s_init -
  string_list_s list_0;
  string_list_s_init(&list_0);
  cassert(list_0.size == 0 && list_0.used == 0 && list_0.data == NULL);

  // - string_list_s_append -
  const char *data[] = {"One","Two","Three","Four"};
  unsigned idx = 0;
  do {
    CONT_INIT(string_s,string);
    string_s_set_ptr(&string,data[idx]);
    string_list_s_append(&list_0,&string);
    string_s_clear(&string);
  } while(++idx < sizeof(data)/sizeof(data[0]));
  DYNAMIC_LIST_S_TO_BUFFER(&list_0);
  cassert(strcmp(buffer.data,"[One,Two,Three,Four]") == 0);

  // - string_list_s_to_string_separator -
  buffer.used = 0;
  string_list_s_to_string_separator(&list_0,&buffer,4," -- ");
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"One -- Two -- Three -- Four") == 0);

  string_list_s_clear(&list_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_basic_type_tree()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define BASIC_TREE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  ui_tree_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - ui_tree_s_init -
  ui_tree_s tree_0;
  ui_tree_s_init(&tree_0);
  cassert(tree_0.size == 0 && tree_0.used == 0 && tree_0.data == NULL);

  // - ui_tree_s_append -
  unsigned idx = 0;
  do {
    ui_tree_s_insert(&tree_0,idx);
  } while(++idx < 10);
  BASIC_TREE_S_TO_BUFFER(&tree_0);
  cassert(strcmp(buffer.data,"[0,1,2,3,4,5,6,7,8,9]") == 0);

  ui_tree_s_clear(&tree_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_static_type_tree()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define STATIC_TREE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  static_tree_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - static_tree_s_init -
  static_tree_s tree_0;
  static_tree_s_init(&tree_0);
  cassert(tree_0.size == 0 && tree_0.used == 0 && tree_0.data == NULL);

  // - static_tree_s_append -
  unsigned idx = 0;
  do {
    static_s data = {idx,idx,idx};
    static_tree_s_insert(&tree_0,&data);
  } while(++idx < 3);
  STATIC_TREE_S_TO_BUFFER(&tree_0);
  cassert(strcmp(buffer.data,"[{first:0,second:0,third:0},{first:1,second:1,third:1},{first:2,second:2,third:2}]") == 0);

#if OPTION_TO_JSON == ENABLED
  // - static_tree_s_to_json -
  buffer.used = 0;
  static_tree_s_to_json(&tree_0,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[{\"first\":0,\"second\":0,\"third\":0},{\"first\":1,\"second\":1,\"third\":1},{\"first\":2,\"second\":2,\"third\":2}]") == 0);

  // - static_tree_s_to_json_nice -
  CONT_INIT(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

  buffer.used = 0;
  static_tree_s_to_json_nice(&tree_0,&json_nice,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,
"[\n"
"==--{\n"
"==----\"first\": 0,\n"
"==----\"second\": 0,\n"
"==----\"third\": 0\n"
"==--},\n"
"==--{\n"
"==----\"first\": 1,\n"
"==----\"second\": 1,\n"
"==----\"third\": 1\n"
"==--},\n"
"==--{\n"
"==----\"first\": 2,\n"
"==----\"second\": 2,\n"
"==----\"third\": 2\n"
"==--}\n"
"==]") == 0);

  json_nice_s_clear(&json_nice);
#endif

  static_tree_s_clear(&tree_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_dynamic_type_tree()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define DYNAMIC_TREE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  string_tree_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - string_tree_s_init -
  string_tree_s tree_0;
  string_tree_s_init(&tree_0);
  cassert(tree_0.size == 0 && tree_0.used == 0 && tree_0.data == NULL);

  // - string_tree_s_append -
  const char *data[] = {"One","Two","Three","Four"};
  unsigned idx = 0;
  do {
    CONT_INIT(string_s,string);
    string_s_set_ptr(&string,data[idx]);
    string_tree_s_swap_insert(&tree_0,&string);
    string_s_clear(&string);
  } while(++idx < sizeof(data)/sizeof(data[0]));
  DYNAMIC_TREE_S_TO_BUFFER(&tree_0);
  cassert(strcmp(buffer.data,"[One,Two,Four,Three]") == 0);

  // - string_tree_s_to_string_separator -
  buffer.used = 0;
  string_tree_s_to_string_separator(&tree_0,&buffer,4," -- ");
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"One -- Two -- Four -- Three") == 0);

  string_tree_s_clear(&tree_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_basic_type_choice()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

  CONT_INIT_CLEAR(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

#define BASIC_CHOICE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  basic_choice_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define BASIC_CHOICE_S_TO_JSON(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  basic_choice_s_to_json(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define BASIC_CHOICE_S_TO_JSON_NICE(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  basic_choice_s_to_json_nice(NAME,&json_nice,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT_CLEAR(basic_choice_s,choice_0);
  *basic_choice_s_integer(&choice_0) = 120;
  BASIC_CHOICE_S_TO_BUFFER(&choice_0);
  cassert(strcmp(buffer.data,"{integer:120}") == 0);

#if OPTION_TO_JSON == ENABLED
  BASIC_CHOICE_S_TO_JSON(&choice_0);
  cassert(strcmp(buffer.data,"{\"integer\":120}") == 0);
  BASIC_CHOICE_S_TO_JSON_NICE(&choice_0);
  cassert(strcmp(buffer.data,
"{\n"
"==--\"integer\": 120\n"
"==}"
  ) == 0);
#endif

  *basic_choice_s_real(&choice_0) = 120.5;
  BASIC_CHOICE_S_TO_BUFFER(&choice_0);
  cassert(strcmp(buffer.data,"{real:120.500000}") == 0);

#if OPTION_TO_JSON == ENABLED
  BASIC_CHOICE_S_TO_JSON(&choice_0);
  cassert(strcmp(buffer.data,"{\"real\":120.500000}") == 0);
  BASIC_CHOICE_S_TO_JSON_NICE(&choice_0);
  cassert(strcmp(buffer.data,
"{\n"
"==--\"real\": 120.500000\n"
"==}"
  ) == 0);
#endif

  *basic_choice_s_longint(&choice_0) = 120;
  BASIC_CHOICE_S_TO_BUFFER(&choice_0);
  cassert(strcmp(buffer.data,"{longint:120}") == 0);

#if OPTION_TO_JSON == ENABLED
  BASIC_CHOICE_S_TO_JSON(&choice_0);
  cassert(strcmp(buffer.data,"{\"longint\":120}") == 0);
  BASIC_CHOICE_S_TO_JSON_NICE(&choice_0);
  cassert(strcmp(buffer.data,
"{\n"
"==--\"longint\": 120\n"
"==}"
  ) == 0);
#endif

#endif
}/*}}}*/

void test_dynamic_type_choice()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

  CONT_INIT_CLEAR(json_nice_s,json_nice);
  json_nice_s_create(&json_nice,"--","==",NULL);

#define DYNAMIC_CHOICE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  dynamic_choice_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define DYNAMIC_CHOICE_S_TO_JSON(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  dynamic_choice_s_to_json(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define DYNAMIC_CHOICE_S_TO_JSON_NICE(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  dynamic_choice_s_to_json_nice(NAME,&json_nice,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT_CLEAR(dynamic_choice_s,choice_0);
  *dynamic_choice_s_integer(&choice_0) = 120;
  DYNAMIC_CHOICE_S_TO_BUFFER(&choice_0);
  cassert(strcmp(buffer.data,"{integer:120}") == 0);

#if OPTION_TO_JSON == ENABLED
  DYNAMIC_CHOICE_S_TO_JSON(&choice_0);
  cassert(strcmp(buffer.data,"{\"integer\":120}") == 0);
  DYNAMIC_CHOICE_S_TO_JSON_NICE(&choice_0);
  cassert(strcmp(buffer.data,
"{\n"
"==--\"integer\": 120\n"
"==}"
  ) == 0);
#endif

  string_s_set_ptr(dynamic_choice_s_string(&choice_0),"Hello world!");
  DYNAMIC_CHOICE_S_TO_BUFFER(&choice_0);
  cassert(strcmp(buffer.data,"{string:Hello world!}") == 0);

#if OPTION_TO_JSON == ENABLED
  DYNAMIC_CHOICE_S_TO_JSON(&choice_0);
  cassert(strcmp(buffer.data,"{\"string\":\"Hello world!\"}") == 0);
  DYNAMIC_CHOICE_S_TO_JSON_NICE(&choice_0);
  cassert(strcmp(buffer.data,
"{\n"
"==--\"string\": \"Hello world!\"\n"
"==}"
  ) == 0);
#endif

  ui_array_s *array = dynamic_choice_s_array(&choice_0);

  unsigned idx = 0;
  do {
    ui_array_s_push(array,idx);
  } while(++idx < 10);

  DYNAMIC_CHOICE_S_TO_BUFFER(&choice_0);
  cassert(strcmp(buffer.data,"{array:[0,1,2,3,4,5,6,7,8,9]}") == 0);

#if OPTION_TO_JSON == ENABLED
  DYNAMIC_CHOICE_S_TO_JSON(&choice_0);
  cassert(strcmp(buffer.data,"{\"array\":[0,1,2,3,4,5,6,7,8,9]}") == 0);
  DYNAMIC_CHOICE_S_TO_JSON_NICE(&choice_0);
  cassert(strcmp(buffer.data,
"{\n"
"==--\"array\": [\n"
"==----0,\n"
"==----1,\n"
"==----2,\n"
"==----3,\n"
"==----4,\n"
"==----5,\n"
"==----6,\n"
"==----7,\n"
"==----8,\n"
"==----9\n"
"==--]\n"
"==}"
  ) == 0);
#endif

#endif
}/*}}}*/

void test_doc_examples()
{/*{{{*/

  // Count of items in dynamic array.
  {
    CONT_INIT_CLEAR(ui_array_s,array);

    unsigned used = array.used;
    fprintf(stderr,"array used: %u\n",used);
  }

  // Iteration over items of dynamic array.
  {
    CONT_INIT_CLEAR(ui_array_s,array);

    unsigned idx = 0;
    do {
      ui_array_s_push(&array,idx);
    } while(++idx < 10);

    if (array.used != 0)
    {
      ui *a_ptr = array.data;
      ui *a_ptr_end = a_ptr + array.used;
      do {
        fprintf(stderr,"array iter value: %u\n",*a_ptr);
      } while(++a_ptr < a_ptr_end);
    }
  }

  // Count of items of dynamic queue.
  {
    CONT_INIT_CLEAR(ui_queue_s,queue);

    unsigned used = queue.used;
    fprintf(stderr,"queue used: %u\n",used);
  }

  // Selection of all items from queue.
  {
    CONT_INIT_CLEAR(ui_queue_s,queue);

    unsigned idx = 0;
    do {
      ui_queue_s_insert(&queue,idx);
    } while(++idx < 10);

    while (queue.used > 0)
    {
      ui value = ui_queue_s_next(&queue);
      fprintf(stderr,"queue next value: %u\n",value);
    }
  }

  // Iteration over items of dynamic queue.
  {
    CONT_INIT_CLEAR(ui_queue_s,queue);

    unsigned idx = 0;
    do {
      ui_queue_s_insert(&queue,idx);
    } while(++idx < 10);

    if (queue.used != 0)
    {
      unsigned idx = 0;
      do {
        ui *value_ptr = ui_queue_s_at(&queue,idx);
        fprintf(stderr,"queue iter value: %u\n",*value_ptr);
      } while(++idx < queue.used);
    }
  }

  // Count of elements in linked list (only for save variant of linked list).
  {
    CONT_INIT_CLEAR(ui_list_s,list);

    unsigned count = list.count;
    fprintf(stderr,"list count: %u\n",count);
  }

  // Iteration over elements of linked list.
  {
    CONT_INIT_CLEAR(ui_list_s,list);

    unsigned idx = 0;
    do {
      ui_list_s_append(&list,idx);
    } while(++idx < 10);

    if (list.first_idx != c_idx_not_exist)
    {
      unsigned idx = list.first_idx;
      do {
        ui *value_ptr = ui_list_s_at(&list,idx);
        fprintf(stderr,"list iter value: %u\n",*value_ptr);

        idx = ui_list_s_next_idx(&list,idx);
      } while(idx != c_idx_not_exist);
    }
  }

  // Count of nodes in balanced tree (only for save variant of balanced tree).
  {
    CONT_INIT_CLEAR(ui_tree_s,tree);

    unsigned count = tree.count;
    fprintf(stderr,"count: %u\n",count);
  }

  // Iteration over nodes of balanced tree in tree order (from smallest to
  // biggest).
  {
    CONT_INIT_CLEAR(ui_tree_s,tree);

    unsigned idx = 0;
    do {
      ui_tree_s_insert(&tree,idx);
    } while(++idx < 10);

    if (tree.root_idx != c_idx_not_exist)
    {
      unsigned idx = ui_tree_s_get_min_value_idx(&tree,tree.root_idx);
      do {
        ui *value_ptr = ui_tree_s_at(&tree,idx);
        fprintf(stderr,"tree iter value: %u\n",*value_ptr);

        idx = ui_tree_s_get_next_idx(&tree,idx);
      } while(idx != c_idx_not_exist);
    }
  }

  //Iteration over nodes of balanced tree in tree order (from smallest to
  //biggest). Iteration uses stack provided to balanced tree function as
  //temporary index storage.
  {
    CONT_INIT_CLEAR(ui_tree_s,tree);

    unsigned idx = 0;
    do {
      ui_tree_s_insert(&tree,idx);
    } while(++idx < 10);

    if (tree.root_idx != c_idx_not_exist)
    {
      unsigned stack[RB_TREE_STACK_SIZE(ui_tree_s,&tree)];
      unsigned *stack_ptr = stack;

      unsigned idx = ui_tree_s_get_stack_min_value_idx(&tree,tree.root_idx,&stack_ptr);
      do {
        ui *value_ptr = ui_tree_s_at(&tree,idx);
        fprintf(stderr,"tree stack iter value: %u\n",*value_ptr);

        idx = ui_tree_s_get_stack_next_idx(&tree,idx,&stack_ptr,stack);
      } while(idx != c_idx_not_exist);
    }
  }

  // Out of order iteration over nodes of balanced tree. Available only for
  // `save` variant of balanced tree.
  {
    CONT_INIT_CLEAR(ui_tree_s,tree);

    unsigned idx = 0;
    do {
      ui_tree_s_insert(&tree,idx);
    } while(++idx < 10);

    if (tree.root_idx != c_idx_not_exist)
    {
      ui_tree_s_node *utn_ptr = tree.data;
      ui_tree_s_node *utn_ptr_end = utn_ptr + tree.used;
      do {
        if (utn_ptr->valid)
        {
          unsigned value = utn_ptr->object;
          fprintf(stderr,"tree safe iter value: %u\n",value);
        }
      } while(++utn_ptr < utn_ptr_end);
    }
  }
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

