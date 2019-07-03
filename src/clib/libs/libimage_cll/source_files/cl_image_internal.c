
@begin
include "cl_image_internal.h"
@end

// - sizes of pixels by format in bytes -
unsigned c_bigest_pixel_size = 2*sizeof(double);
unsigned c_pixel_sizes[c_pixel_format_cnt] =
{
  sizeof(unsigned char),
  3*sizeof(unsigned char),
  4*sizeof(unsigned char),
  sizeof(double),
  2*sizeof(double),
};

unsigned c_pixel_values_cnt[c_pixel_format_cnt] =
{
  1,
  3,
  4,
  1,
  2,
};

/*
 * methods of structure image_internal_s
 */

int image_internal_s_compare(const image_internal_s *this,const image_internal_s *a_second)
{/*{{{*/
  if (this->pixel_format != a_second->pixel_format)
  {
    return 0;
  }

  if (this->pixel_format == c_image_pixel_format_blank)
  {
    return 1;
  }

  if (this->width != a_second->width || this->height != a_second->height)
  {
    return 0;
  }

  unsigned line_size = this->image_data_ptr->line_bytes;
  unsigned s_line_size = a_second->image_data_ptr->line_bytes;
  unsigned image_ls = this->width*this->pixel_step;

  unsigned char *ptr = this->image_data_ptr->data + this->y_pos*line_size + this->x_pos*this->pixel_step;
  unsigned char *ptr_end = ptr + (this->height - 1)*line_size + this->width*this->pixel_step;
  unsigned char *s_ptr = a_second->image_data_ptr->data + a_second->y_pos*s_line_size + a_second->x_pos*this->pixel_step;

  do {
    if (memcmp(ptr,s_ptr,image_ls) != 0) {
      return 0;
    }

    ptr += line_size;
    s_ptr += s_line_size;
  } while(ptr < ptr_end);

  return 1;
}/*}}}*/

int image_internal_s_create_header(image_internal_s *this,
    unsigned a_width,unsigned a_height,unsigned a_pixel_format)
{/*{{{*/
  image_internal_s_clear(this);

  if (a_width == 0 || a_height == 0 || a_pixel_format == c_image_pixel_format_blank)
  {
    return 0;
  }

  this->root = 1;
  this->pixel_format = a_pixel_format;
  this->width = a_width;
  this->height = a_height;
  this->x_pos = 0;
  this->y_pos = 0;
  this->pixel_step = c_pixel_sizes[this->pixel_format];

  // - compute line bytes count -
  unsigned tmp_line_bytes = this->width*this->pixel_step;
  if (tmp_line_bytes & (sizeof(int) - 1))
  {
    tmp_line_bytes = (tmp_line_bytes & ~(sizeof(int) - 1)) + sizeof(int);
  }

  // - creation of image data -
  this->image_data_ptr = (image_internal_data_s *)cmalloc(sizeof(image_internal_data_s));
  atomic_s_set(&this->image_data_ptr->reference_cnt,1);
  this->image_data_ptr->line_bytes = tmp_line_bytes;
  this->image_data_ptr->data = NULL;

  return 1;
}/*}}}*/

int image_internal_s_create_referred(image_internal_s *this,
    unsigned a_x_pos,unsigned a_y_pos,unsigned a_width,unsigned a_height,image_internal_s *a_src)
{/*{{{*/
  image_internal_s_clear(this);

  if (a_src->pixel_format == c_image_pixel_format_blank || a_width == 0 || a_height == 0 ||
      a_x_pos + a_width > a_src->width || a_y_pos + a_height > a_src->height)
  {
    return 0;
  }

  this->root = 0;
  this->pixel_format = a_src->pixel_format;
  this->width = a_width;
  this->height = a_height;
  this->x_pos = a_src->x_pos + a_x_pos;
  this->y_pos = a_src->y_pos + a_y_pos;
  this->pixel_step = a_src->pixel_step;

  this->image_data_ptr = a_src->image_data_ptr;
  atomic_s_inc(&this->image_data_ptr->reference_cnt);

  return 1;
}/*}}}*/

