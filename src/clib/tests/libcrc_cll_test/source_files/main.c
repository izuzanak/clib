
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libcrc_cll_test";

const char *test_names[] =
{/*{{{*/
  "crc16",
  "crc32",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_crc16,
  test_crc32,
};/*}}}*/

// === test execution functions ================================================

void test_crc16()
{/*{{{*/
  crc16_s crc;

  crc = 0x5555;
  crc16_s_update(&crc,0,"");
  cassert(crc == 21845);

  crc = 0x5555;
  char data_0[] = "Hello world!!!";
  crc16_s_update(&crc,sizeof(data_0) - 1,data_0);
  cassert(crc == 18651);

  crc = 0x5555;
  char data_1[256];
  memset(data_1,0,sizeof(data_1));
  crc16_s_update(&crc,sizeof(data_1),data_1);
  cassert(crc == 62833);
}/*}}}*/

void test_crc32()
{/*{{{*/
  crc32_s crc;

  crc = 0x55555555;
  crc32_s_update(&crc,0,"");
  cassert(crc == 1431655765);

  crc = 0x55555555;
  char data_0[] = "Hello world!!!";
  crc32_s_update(&crc,sizeof(data_0) - 1,data_0);
  cassert(crc == 586538615);

  crc = 0x55555555;
  char data_1[256];
  memset(data_1,0,sizeof(data_1));
  crc32_s_update(&crc,sizeof(data_1),data_1);
  cassert(crc == 3876072354);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

