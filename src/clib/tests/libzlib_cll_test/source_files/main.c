
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libzlib_cll_test";

const char *test_names[] =
{/*{{{*/
  "crc32",
  "compress",
  "gz_file",
  "gz_temp",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_crc32,
  test_compress,
  test_gz_file,
  test_gz_temp,
};/*}}}*/

// === test execution functions ================================================

void test_crc32()
{/*{{{*/
  zlib_crc32_s crc;

  crc = ~0x55555555;
  zlib_crc32_s_update(&crc,0,"");
  cassert(~crc == 1431655765);

  crc = ~0x55555555;
  char data_0[] = "Hello world!!!";
  zlib_crc32_s_update(&crc,sizeof(data_0) - 1,data_0);
  cassert(~crc == 586538615);

  crc = ~0x55555555;
  char data_1[256];
  memset(data_1,0,sizeof(data_1));
  zlib_crc32_s_update(&crc,sizeof(data_1),data_1);
  cassert(~crc == 3876072354);
}/*}}}*/

void test_compress()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,data_file);
  cassert(file_s_open(&data_file,"tests/libzlib_cll_test/resources/file.xml","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&data_file,&data) == 0);

  // - zlib_compress -
  CONT_INIT_CLEAR(bc_array_s,compressed);
  cassert(zlib_compress(data.data,data.used,9,&compressed) == 0);

  unsigned offset = compressed.used;
  cassert(zlib_compress(data.data,data.used,9,&compressed) == 0);
  cassert(memcmp(compressed.data,compressed.data + offset,offset) == 0);

  // - zlib_uncompress -
  CONT_INIT_CLEAR(bc_array_s,uncompressed);
  cassert(zlib_uncompress(compressed.data,offset,data.used,&uncompressed) == 0);
  cassert(bc_array_s_compare(&data,&uncompressed));

  unsigned un_offset = uncompressed.used;
  cassert(zlib_uncompress(compressed.data,offset,data.used,&uncompressed) == 0);
  cassert(memcmp(uncompressed.data,uncompressed.data + un_offset,un_offset) == 0);
}/*}}}*/

void test_gz_file()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,data_file);
  cassert(file_s_open(&data_file,"tests/libzlib_cll_test/resources/file.xml","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&data_file,&data) == 0);

  // - gz_file_s_open -
  CONT_INIT_CLEAR(gz_file_s,gz_file);
  cassert(gz_file_s_open(&gz_file,"tests/libzlib_cll_test/gz_file/file.gz","w") == 0);

  // - gz_file_s_write -
  cassert(gz_file_s_write(&gz_file,data.data,1000) == 0);

  // - gz_file_s_write_close -
  cassert(gz_file_s_write_close(&gz_file,data.data + 1000,data.used - 1000) == 0);

  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libzlib_cll_test/gz_file/file.gz","r") == 0);

  // - gz_file_s_fd_open -
  cassert(gz_file_s_fd_open(&gz_file,dup(stream_s_fd(&file)),"r") == 0);
  file_s_clear(&file);

  // - gz_file_s_read -
  CONT_INIT_CLEAR(bc_array_s,buffer);
  cassert(gz_file_s_read(&gz_file,&buffer) == 0);
  cassert(data.used == buffer.used && bc_array_s_compare(&data,&buffer));

  // - gz_file_s_seek -
  z_off_t seek_offset;
  cassert(gz_file_s_seek(&gz_file,buffer.used - 8,SEEK_SET,&seek_offset) == 0);
  cassert(seek_offset == 3377);

  // - gz_file_s_tell -
  z_off_t tell_offset;
  cassert(gz_file_s_tell(&gz_file,&tell_offset) == 0);
  cassert(tell_offset == 3377);

  // - gz_file_s_read_close -
  buffer.used = 0;
  cassert(gz_file_s_read_close(&gz_file,&buffer) == 0);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"</ROOT>\n") == 0);
}/*}}}*/

void test_gz_temp()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,data_file);
  cassert(file_s_open(&data_file,"tests/libzlib_cll_test/resources/file.xml","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&data_file,&data) == 0);

  // - compress to temporary file -
  CONT_INIT_CLEAR(file_s,tmp_comp_file);
  cassert((tmp_comp_file = tmpfile()) != NULL);

  CONT_INIT_CLEAR(gz_file_s,gz_file);
  cassert(gz_file_s_fd_open(&gz_file,dup(stream_s_fd(&tmp_comp_file)),"w") == 0);
  cassert(gz_file_s_write_close(&gz_file,data.data,data.used) == 0);

  CONT_INIT_CLEAR(bc_array_s,compressed);
  cassert(file_s_seek(&tmp_comp_file,SEEK_SET,0) == 0);
  cassert(file_s_read_close(&tmp_comp_file,&compressed) == 0);

  // - uncompress from temporary file -
  CONT_INIT_CLEAR(file_s,tmp_decomp_file);
  cassert((tmp_decomp_file = tmpfile()) != NULL);

  cassert(gz_file_s_fd_open(&gz_file,dup(stream_s_fd(&tmp_decomp_file)),"r") == 0);
  cassert(stream_s_write(&tmp_decomp_file,compressed.data,compressed.used) == 0);
  cassert(file_s_seek(&tmp_decomp_file,SEEK_SET,0) == 0);
  file_s_clear(&tmp_decomp_file);

  CONT_INIT_CLEAR(bc_array_s,uncompressed);
  cassert(gz_file_s_read_close(&gz_file,&uncompressed) == 0);

  // - test data -
  cassert(bc_array_s_compare(&data,&uncompressed));
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

