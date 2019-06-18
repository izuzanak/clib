
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libsnappy_cll_test";

const char *test_names[] =
{/*{{{*/
  "compress",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_compress,
};/*}}}*/

// === methods of generated structures =========================================

// === test execution functions ================================================

void test_compress()
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,data);
  CONT_INIT_CLEAR(bc_array_s,snappy_data);
  CONT_INIT_CLEAR(bc_array_s,uncompressed_data);

  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libsnappy_cll_test/resources/file.xml","r") == 0);
  cassert(file_s_read_close(&file,&data) == 0);

  cassert(cl_snappy_compress(data.data,data.used,&snappy_data) == 0);
  cassert(cl_snappy_uncompress(snappy_data.data,snappy_data.used,&uncompressed_data) == 0);
  cassert(bc_array_s_compare(&data,&uncompressed_data));

  printf("Compress ratio: %.2f\n",100.0*snappy_data.used/data.used);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

