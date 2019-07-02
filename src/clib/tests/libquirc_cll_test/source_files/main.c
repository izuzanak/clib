
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libquirc_cll_test";

const char *test_names[] =
{/*{{{*/
  "create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_create,
};/*}}}*/

// === test execution functions ================================================

void test_create()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libquirc_cll_test/resources/raw_image.bin","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  cassert(file_s_read_close(&file,&buffer) == 0);

  CONT_INIT_CLEAR(quirc_s,quirc);
  cassert(quirc_s_create(&quirc,300,169) == 0);
  cassert(quirc_s_process(&quirc,buffer.data,buffer.used) == 0);

  quirc_code_s code;
  cassert(quirc_s_extract(&quirc,0,&code) == 0);

  quirc_data_s data;
  cassert(quirc_code_s_decode(&code,&data) == 0);

  CONT_INIT_CLEAR(bc_array_s,payload);
  quirc_data_s_payload(&data,&payload);
  bc_array_s_push(&payload,'\0');
  cassert(strcmp(payload.data,"This is the 1st camera in vehicle 45") == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

