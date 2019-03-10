
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libpcre_cll_test";

const char *test_names[] =
{/*{{{*/
  "pcre",
  "pcre_split",
  "pcre_replace",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_pcre,
  test_pcre_split,
  test_pcre_replace,
};/*}}}*/

// === methods of generated structures =========================================

// -- pcre_array_s --
@begin
methods pcre_array_s
@end

// === test execution functions ================================================

void test_pcre()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define REGEX_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  pcre_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define REGMATCH_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  regmatch_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - pcre_s_create -
  CONT_INIT(pcre_s,pcre_0);
  cassert(pcre_s_create(&pcre_0,"^\\[(pcre_s{.*},){4,4}(pcre_s{.*})\\]$") == 0);

  CONT_INIT(pcre_array_s,pcre_array);

  unsigned idx = 0;
  do {
    pcre_array_s_push_blank(&pcre_array);
    cassert(pcre_s_create(pcre_array_s_last(&pcre_array),"^.*$") == 0);
  } while(++idx < 5);

  CONT_INIT(bc_array_s,test_buffer);
  pcre_array_s_to_string(&pcre_array,&test_buffer);
  bc_array_s_push(&test_buffer,'\0');

  // - pcre_s_match -
  regmatch_s match;
  cassert(pcre_s_match(&pcre_0,test_buffer.data,&match));

  // - pcre_s_match_n -
  CONT_INIT(regmatch_array_s,match_array);
  cassert(pcre_s_match_n(&pcre_0,test_buffer.data,10,&match_array) && match_array.used == 3);

  regmatch_array_s_clear(&match_array);
  bc_array_s_clear(&test_buffer);
  pcre_array_s_clear(&pcre_array);
  pcre_s_clear(&pcre_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_pcre_split()
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

  CONT_INIT(pcre_s,pcre);
  cassert(pcre_s_create(&pcre," ") == 0);

  // - pcre_s_split -
  CONT_INIT(string_array_s,array_0);
  cassert(pcre_s_split(&pcre,&string_0,&array_0) == 0);
  STRING_ARRAY_S_TO_BUFFER(&array_0);
  cassert(strcmp(buffer.data,"[Some,text,to,be,splitted]") == 0);

  cassert(pcre_s_create(&pcre,"e") == 0);
  cassert(pcre_s_split(&pcre,&string_0,&array_0) == 0);
  STRING_ARRAY_S_TO_BUFFER(&array_0);
  cassert(strcmp(buffer.data,"[Som, t,xt to b, splitt,d]") == 0);

  char data_1[] = "abacaaadaaaaeaaaaa";
  string_s string_1 = {strlen(data_1) + 1,data_1};

  cassert(pcre_s_create(&pcre,"aa*") == 0);
  cassert(pcre_s_split(&pcre,&string_1,&array_0) == 0);
  STRING_ARRAY_S_TO_BUFFER(&array_0);
  cassert(strcmp(buffer.data,"[,b,c,d,e,]") == 0);

  string_array_s_clear(&array_0);
  pcre_s_clear(&pcre);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_pcre_replace()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  char data_0[] = "abacaaadaaaaeaaaaa";
  string_s string_0 = {strlen(data_0) + 1,data_0};

  char data_1[] = " - ";
  string_s string_1 = {strlen(data_1) + 1,data_1};

  CONT_INIT(pcre_s,pcre);
  cassert(pcre_s_create(&pcre,"aa*") == 0);
  cassert(pcre_s_replace(&pcre,&string_0,&string_1,&buffer) == 0);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data," - b - c - d - e - ") == 0);

  pcre_s_clear(&pcre);
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

