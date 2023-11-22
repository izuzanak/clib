
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libutf8proc_cll_test";

const char *test_names[] =
{/*{{{*/
  "version",
  "utf8_to_lower",
  "utf8_to_upper",
  "utf8_map",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_version,
  test_utf8_to_lower,
  test_utf8_to_upper,
  test_utf8_map,
};/*}}}*/

// === test execution functions ================================================

void test_version()
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,buffer);
  utf8proc_s_version(&buffer);
}/*}}}*/

void test_utf8_to_lower()
{/*{{{*/
  const char *source = "Jiří Zuzaňák";
  unsigned length = strlen(source);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  cassert(utf8proc_s_to_lower(length,source,&buffer) == 0);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"jiří zuzaňák") == 0);
}/*}}}*/

void test_utf8_to_upper()
{/*{{{*/
  const char *source = "Jiří Zuzaňák";
  unsigned length = strlen(source);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  cassert(utf8proc_s_to_upper(length,source,&buffer) == 0);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"JIŘÍ ZUZAŇÁK") == 0);
}/*}}}*/

void test_utf8_map()
{/*{{{*/
  const char *source = "Jiří Zuzaňák";
  unsigned length = strlen(source);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  cassert(utf8proc_s_map(length,source,
        UTF8PROC_NULLTERM | UTF8PROC_STABLE | UTF8PROC_STRIPMARK | UTF8PROC_DECOMPOSE | UTF8PROC_CASEFOLD
        ,&buffer) == 0);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"jiri zuzanak") == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

