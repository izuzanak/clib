
@begin
include "cl_image.h"
@end

// === methods of structure image_s ============================================

int image_s_create(image_s *this,unsigned a_width,unsigned a_height,unsigned a_format)
{/*{{{*/

  // - ERROR -
  if (a_width <= 0 || a_height <= 0)
  {
    throw_error(IMAGE_WRONG_PROPERTIES);
  }

  switch (a_format)
  {
  case c_image_pixel_format_GRAY8:
  case c_image_pixel_format_RGB24:
  case c_image_pixel_format_RGBA:
  case c_image_pixel_format_DOUBLE:
  case c_image_pixel_format_COMPLEX:
    break;

  // - ERROR -
  default:
    throw_error(IMAGE_WRONG_PROPERTIES);
  }

  // - create image content -
  if (!image_internal_s_create(this,a_width,a_height,a_format))
  {
    throw_error(IMAGE_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

int image_s_create_data(image_s *this,
    unsigned a_width,unsigned a_height,unsigned a_format,unsigned a_size,const char *a_data)
{/*{{{*/

  // - ERROR -
  if (a_width <= 0 || a_height <= 0)
  {
    throw_error(IMAGE_WRONG_PROPERTIES);
  }

  switch (a_format)
  {
  case c_image_pixel_format_GRAY8:
  case c_image_pixel_format_RGB24:
  case c_image_pixel_format_RGBA:
  case c_image_pixel_format_DOUBLE:
  case c_image_pixel_format_COMPLEX:
    break;

  // - ERROR -
  default:

    throw_error(IMAGE_WRONG_PROPERTIES);
  }

  unsigned line_bytes = a_size / a_height;

  // - ERROR -
  if (a_height*line_bytes != a_size || a_width*c_pixel_sizes[a_format] > line_bytes)
  {
    throw_error(IMAGE_INVALID_SOURCE_DATA_SIZE);
  }

  // - image data structure -
  image_internal_data_s image_data;
  image_data.line_bytes = line_bytes;
  image_data.data = (unsigned char *)a_data;

  // - image structure -
  image_internal_s image;
  image.root = 1;
  image.pixel_format = a_format;
  image.width = a_width;
  image.height = a_height;
  image.x_pos = 0;
  image.y_pos = 0;
  image.pixel_step = c_pixel_sizes[a_format];
  image.image_data_ptr = &image_data;

  // - copy image data -
  image_internal_s_copy(this,&image);

  return 0;
}/*}}}*/

#define IMAGE_CREATE_ROW_PTRS(IMG_PTR,ROW_PTRS) \
{/*{{{*/\
  unsigned line_size = (IMG_PTR)->image_data_ptr->line_bytes;\
\
  unsigned char *ptr = (IMG_PTR)->image_data_ptr->data + (IMG_PTR)->y_pos*line_size + (IMG_PTR)->x_pos*(IMG_PTR)->pixel_step;\
  unsigned char *ptr_end = ptr + ((IMG_PTR)->height - 1)*line_size + (IMG_PTR)->width*(IMG_PTR)->pixel_step;\
  png_bytepp r_ptr = ROW_PTRS;\
\
  do {\
    *r_ptr++ = ptr;\
\
    ptr += line_size;\
  } while(ptr < ptr_end);\
}/*}}}*/

#define IMAGE_READ_PNG_DATA(RELEASE_CODE) \
{/*{{{*/\
  png_set_sig_bytes(png_ptr,8);\
  png_read_info(png_ptr,info_ptr);\
\
  unsigned width      = png_get_image_width (png_ptr,info_ptr);\
  unsigned height     = png_get_image_height(png_ptr,info_ptr);\
  png_byte color_type = png_get_color_type  (png_ptr,info_ptr);\
  png_byte bit_depth  = png_get_bit_depth   (png_ptr,info_ptr);\
\
  /* - ERROR - */\
  if (bit_depth != 8)\
  {\
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);\
    RELEASE_CODE;\
\
    throw_error(IMAGE_UNSUPPORTED_PIXEL_FORMAT);\
  }\
\
  unsigned pixel_format;\
\
  switch (color_type)\
  {\
  case PNG_COLOR_TYPE_GRAY:\
    pixel_format = c_image_pixel_format_GRAY8;\
    break;\
  case PNG_COLOR_TYPE_RGB:\
    pixel_format = c_image_pixel_format_RGB24;\
    break;\
  case PNG_COLOR_TYPE_RGB_ALPHA:\
    pixel_format = c_image_pixel_format_RGBA;\
    break;\
\
  /* - ERROR - */\
  default:\
\
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);\
    RELEASE_CODE;\
\
    throw_error(IMAGE_UNSUPPORTED_PIXEL_FORMAT);\
  }\
\
  /* - create image content - */\
  image_internal_s_create(this,width,height,pixel_format);\
\
  /* - ERROR - */\
  if (png_get_rowbytes(png_ptr,info_ptr) > this->image_data_ptr->line_bytes)\
  {\
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);\
    RELEASE_CODE;\
\
    throw_error(IMAGE_UNSUPPORTED_PIXEL_FORMAT);\
  }\
\
  /* - create target row pointer array - */\
  png_bytepp row_ptrs = (png_bytepp)cmalloc(height*sizeof(png_bytep));\
\
  /* - fill target row pointer array - */\
  IMAGE_CREATE_ROW_PTRS(this,row_ptrs)\
\
  /* - ERROR - */\
  if (setjmp(png_jmpbuf(png_ptr)))\
  {\
    cfree(row_ptrs);\
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);\
    RELEASE_CODE;\
\
    throw_error(IMAGE_PNG_DATA_READ_ERROR);\
  }\