int image_internal_s_io_clear(image_internal_s *this)
{/*{{{*/
  if (this->pixel_format == c_image_pixel_format_blank)
  {
    return 0;
  }

  unsigned line_size = this->image_data_ptr->line_bytes;
  unsigned image_ls = this->width*this->pixel_step;

  unsigned char *ptr = this->image_data_ptr->data + this->y_pos*line_size + this->x_pos*this->pixel_step;
  unsigned char *ptr_end = ptr + (this->height - 1)*line_size + this->width*this->pixel_step;

  do {
    memset(ptr,0,image_ls);

    ptr += line_size;
  } while(ptr < ptr_end);

  return 1;
}/*}}}*/

int image_internal_s_io_fill(image_internal_s *this,const unsigned char *a_color)
{/*{{{*/
  if (this->pixel_format == c_image_pixel_format_blank)
  {
    return 0;
  }

  unsigned line_size = this->image_data_ptr->line_bytes;
  unsigned image_ls = this->width*this->pixel_step;

  unsigned char *ptr = this->image_data_ptr->data + this->y_pos*line_size + this->x_pos*this->pixel_step;
  unsigned char *ptr_end = ptr + (this->height - 1)*line_size + this->width*this->pixel_step;

#define IMAGE_FILL(OPERATION) \
  {/*{{{*/\
    do {\
      unsigned char *ptr_w_end = ptr + image_ls;\
      do {\
        OPERATION;\
      } while((ptr += this->pixel_step) < ptr_w_end);\
\
      ptr += line_size - image_ls;\
    } while(ptr < ptr_end);\
  }/*}}}*/

  switch (this->pixel_format)
  {
    case c_image_pixel_format_GRAY8:
      IMAGE_FILL(
          *ptr = *a_color;
          );
      break;
    case c_image_pixel_format_RGB24:
      IMAGE_FILL(
          ptr[0] = a_color[0];
          ptr[1] = a_color[1];
          ptr[2] = a_color[2];
          );
      break;
    case c_image_pixel_format_RGBA:
      IMAGE_FILL(
          ptr[0] = a_color[0];
          ptr[1] = a_color[1];
          ptr[2] = a_color[2];
          ptr[3] = a_color[3];
          );
      break;
    default:
      return 0;
  }

  return 1;
}/*}}}*/

int image_internal_s_io_copy(image_internal_s *this,const image_internal_s *a_src)
{/*{{{*/
  if (this->pixel_format != a_src->pixel_format || this->width != a_src->width || this->height != a_src->height ||
      this->pixel_format == c_image_pixel_format_blank)
  {
    return 0;
  }

  unsigned line_size = this->image_data_ptr->line_bytes;
  unsigned s_line_size = a_src->image_data_ptr->line_bytes;
  unsigned image_ls = this->width*this->pixel_step;

  unsigned char *ptr = this->image_data_ptr->data + this->y_pos*line_size + this->x_pos*this->pixel_step;
  unsigned char *ptr_end = ptr + (this->height - 1)*line_size + this->width*this->pixel_step;
  unsigned char *s_ptr = a_src->image_data_ptr->data + a_src->y_pos*s_line_size + a_src->x_pos*this->pixel_step;

  do {
    memcpy(ptr,s_ptr,image_ls);

    ptr += line_size;
    s_ptr += s_line_size;
  } while(ptr < ptr_end);

  return 1;
}/*}}}*/

