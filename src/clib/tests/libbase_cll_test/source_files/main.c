
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libbase_cll_test";

const char *test_names[] =
{/*{{{*/
  "bc_array",
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
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_bc_array,
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

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_string()
{/*{{{*/

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

  // - string_s_clear -
  string_s_clear(&string_1);
  cassert(!string_s_compare(&string_0,&string_1));

  string_s_clear(&string_1);
  string_s_clear(&string_0);
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

  // - ui ui_array_s_pop -
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

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