\
  /* - read image data - */\
  png_read_image(png_ptr,row_ptrs);\
\
  /* - release resources - */\
  cfree(row_ptrs);\
  png_destroy_read_struct(&png_ptr,&info_ptr,NULL);\
  RELEASE_CODE;\
}/*}}}*/

int image_s_read_png_data(image_s *this,unsigned a_size,const char *a_data)
{/*{{{*/

  // - ERROR -
  if (a_size < 8 || png_sig_cmp((unsigned char *)a_data,0,8) != 0)
  {
    throw_error(IMAGE_PNG_DATA_READ_ERROR);
  }

  // - create png reader structure -
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

  // - ERROR -
  if (png_ptr == NULL)
  {
    throw_error(IMAGE_PNG_READ_INIT_ERROR);
  }

  // - create info structure -
  png_infop info_ptr = png_create_info_struct(png_ptr);

  // - ERROR -
  if (info_ptr == NULL)
  {
    png_destroy_read_struct(&png_ptr,NULL,NULL);

    throw_error(IMAGE_PNG_READ_INIT_ERROR);
  }

  // - ERROR -
  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);

    throw_error(IMAGE_PNG_READ_INIT_ERROR);
  }

  // - create png data object -
  png_data_s png_data;
  png_data.data = a_data;
  png_data.size = a_size;
  png_data.read = 8;

  png_set_read_fn(png_ptr,&png_data,png_data_s_read_data);

  IMAGE_READ_PNG_DATA();

  return 0;
}/*}}}*/

int image_s_read_png_file(image_s *this,const char *a_path)
{/*{{{*/

  // - open source png file -
  FILE *file = fopen(a_path,"rb");

  // - ERROR -
  if (file == NULL)
  {
    throw_error(IMAGE_CANNOT_OPEN_FILE);
  }

  // - read png file header -
  unsigned char png_header[8];
  size_t read_size = fread(png_header,1,8,file);

  // - ERROR -
  if (read_size != 8 || png_sig_cmp(png_header,0,8) != 0)
  {
    fclose(file);

    throw_error(IMAGE_FILE_IS_NOT_PNG_FILE);
  }

  // - create png reader structure -
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

  // - ERROR -
  if (png_ptr == NULL)
  {
    fclose(file);

    throw_error(IMAGE_PNG_READ_INIT_ERROR);
  }

  // - create info structure -
  png_infop info_ptr = png_create_info_struct(png_ptr);

  // - ERROR -
  if (info_ptr == NULL)
  {
    png_destroy_read_struct(&png_ptr,NULL,NULL);
    fclose(file);

    throw_error(IMAGE_PNG_READ_INIT_ERROR);
  }

  // - ERROR -
  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
    fclose(file);

    throw_error(IMAGE_PNG_FILE_READ_ERROR);
  }

  png_init_io(png_ptr,file);

  IMAGE_READ_PNG_DATA(fclose(file));

  return 0;
}/*}}}*/