int image_internal_s_io_convert(image_internal_s *this,const image_internal_s *a_src)
{/*{{{*/
  if (this->width != a_src->width || this->height != a_src->height ||
      this->pixel_format == c_image_pixel_format_blank || a_src->pixel_format == c_image_pixel_format_blank)
  {
    return 0;
  }

  unsigned line_size = this->image_data_ptr->line_bytes;
  unsigned s_line_size = a_src->image_data_ptr->line_bytes;
  unsigned image_ls = this->width*this->pixel_step;
  unsigned s_image_ls = this->width*a_src->pixel_step;

  unsigned char *ptr = this->image_data_ptr->data + this->y_pos*line_size + this->x_pos*this->pixel_step;
  unsigned char *ptr_end = ptr + (this->height - 1)*line_size + this->width*this->pixel_step;
  unsigned char *s_ptr = a_src->image_data_ptr->data + a_src->y_pos*s_line_size + a_src->x_pos*a_src->pixel_step;

#define IMAGE_CONVERT(OPERATION) \
  {/*{{{*/\
    do {\
      unsigned char *ptr_w_end = ptr + image_ls;\
      do {\
        OPERATION;\
      } while(s_ptr += a_src->pixel_step,(ptr += this->pixel_step) < ptr_w_end);\
\
      ptr += line_size - image_ls;\
      s_ptr += s_line_size - s_image_ls;\
    } while(ptr < ptr_end);\
  }/*}}}*/

  switch (this->pixel_format)
  {
  case c_image_pixel_format_GRAY8:
    switch (a_src->pixel_format)
    {
    case c_image_pixel_format_RGB24:
    case c_image_pixel_format_RGBA:
      IMAGE_CONVERT(
          ptr[0] = (5014710U*s_ptr[0] + 9848226U*s_ptr[1] + 1914280U*s_ptr[2]) >> 24;
          );
      break;
    case c_image_pixel_format_DOUBLE:
      IMAGE_CONVERT(
          ptr[0] = *((double *)s_ptr)*255.0;
          );
      break;
    case c_image_pixel_format_COMPLEX:
      IMAGE_CONVERT(
          ptr[0] = ((double *)s_ptr)[0]*255.0;
          );
      break;
    default:
      return 0;
    }
    break;
  case c_image_pixel_format_RGB24:
    switch (a_src->pixel_format)
    {
    case c_image_pixel_format_GRAY8:
      IMAGE_CONVERT(
          ptr[0] =
          ptr[1] =
          ptr[2] = s_ptr[0];
          );
      break;
    case c_image_pixel_format_RGBA:
      IMAGE_CONVERT(
          ptr[0] = s_ptr[0];
          ptr[1] = s_ptr[1];
          ptr[2] = s_ptr[2];
          );
      break;
    default:
      return 0;
    }
    break;
  case c_image_pixel_format_RGBA:
    switch (a_src->pixel_format)
    {
    case c_image_pixel_format_GRAY8:
      IMAGE_CONVERT(
          ptr[0] =
          ptr[1] =
          ptr[2] = s_ptr[0];
          ptr[3] = 255;
          );
      break;
    case c_image_pixel_format_RGB24:
      IMAGE_CONVERT(
          ptr[0] = s_ptr[0];
          ptr[1] = s_ptr[1];
          ptr[2] = s_ptr[2];
          ptr[3] = 255;
          );
      break;
    default:
      return 0;
    }
    break;
  case c_image_pixel_format_DOUBLE:
    switch (a_src->pixel_format)
    {
    case c_image_pixel_format_GRAY8:
      IMAGE_CONVERT(
          *((double *)ptr) = s_ptr[0]/255.0;
          );
      break;
    case c_image_pixel_format_COMPLEX:
      IMAGE_CONVERT(
          double real = ((double *)s_ptr)[0];
          double imag = ((double *)s_ptr)[1];

          *((double *)ptr) = sqrt(real*real + imag*imag);
          );
      break;
    default:
      return 0;
    }
    break;
  case c_image_pixel_format_COMPLEX:
    switch (a_src->pixel_format)
    {
    case c_image_pixel_format_GRAY8:
      IMAGE_CONVERT(
          ((double *)ptr)[0] = s_ptr[0]/255.0;
          ((double *)ptr)[1] = 0.0;
          );
      break;
    default:
      return 0;
    }
    break;
  default:
    return 0;
  }

  return 1;
}/*}}}*/

