
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libiconv_cll_test";

const char *test_names[] =
{/*{{{*/
  "iconv_create",
  "iconv_convert",
  "iconv_convert_error",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_iconv_create,
  test_iconv_convert,
  test_iconv_convert_error,
};/*}}}*/

// === test execution functions ================================================

void test_iconv_create()
{/*{{{*/

  // - test successful create -
  CONT_INIT_CLEAR(iconv_s,iconv_0);
  cassert(iconv_s_create(&iconv_0,"UTF-8","ISO-8859-1") == 0);

  // - test create with same encoding -
  CONT_INIT_CLEAR(iconv_s,iconv_1);
  cassert(iconv_s_create(&iconv_1,"UTF-8","UTF-8") == 0);

  // - test compare -
  cassert(iconv_s_compare(&iconv_0,&iconv_0));
  cassert(!iconv_s_compare(&iconv_0,&iconv_1));

#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);
  iconv_s_to_string(&iconv_0,&buffer);
  cassert(buffer.used > 0);
#endif
}/*}}}*/

void test_iconv_convert()
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,buffer);

  // - test ISO-8859-1 to UTF-8 -
  {
    CONT_INIT_CLEAR(iconv_s,iconv_0);
    cassert(iconv_s_create(&iconv_0,"UTF-8","ISO-8859-1") == 0);

    // - ISO-8859-1 bytes for "cafe" with e-acute (0xe9) -
    const char src[] = {'c','a','f','\xe9'};
    buffer.used = 0;
    cassert(iconv_s_convert(&iconv_0,4,src,&buffer) == 0);

    // - UTF-8 e-acute is 0xc3 0xa9 -
    cassert(buffer.used == 5);
    cassert(strcmp(buffer.data,"caf\xc3\xa9") == 0);
  }

  // - test UTF-8 to ISO-8859-1 -
  {
    CONT_INIT_CLEAR(iconv_s,iconv_1);
    cassert(iconv_s_create(&iconv_1,"ISO-8859-1","UTF-8") == 0);

    const char *src = "caf\xc3\xa9";
    buffer.used = 0;
    cassert(iconv_s_convert(&iconv_1,strlen(src),src,&buffer) == 0);
    cassert(buffer.used == 4);
    cassert((unsigned char)buffer.data[3] == 0xe9);
  }

  // - test empty input -
  {
    CONT_INIT_CLEAR(iconv_s,iconv_2);
    cassert(iconv_s_create(&iconv_2,"UTF-8","ISO-8859-1") == 0);

    buffer.used = 0;
    cassert(iconv_s_convert(&iconv_2,0,"",&buffer) == 0);
    cassert(buffer.used == 0);
  }

  // - test conversion that triggers buffer growth (E2BIG) -
  {
    CONT_INIT_CLEAR(iconv_s,iconv_3);
    cassert(iconv_s_create(&iconv_3,"UTF-32LE","UTF-8") == 0);

    // - each ASCII byte becomes 4 bytes in UTF-32, forcing buffer growth -
    const char *src = "Hello World";
    unsigned src_len = strlen(src);
    buffer.used = 0;
    cassert(iconv_s_convert(&iconv_3,src_len,src,&buffer) == 0);
    cassert(buffer.used == src_len * 4);
  }
}/*}}}*/

void test_iconv_convert_error()
{/*{{{*/

  // - test invalid encoding name -
  {
    CONT_INIT_CLEAR(iconv_s,iconv_0);
    cassert(iconv_s_create(&iconv_0,"INVALID-ENCODING","UTF-8") != 0);
  }

  // - test invalid input sequence -
  {
    CONT_INIT_CLEAR(iconv_s,iconv_1);
    cassert(iconv_s_create(&iconv_1,"UTF-8","UTF-8") == 0);

    CONT_INIT_CLEAR(bc_array_s,buffer);

    // - invalid UTF-8 sequence -
    const char invalid[] = {'\xff','\xfe'};
    cassert(iconv_s_convert(&iconv_1,2,invalid,&buffer) != 0);

    // - buffer should be unchanged after error -
    cassert(buffer.used == 0);
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
