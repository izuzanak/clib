
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libimage_cll_test";

const char *test_names[] =
{/*{{{*/
  "create",
  "create_data",
  "png_format",
  "jpeg_format",
  "operations",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_create,
  test_create_data,
  test_png_format,
  test_jpeg_format,
  test_operations,
};/*}}}*/

// === test execution functions ================================================

void test_create()
{/*{{{*/
  CONT_INIT_CLEAR(image_s,image);
  cassert(image_s_create(&image,1024,768,c_image_pixel_format_RGB24) == 0);
}/*}}}*/

void test_create_data()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libimage_cll_test/resources/raw_image.bin","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  cassert(file_s_read_close(&file,&buffer) == 0);

  CONT_INIT_CLEAR(image_s,image);
  cassert(image_s_create_data(&image,300,169,c_image_pixel_format_GRAY8,buffer.used,buffer.data) == 0);
}/*}}}*/

void test_png_format()
{/*{{{*/
  CONT_INIT_CLEAR(image_s,image);
  cassert(image_s_read_png_file(&image,"tests/libimage_cll_test/resources/image.png") == 0);
  cassert(image_s_write_png_file(&image,"tests/libimage_cll_test/png_format/copy.png") == 0);

  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libimage_cll_test/resources/image.png","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  cassert(file_s_read_close(&file,&buffer) == 0);
  cassert(image_s_read_png_data(&image,buffer.used,buffer.data) == 0);
}/*}}}*/

void test_jpeg_format()
{/*{{{*/
  CONT_INIT_CLEAR(image_s,image);
  cassert(image_s_read_jpeg_file(&image,"tests/libimage_cll_test/resources/image.jpeg") == 0);
  cassert(image_s_write_jpeg_file(&image,"tests/libimage_cll_test/jpeg_format/copy.jpeg",90) == 0);

  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libimage_cll_test/resources/image.jpeg","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  cassert(file_s_read_close(&file,&buffer) == 0);
  cassert(image_s_read_jpeg_data(&image,buffer.used,buffer.data) == 0);
}/*}}}*/

void test_operations()
{/*{{{*/
  CONT_INIT_CLEAR(image_s,image);
  cassert(image_s_read_png_file(&image,"tests/libimage_cll_test/resources/image.png") == 0);

  CONT_INIT_CLEAR(image_s,duplicate);
  cassert(image_s_duplicate(&duplicate,&image) == 0);
  cassert(image_s_write_png_file(&duplicate,"tests/libimage_cll_test/operations/duplicate.png") == 0);

  CONT_INIT_CLEAR(image_s,reffered);
  cassert(image_s_create_referred(&reffered,100,100,100,100,&image) == 0);
  cassert(image_s_write_png_file(&reffered,"tests/libimage_cll_test/operations/referred.png") == 0);

  CONT_INIT_CLEAR(image_s,cleared);
  cassert(image_s_duplicate(&cleared,&image) == 0);
  cassert(image_s_io_clear(&cleared) == 0);
  cassert(image_s_write_png_file(&cleared,"tests/libimage_cll_test/operations/cleared.png") == 0);

  unsigned char color[] = {255,0,0,0};
  CONT_INIT_CLEAR(image_s,filled);
  cassert(image_s_duplicate(&filled,&image) == 0);
  cassert(image_s_io_fill(&filled,color) == 0);
  cassert(image_s_write_png_file(&filled,"tests/libimage_cll_test/operations/filled.png") == 0);

  CONT_INIT_CLEAR(image_s,copy);
  cassert(image_s_create(&copy,image.width,image.height,image.pixel_format) == 0);
  cassert(image_s_io_copy(&copy,&image) == 0);
  cassert(image_s_write_png_file(&copy,"tests/libimage_cll_test/operations/copy.png") == 0);

  // FIXME TODO ... test image_s_io_apply

  CONT_INIT_CLEAR(image_s,converted);
  cassert(image_s_create(&converted,image.width,image.height,c_image_pixel_format_GRAY8) == 0);
  cassert(image_s_io_convert(&converted,&image) == 0);
  cassert(image_s_write_png_file(&converted,"tests/libimage_cll_test/operations/converted.png") == 0);

  CONT_INIT_CLEAR(image_s,normalized);
  cassert(image_s_create(&normalized,converted.width,converted.height,c_image_pixel_format_DOUBLE) == 0);
  cassert(image_s_io_convert(&normalized,&converted) == 0);
  cassert(image_s_io_normalize(&normalized,&normalized) == 0);
  cassert(image_s_io_convert(&converted,&normalized) == 0);
  cassert(image_s_write_png_file(&converted,"tests/libimage_cll_test/operations/normalized.png") == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

