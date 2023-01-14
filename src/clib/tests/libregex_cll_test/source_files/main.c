
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libregex_cll_test";

const char *test_names[] =
{/*{{{*/
  "regex",
  "regex_split",
  "regex_replace",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_regex,
  test_regex_split,
  test_regex_replace,
};/*}}}*/

// === methods of generated structures =========================================

// -- regex_array_s --
@begin
methods regex_array_s
@end

// === test execution functions ================================================

void test_regex()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define REGEX_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  regex_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define REGMATCH_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  regmatch_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - regex_s_create -
  CONT_INIT(regex_s,regex_0);
  cassert(regex_s_create(&regex_0,"^\\[(regex_s\\{.*\\},){4,4}(regex_s\\{.*\\})\\]$") == 0);
  cassert(regex_s_compare(&regex_0,&regex_0));

  buffer.used = 0;
  regex_s_to_string(&regex_0,&buffer);

  CONT_INIT(regex_array_s,regex_array);

  unsigned idx = 0;
  do {
    regex_array_s_push_blank(&regex_array);
    cassert(regex_s_create(regex_array_s_last(&regex_array),"^.*$") == 0);
  } while(++idx < 5);

  CONT_INIT(bc_array_s,test_buffer);
  regex_array_s_to_string(&regex_array,&test_buffer);
  bc_array_s_push(&test_buffer,'\0');

  // - regex_s_match -
  regmatch_s match;
  cassert(regex_s_match(&regex_0,test_buffer.data,&match));

  // - regex_s_match_n -
  CONT_INIT(regmatch_array_s,match_array);
  cassert(regex_s_match_n(&regex_0,test_buffer.data,10,&match_array) && match_array.used == 3);

  regmatch_array_s_clear(&match_array);
  bc_array_s_clear(&test_buffer);
  regex_array_s_clear(&regex_array);
  regex_s_clear(&regex_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_regex_split()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define STRING_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  string_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  char data_0[] = "Some text to be splitted";
  string_s string_0 = {strlen(data_0) + 1,data_0};

  CONT_INIT(regex_s,regex);
  cassert(regex_s_create(&regex," ") == 0);

  // - regex_s_split -
  CONT_INIT(string_array_s,array_0);
  cassert(regex_s_split(&regex,&string_0,&array_0) == 0);
  STRING_ARRAY_S_TO_BUFFER(&array_0);
  cassert(strcmp(buffer.data,"[Some,text,to,be,splitted]") == 0);

  cassert(regex_s_create(&regex,"e") == 0);
  cassert(regex_s_split(&regex,&string_0,&array_0) == 0);
  STRING_ARRAY_S_TO_BUFFER(&array_0);
  cassert(strcmp(buffer.data,"[Som, t,xt to b, splitt,d]") == 0);

  char data_1[] = "abacaaadaaaaeaaaaa";
  string_s string_1 = {strlen(data_1) + 1,data_1};

  cassert(regex_s_create(&regex,"aa*") == 0);
  cassert(regex_s_split(&regex,&string_1,&array_0) == 0);
  STRING_ARRAY_S_TO_BUFFER(&array_0);
  cassert(strcmp(buffer.data,"[,b,c,d,e,]") == 0);

  string_array_s_clear(&array_0);
  regex_s_clear(&regex);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_regex_replace()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  char data_0[] = "abacaaadaaaaeaaaaa";
  string_s string_0 = {strlen(data_0) + 1,data_0};

  char data_1[] = " - ";
  string_s string_1 = {strlen(data_1) + 1,data_1};

  CONT_INIT(regex_s,regex);
  cassert(regex_s_create(&regex,"aa*") == 0);
  cassert(regex_s_replace(&regex,&string_0,&string_1,&buffer) == 0);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data," - b - c - d - e - ") == 0);

  regex_s_clear(&regex);
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