int image_internal_s_io_apply(image_internal_s *this,const image_internal_s *a_src)
{/*{{{*/
  if (this->pixel_format != a_src->pixel_format || this->width != a_src->width || this->height != a_src->height ||
      this->pixel_format == c_image_pixel_format_blank)
  {
    return 0;
  }

  unsigned line_size = this->image_data_ptr->line_bytes;
  unsigned s_line_size = a_src->image_data_ptr->line_bytes;
  unsigned image_ls = this->width*this->pixel_step;

  unsigned char *ptr = this->image_data_ptr->data + this->y_pos*line_size + this->x_pos*this->pixel_step;
  unsigned char *ptr_end = ptr + (this->height - 1)*line_size + this->width*this->pixel_step;
  unsigned char *s_ptr = a_src->image_data_ptr->data + a_src->y_pos*s_line_size + a_src->x_pos*this->pixel_step;

#define IMAGE_APPLY(OPERATION) \
  {/*{{{*/\
    do {\
      unsigned char *ptr_w_end = ptr + image_ls;\
      do {\
        OPERATION;\
      } while(s_ptr += this->pixel_step,(ptr += this->pixel_step) < ptr_w_end);\
\
      ptr += line_size - image_ls;\
      s_ptr += s_line_size - image_ls;\
    } while(ptr < ptr_end);\
  }/*}}}*/

  switch (this->pixel_format)
  {
  case c_image_pixel_format_RGBA:
    IMAGE_APPLY(
        unsigned alpha = s_ptr[3];
        unsigned r_alpha = 255 - alpha;

        ptr[0] = (alpha*s_ptr[0] + r_alpha*ptr[0])/255;
        ptr[1] = (alpha*s_ptr[1] + r_alpha*ptr[1])/255;
        ptr[2] = (alpha*s_ptr[2] + r_alpha*ptr[2])/255;
        );
    break;
  default:
    return 0;
  }

  return 1;
}/*}}}*/

int image_internal_s_io_normalize(image_internal_s *this,const image_internal_s *a_src)
{/*{{{*/
  if (this->pixel_format != a_src->pixel_format || this->width != a_src->width || this->height != a_src->height ||
      this->pixel_format == c_image_pixel_format_blank)
  {
    return 0;
  }

  unsigned line_size = this->image_data_ptr->line_bytes;
  unsigned s_line_size = a_src->image_data_ptr->line_bytes;
  unsigned image_ls = this->width*this->pixel_step;

#define IMAGE_NORMALIZE_GET_MIN_MAX(OPERATION) \
  {/*{{{*/\
    unsigned char *s_ptr = a_src->image_data_ptr->data + a_src->y_pos*s_line_size + a_src->x_pos*a_src->pixel_step;\
    unsigned char *s_ptr_end = s_ptr + (a_src->height - 1)*s_line_size + a_src->width*a_src->pixel_step;\
\
    do {\
      unsigned char *s_ptr_w_end = s_ptr + image_ls;\
      do {\
        OPERATION;\
      } while((s_ptr += a_src->pixel_step) < s_ptr_w_end);\
\
      s_ptr += s_line_size - image_ls;\
    } while(s_ptr < s_ptr_end);\
  }/*}}}*/

#define IMAGE_NORMALIZE_APPLY(OPERATION) \
  {/*{{{*/\
    unsigned char *ptr = this->image_data_ptr->data + this->y_pos*line_size + this->x_pos*this->pixel_step;\
    unsigned char *ptr_end = ptr + (this->height - 1)*line_size + this->width*this->pixel_step;\
\
    do {\
      unsigned char *ptr_w_end = ptr + image_ls;\
      do {\
        OPERATION;\
      } while((ptr += this->pixel_step) < ptr_w_end);\
\
      ptr += line_size - image_ls;\
    } while(ptr < ptr_end);\
  }/*}}}*/

  switch (this->pixel_format)
  {
  case c_image_pixel_format_DOUBLE:
    {/*{{{*/
      double min = INFINITY;
      double max = -INFINITY;

      IMAGE_NORMALIZE_GET_MIN_MAX(
          double value = ((double *)s_ptr)[0];

          if (value < min) min = value;
          if (value > max) max = value;
          );

      if (max > min)
      {
        double div = max - min;

        IMAGE_NORMALIZE_APPLY(
            ((double *)ptr)[0] = (((double *)ptr)[0] - min)/div;
            );
      }
    }/*}}}*/
    break;
  default:
    return 0;
  }

  return 1;
}/*}}}*/