int image_s_write_png_file(image_s *this,const char *a_path)
{/*{{{*/

  // - retrieve png image details -
  unsigned width = this->width;
  unsigned height = this->height;
  png_byte color_type;
  png_byte bit_depth;

  switch (this->pixel_format)
  {
  case c_image_pixel_format_GRAY8:
    color_type = PNG_COLOR_TYPE_GRAY;
    bit_depth = 8;
    break;
  case c_image_pixel_format_RGB24:
    color_type = PNG_COLOR_TYPE_RGB;
    bit_depth = 8;
    break;
  case c_image_pixel_format_RGBA:
    color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    bit_depth = 8;
    break;

  // - ERROR -
  default:

    throw_error(IMAGE_UNSUPPORTED_PIXEL_FORMAT);
  }

  // - create png writer structure -
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

  // - ERROR -
  if (png_ptr == NULL)
  {
    throw_error(IMAGE_PNG_WRITE_INIT_ERROR);
  }

  // - create info structure -
  png_infop info_ptr = png_create_info_struct(png_ptr);

  // - ERROR -
  if (info_ptr == NULL)
  {
    png_destroy_write_struct(&png_ptr,NULL);

    throw_error(IMAGE_PNG_WRITE_INIT_ERROR);
  }

  // - open target png file -
  FILE *f = fopen(a_path,"wb");

  // - ERROR -
  if (f == NULL)
  {
    png_destroy_write_struct(&png_ptr,&info_ptr);

    throw_error(IMAGE_CANNOT_OPEN_FILE);
  }

  // - ERROR -
  if (setjmp(png_jmpbuf(png_ptr)))
  {
    fclose(f);
    png_destroy_write_struct(&png_ptr,&info_ptr);

    throw_error(IMAGE_PNG_FILE_WRITE_ERROR);
  }

  png_init_io(png_ptr,f);

  // - write png image header -
  png_set_IHDR(png_ptr,info_ptr,
      width,height,bit_depth,color_type,
      PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_BASE,PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr,info_ptr);

  // - create target row pointer array -
  png_bytepp row_ptrs = (png_bytepp)cmalloc(height*sizeof(png_bytep));

  // - fill target row pointer array -
  IMAGE_CREATE_ROW_PTRS(this,row_ptrs);

  // - ERROR -
  if (setjmp(png_jmpbuf(png_ptr)))
  {
    cfree(row_ptrs);
    fclose(f);
    png_destroy_write_struct(&png_ptr,&info_ptr);

    throw_error(IMAGE_PNG_FILE_WRITE_ERROR);
  }

  // - write image data -
  png_write_image(png_ptr,row_ptrs);
  png_write_end(png_ptr,NULL);

  // - release resources -
  cfree(row_ptrs);
  fclose(f);
  png_destroy_write_struct(&png_ptr,&info_ptr);

  return 0;
}/*}}}*/

#define IMAGE_READ_JPEG_DATA(RELEASE_CODE) \
{/*{{{*/\
\
  /* - read jpeg image header - */\
  jpeg_read_header(&cinfo,TRUE);\
\
  unsigned pixel_format;\
\
  switch (cinfo.out_color_space)\
  {\
    case JCS_GRAYSCALE:\
      pixel_format = c_image_pixel_format_GRAY8;\
      break;\
    case JCS_RGB:\
      pixel_format = c_image_pixel_format_RGB24;\
      break;\
\
    /* - ERROR - */\
    case JCS_YCbCr:\
    case JCS_CMYK:\
    case JCS_YCCK:\
    default:\
      jpeg_destroy_decompress(&cinfo);\
      RELEASE_CODE;\
\
      throw_error(IMAGE_UNSUPPORTED_PIXEL_FORMAT);\
  }\
\
  /* - create image content - */\
  image_internal_s_create(this,cinfo.image_width,cinfo.image_height,pixel_format);\
\
  /* - start decompression - */\
  jpeg_start_decompress(&cinfo);\
\
  /* - read jpeg image data - */\
  unsigned char *line_ptr = this->image_data_ptr->data;\
  unsigned line_bytes = this->image_data_ptr->line_bytes;\
  while (cinfo.output_scanline < cinfo.output_height)\
  {\
    jpeg_read_scanlines(&cinfo,&line_ptr,1);\
    line_ptr += line_bytes;\
  }\
\
  jpeg_destroy_decompress(&cinfo);\
  RELEASE_CODE;\
}/*}}}*/

int image_s_read_jpeg_data(image_s *this,unsigned a_size,const char *a_data)
{/*{{{*/
  struct jpeg_decompress_struct cinfo;
  jpeg_error_mgr_s jem;

  // - initialize error manager -
  cinfo.err = jpeg_std_error(&jem.pub);
  jem.pub.error_exit = jpeg_error_mgr_s_error_exit;

  if (setjmp(jem.jump_buffer))
  {
    jpeg_destroy_decompress(&cinfo);

    throw_error(IMAGE_JPEG_DATA_READ_ERROR);
  }

  jpeg_create_decompress(&cinfo);
  jpeg_source_mgr_s_set_source(&cinfo,a_data,a_size);

  IMAGE_READ_JPEG_DATA();

  return 0;
}/*}}}*/

int image_s_read_jpeg_file(image_s *this,const char *a_path)
{/*{{{*/

  // - open source jpeg file -
  FILE *file = fopen(a_path,"rb");

  // - ERROR -
  if (file == NULL)
  {
    throw_error(IMAGE_CANNOT_OPEN_FILE);
  }

  struct jpeg_decompress_struct cinfo;
  jpeg_error_mgr_s jem;

  // - initialize error manager -
  cinfo.err = jpeg_std_error(&jem.pub);
  jem.pub.error_exit = jpeg_error_mgr_s_error_exit;

  if (setjmp(jem.jump_buffer))
  {
    jpeg_destroy_decompress(&cinfo);
    fclose(file);

    throw_error(IMAGE_JPEG_FILE_READ_ERROR);
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo,file);

  IMAGE_READ_JPEG_DATA(fclose(file));

  return 0;
}/*}}}*/

