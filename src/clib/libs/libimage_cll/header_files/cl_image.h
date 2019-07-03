
#ifndef __CL_IMAGE_H
#define __CL_IMAGE_H

@begin
include "cl_struct.h"
include "cl_atomic.h"
include "cl_image_internal.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libimage_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libimage_cll_EXPORTS
#define libimage_cll_EXPORT __declspec(dllexport)
#else
#define libimage_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_IMAGE_WRONG_PROPERTIES 1
#define ERROR_IMAGE_CREATE_ERROR 2
#define ERROR_IMAGE_INVALID_SOURCE_DATA_SIZE 3
#define ERROR_IMAGE_UNSUPPORTED_PIXEL_FORMAT 4
#define ERROR_IMAGE_CANNOT_OPEN_FILE 5
#define ERROR_IMAGE_FILE_IS_NOT_PNG_FILE 6
#define ERROR_IMAGE_PNG_READ_INIT_ERROR 7
#define ERROR_IMAGE_PNG_FILE_READ_ERROR 8
#define ERROR_IMAGE_PNG_DATA_READ_ERROR 9
#define ERROR_IMAGE_PNG_WRITE_INIT_ERROR 10
#define ERROR_IMAGE_PNG_FILE_WRITE_ERROR 11
#define ERROR_IMAGE_JPEG_FILE_READ_ERROR 12
#define ERROR_IMAGE_JPEG_FILE_WRITE_ERROR 13
#define ERROR_IMAGE_JPEG_DATA_READ_ERROR 14
#define ERROR_IMAGE_OPERATION_ERROR 15

// === definition of structure image_s =========================================

typedef struct image_internal_s image_s;

@begin
define image_s dynamic
@end

static inline void image_s_init(image_s *this);
static inline void image_s_clear(image_s *this);
static inline void image_s_flush_all(image_s *this);
static inline void image_s_swap(image_s *this,image_s *a_second);
static inline void image_s_copy(const image_s *this,const image_s *a_src);
static inline int image_s_compare(const image_s *this,const image_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void image_s_to_string(const image_s *this,bc_array_s *a_trg);
#endif

WUR libimage_cll_EXPORT int image_s_create(image_s *this,
    unsigned a_width,unsigned a_height,unsigned a_format);
WUR libimage_cll_EXPORT int image_s_create_data(image_s *this,
    unsigned a_width,unsigned a_height,unsigned a_format,unsigned a_size,const char *a_data);
WUR static inline int image_s_create_referred(image_s *this,
    unsigned a_x_pos,unsigned a_y_pos,unsigned a_width,unsigned a_height,image_s *a_src);
WUR libimage_cll_EXPORT int image_s_read_png_data(image_s *this,unsigned a_size,const char *a_data);
WUR libimage_cll_EXPORT int image_s_read_png_file(image_s *this,const char *a_path);
WUR libimage_cll_EXPORT int image_s_write_png_file(image_s *this,const char *a_path);
WUR libimage_cll_EXPORT int image_s_read_jpeg_data(image_s *this,unsigned a_size,const char *a_data);
WUR libimage_cll_EXPORT int image_s_read_jpeg_file(image_s *this,const char *a_path);
WUR libimage_cll_EXPORT int image_s_write_jpeg_file(image_s *this,const char *a_path,int a_quality);
WUR static inline int image_s_duplicate(image_s *this,const image_s *a_src);
WUR static inline int image_s_io_clear(image_s *this);
WUR static inline int image_s_io_fill(image_s *this,const unsigned char *a_color);
WUR libimage_cll_EXPORT int image_s_io_copy(image_s *this,const image_s *a_src);
WUR libimage_cll_EXPORT int image_s_io_convert(image_s *this,const image_s *a_src);
WUR libimage_cll_EXPORT int image_s_io_apply(image_s *this,const image_s *a_src);
WUR libimage_cll_EXPORT int image_s_io_normalize(image_s *this,const image_s *a_src);

// === inline methods of structure image_s =====================================

static inline void image_s_init(image_s *this)
{/*{{{*/
  image_internal_s_init(this);
}/*}}}*/

static inline void image_s_clear(image_s *this)
{/*{{{*/
  image_internal_s_clear(this);
}/*}}}*/

static inline void image_s_flush_all(image_s *this)
{/*{{{*/
}/*}}}*/

static inline void image_s_swap(image_s *this,image_s *a_second)
{/*{{{*/
  image_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void image_s_copy(const image_s *this,const image_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int image_s_compare(const image_s *this,const image_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void image_s_to_string(const image_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"image_s{%p}",this);
}/*}}}*/
#endif

static inline int image_s_create_referred(image_s *this,
    unsigned a_x_pos,unsigned a_y_pos,unsigned a_width,unsigned a_height,image_s *a_src)
{/*{{{*/

  // - ERROR -
  if (a_width <= 0 || a_height <= 0 || a_x_pos + a_width > a_src->width || a_y_pos + a_height > a_src->height)
  {
    throw_error(IMAGE_WRONG_PROPERTIES);
  }

  image_internal_s_create_referred(this,a_x_pos,a_y_pos,a_width,a_height,a_src);

  return 0;
}/*}}}*/

static inline int image_s_duplicate(image_s *this,const image_s *a_src)
{/*{{{*/
  image_internal_s_copy(this,a_src);

  return 0;
}/*}}}*/

static inline int image_s_io_clear(image_s *this)
{/*{{{*/

  // - ERROR -
  if (!image_internal_s_io_clear(this))
  {
    throw_error(IMAGE_OPERATION_ERROR);
  }

  return 0;
}/*}}}*/

static inline int image_s_io_fill(image_s *this,const unsigned char *a_color)
{/*{{{*/

  // - ERROR -
  if (!image_internal_s_io_fill(this,a_color))
  {
    throw_error(IMAGE_OPERATION_ERROR);
  }

  return 0;
}/*}}}*/

#endif