/*
 * methods of structure png_data_s
 */

void png_data_s_read_data(png_structp a_png_ptr,png_bytep a_data,png_size_t a_length)
{/*{{{*/

  // - retrieve png data structure pointer -
  png_data_s *pd_ptr = (png_data_s *)png_get_io_ptr(a_png_ptr);

  // - check requested data size -
  if (pd_ptr->read + a_length > pd_ptr->size)
  {
    png_error(a_png_ptr,"EOF");
    return;
  }

  // - copy data to target buffer -
  memcpy(a_data,pd_ptr->data + pd_ptr->read,a_length);
  pd_ptr->read += a_length;
}/*}}}*/

/*
 * methods of structure jpeg_error_mgr_s
 */

void jpeg_error_mgr_s_error_exit(j_common_ptr cinfo)
{/*{{{*/
  longjmp(((jpeg_error_mgr_s *)cinfo->err)->jump_buffer,1);
}/*}}}*/

/*
 * static members of structure jpeg_source_mgr_s
 */

JOCTET EOI_BYTE = JPEG_EOI;

/*
 * methods of structure jpeg_source_mgr_s
 */

void jpeg_source_mgr_s_init_source(j_decompress_ptr cinfo)
{/*{{{*/
}/*}}}*/

boolean jpeg_source_mgr_s_fill_buffer(j_decompress_ptr cinfo)
{/*{{{*/
  jpeg_source_mgr_s *src = (jpeg_source_mgr_s *)cinfo->src;

  src->pub.next_input_byte = &EOI_BYTE;
  src->pub.bytes_in_buffer = 1;

  return TRUE;
}/*}}}*/

void jpeg_source_mgr_s_skip_data(j_decompress_ptr cinfo,long num_bytes)
{/*{{{*/
  jpeg_source_mgr_s *src = (jpeg_source_mgr_s *)cinfo->src;

  if (src->pub.bytes_in_buffer < (size_t)num_bytes)
  {
    src->pub.next_input_byte = &EOI_BYTE;
    src->pub.bytes_in_buffer = 1;
  }
  else
  {
    src->pub.next_input_byte += num_bytes;
    src->pub.bytes_in_buffer -= num_bytes;
  }
}/*}}}*/

void jpeg_source_mgr_s_term_source(j_decompress_ptr cinfo)
{/*{{{*/
}/*}}}*/

void jpeg_source_mgr_s_set_source(j_decompress_ptr cinfo,const char *data,unsigned length)
{/*{{{*/
  cinfo->src = (struct jpeg_source_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr)cinfo,JPOOL_PERMANENT,sizeof(jpeg_source_mgr_s));
  jpeg_source_mgr_s *src = (jpeg_source_mgr_s *)cinfo->src;

  src->pub.init_source       = jpeg_source_mgr_s_init_source;
  src->pub.fill_input_buffer = jpeg_source_mgr_s_fill_buffer;
  src->pub.skip_input_data   = jpeg_source_mgr_s_skip_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart;
  src->pub.term_source       = jpeg_source_mgr_s_term_source;

  src->data = (const JOCTET *)data;
  src->length = length;
  src->pub.bytes_in_buffer = length;
  src->pub.next_input_byte = src->data;
}/*}}}*/