int image_s_write_jpeg_file(image_s *this,const char *a_path,int a_quality)
{/*{{{*/

  // - check jpeg image details -
  switch (this->pixel_format)
  {
  case c_image_pixel_format_GRAY8:
  case c_image_pixel_format_RGB24:
    break;

  // - ERROR -
  default:
    throw_error(IMAGE_UNSUPPORTED_PIXEL_FORMAT);
  }

  // - open target jpeg file -
  FILE *file = fopen(a_path,"wb");

  // - ERROR -
  if (file == NULL)
  {
    throw_error(IMAGE_CANNOT_OPEN_FILE);
  }

  struct jpeg_compress_struct cinfo;
  jpeg_error_mgr_s jem;

  // - initialize error manager -
  cinfo.err = jpeg_std_error(&jem.pub);
  jem.pub.error_exit = jpeg_error_mgr_s_error_exit;

  if (setjmp(jem.jump_buffer))
  {
    jpeg_destroy_compress(&cinfo);
    fclose(file);

    throw_error(IMAGE_JPEG_FILE_WRITE_ERROR);
  }

  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo,file);

  // - retrieve jpeg image details -
  J_COLOR_SPACE color_space;
  int components;

  switch (this->pixel_format)
  {
  case c_image_pixel_format_GRAY8:
    color_space = JCS_GRAYSCALE;
    components = 1;
    break;
  case c_image_pixel_format_RGB24:
    color_space = JCS_RGB;
    components = 3;
    break;

  // - ERROR -
  default:
    throw_error(IMAGE_UNSUPPORTED_PIXEL_FORMAT);
  }

  // - initialize image properties -
  cinfo.image_width = this->width;
  cinfo.image_height = this->height;
  cinfo.input_components = components;
  cinfo.in_color_space = color_space;

  // - set compression parameters -
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo,a_quality,TRUE);

  // - start compression -
  jpeg_start_compress(&cinfo,TRUE);

  // - write image data -
  unsigned char *line_ptr = this->image_data_ptr->data;
  unsigned line_bytes = this->image_data_ptr->line_bytes;
  while (cinfo.next_scanline < cinfo.image_height)
  {
    jpeg_write_scanlines(&cinfo,&line_ptr,1);
    line_ptr += line_bytes;
  }

  // - finish compression -
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  fclose(file);

  return 0;
}/*}}}*/

#define IMAGE_METHOD_BLOCK_IO(FUNCTION) \
{/*{{{*/\
\
  /* - ERROR - */\
  if (this->pixel_format != a_src->pixel_format ||\
      this->width != a_src->width ||\
      this->height != a_src->height ||\
      this->pixel_format == c_image_pixel_format_blank)\
  {\
    throw_error(IMAGE_WRONG_PROPERTIES);\
  }\
\
  /* - ERROR - */\
  if (!image_internal_s_ ## FUNCTION(this,a_src))\
  {\
    throw_error(IMAGE_OPERATION_ERROR);\
  }\
}/*}}}*/

int image_s_io_copy(image_s *this,const image_s *a_src)
{/*{{{*/
  IMAGE_METHOD_BLOCK_IO(io_copy);

  return 0;
}/*}}}*/

int image_s_io_convert(image_s *this,const image_s *a_src)
{/*{{{*/

  // - ERROR -
  if (this->width != a_src->width ||
      this->height != a_src->height ||
      this->pixel_format == c_image_pixel_format_blank ||
      a_src->pixel_format == c_image_pixel_format_blank)
  {
    throw_error(IMAGE_WRONG_PROPERTIES);
  }

  if (this->pixel_format == a_src->pixel_format)
  {
    // - ERROR -
    if (!image_internal_s_io_copy(this,a_src))
    {
      throw_error(IMAGE_OPERATION_ERROR);
    }
  }
  else
  {
    // - ERROR -
    if (!image_internal_s_io_convert(this,a_src))
    {
      throw_error(IMAGE_OPERATION_ERROR);
    }
  }

  return 0;
}/*}}}*/

int image_s_io_apply(image_s *this,const image_s *a_src)
{/*{{{*/
  IMAGE_METHOD_BLOCK_IO(io_apply);

  return 0;
}/*}}}*/

int image_s_io_normalize(image_s *this,const image_s *a_src)
{/*{{{*/
  IMAGE_METHOD_BLOCK_IO(io_normalize);

  return 0;
}/*}}}*/

