
#ifndef __CL_IMAGE_INTERNAL_H
#define __CL_IMAGE_INTERNAL_H

@begin
include "cl_struct.h"
include "cl_atomic.h"
@end

#include "png.h"
#include "jpeglib.h"

// - pixel formats -
#define c_pixel_format_cnt  5

enum
{
  c_image_pixel_format_GRAY8 = 0,
  c_image_pixel_format_RGB24,
  c_image_pixel_format_RGBA,
  c_image_pixel_format_DOUBLE,
  c_image_pixel_format_COMPLEX,

  c_image_pixel_format_blank,
};

// - sizes of pixels by format in bytes -
extern unsigned c_bigest_pixel_size;
extern unsigned c_pixel_sizes[c_pixel_format_cnt];
extern unsigned c_pixel_values_cnt[c_pixel_format_cnt];

/*
 * definition of structure image_internal_s
 */

typedef struct image_internal_data_s
{
  unsigned line_bytes;
  atomic_s reference_cnt;
  unsigned char *data;
} image_internal_data_s;

typedef struct image_internal_s
{
  int root;
  unsigned pixel_format;
  unsigned width;
  unsigned height;
  unsigned x_pos;
  unsigned y_pos;
  unsigned pixel_step;
  image_internal_data_s *image_data_ptr;
} image_internal_s;

static inline void image_internal_s_init(image_internal_s *this);
static inline void image_internal_s_clear(image_internal_s *this);
static inline void image_internal_s_flush_all(image_internal_s *this) {}
static inline void image_internal_s_swap(image_internal_s *this,image_internal_s *a_second);
static inline void image_internal_s_copy(image_internal_s *this,const image_internal_s *a_src);
int image_internal_s_compare(const image_internal_s *this,const image_internal_s *a_second);

int image_internal_s_create_header(image_internal_s *this,
    unsigned a_width,unsigned a_height,unsigned a_pixel_format);
static inline int image_internal_s_create(image_internal_s *this,
    unsigned a_width,unsigned a_height,unsigned a_pixel_format);
static inline int image_internal_s_create_from_image(image_internal_s *this,
    const image_internal_s *a_img);
int image_internal_s_create_referred(image_internal_s *this,
    unsigned a_x_pos,unsigned a_y_pos,unsigned a_width,unsigned a_height,image_internal_s *a_src);

int image_internal_s_io_clear(image_internal_s *this);
int image_internal_s_io_fill(image_internal_s *this,const unsigned char *a_color);
int image_internal_s_io_copy(image_internal_s *this,const image_internal_s *a_src);
int image_internal_s_io_convert(image_internal_s *this,const image_internal_s *a_src);
int image_internal_s_io_apply(image_internal_s *this,const image_internal_s *a_src);
int image_internal_s_io_normalize(image_internal_s *this,const image_internal_s *a_src);

/*
 * definition of structure png_data_s
 */

typedef struct png_data_s
{
  const char *data;
  unsigned size;
  unsigned read;
} png_data_s;

void png_data_s_read_data(png_structp a_png_ptr,png_bytep a_data,png_size_t a_length);

/*
 * definition of structure jpeg_error_mgr_s
 */

typedef struct jpeg_error_mgr_s
{
  struct jpeg_error_mgr pub;
  jmp_buf jump_buffer;
} jpeg_error_mgr_s;

void jpeg_error_mgr_s_error_exit(j_common_ptr cinfo);

/*
 * definition of structure jpeg_source_mgr_s
 */

extern JOCTET EOI_BYTE;

typedef struct jpeg_source_mgr_s
{
  struct jpeg_source_mgr pub;
  const JOCTET *data;
  unsigned length;
} jpeg_source_mgr_s;

void jpeg_source_mgr_s_init_source(j_decompress_ptr cinfo);
boolean jpeg_source_mgr_s_fill_buffer(j_decompress_ptr cinfo);
void jpeg_source_mgr_s_skip_data(j_decompress_ptr cinfo,long num_bytes);
void jpeg_source_mgr_s_term_source(j_decompress_ptr cinfo);
void jpeg_source_mgr_s_set_source(j_decompress_ptr cinfo,const char *data,unsigned length);

/*
 * inline methods of structure image_internal_s
 */

inline void image_internal_s_init(image_internal_s *this)
{/*{{{*/
  this->pixel_format = c_image_pixel_format_blank;
}/*}}}*/

inline void image_internal_s_clear(image_internal_s *this)
{/*{{{*/
  if (this->pixel_format != c_image_pixel_format_blank)
  {
    if (atomic_s_dec_and_test(&this->image_data_ptr->reference_cnt))
    {
      cfree(this->image_data_ptr->data);
      cfree(this->image_data_ptr);
    }
  }

  image_internal_s_init(this);
}/*}}}*/

inline void image_internal_s_swap(image_internal_s *this,image_internal_s *a_second)
{/*{{{*/
  image_internal_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

inline void image_internal_s_copy(image_internal_s *this,const image_internal_s *a_src)
{/*{{{*/
  if (a_src->pixel_format == c_image_pixel_format_blank)
  {
    image_internal_s_clear(this);
  }
  else
  {
    image_internal_s_create(this,a_src->width,a_src->height,a_src->pixel_format);
    image_internal_s_io_copy(this,a_src);
  }
}/*}}}*/

inline int image_internal_s_create(image_internal_s *this,
    unsigned a_width,unsigned a_height,unsigned a_pixel_format)
{/*{{{*/
  if (!image_internal_s_create_header(this,a_width,a_height,a_pixel_format))
  {
    return 0;
  }

  this->image_data_ptr->data = (unsigned char *)cmalloc(this->height*this->image_data_ptr->line_bytes);
  memset(this->image_data_ptr->data,0,this->height*this->image_data_ptr->line_bytes);

  return 1;
}/*}}}*/

inline int image_internal_s_create_from_image(image_internal_s *this,
    const image_internal_s *a_img)
{/*{{{*/
  return image_internal_s_create(this,a_img->width,a_img->height,a_img->pixel_format);
}/*}}}*/

#endif

