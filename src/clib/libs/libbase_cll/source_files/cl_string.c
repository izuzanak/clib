
@begin
include "cl_string.h"
@end

// === constants and definitions ===============================================

// - string terminating character -
const char c_string_terminating_char = '\0';

// === methods of structure string_s ===========================================

void string_s_set_format(string_s *this,const char *a_format,...)
{/*{{{*/
  string_s_clear(this);

  const int init_size = 128;
  va_list ap;

  this->data = (char *)cmalloc(init_size*sizeof(char));

  va_start(ap,a_format);
  int length = vsnprintf(this->data,init_size,a_format,ap);
  va_end(ap);

  this->size = length + 1;

  if (length >= init_size)
  {
    cfree(this->data);
    this->data = (char *)cmalloc(this->size*sizeof(char));

    va_start(ap,a_format);
    vsnprintf(this->data,this->size,a_format,ap);
    va_end(ap);
  }
}/*}}}*/

void string_s_to_json(const string_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_push(a_trg,'"');

  if (this->size > 1)
  {
    char *ptr = this->data;
    char *ptr_end = ptr + this->size - 1;
    do {

      switch (*ptr)
      {
        case '\b':
          bc_array_s_push(a_trg,'\\');
          bc_array_s_push(a_trg,'b');
          break;
        case '\f':
          bc_array_s_push(a_trg,'\\');
          bc_array_s_push(a_trg,'f');
          break;
        case '\n':
          bc_array_s_push(a_trg,'\\');
          bc_array_s_push(a_trg,'n');
          break;
        case '\r':
          bc_array_s_push(a_trg,'\\');
          bc_array_s_push(a_trg,'r');
          break;
        case '\t':
          bc_array_s_push(a_trg,'\\');
          bc_array_s_push(a_trg,'t');
          break;
        case '\\':
          bc_array_s_push(a_trg,'\\');
          bc_array_s_push(a_trg,'\\');
          break;
        case '"':
          bc_array_s_push(a_trg,'\\');
          bc_array_s_push(a_trg,'"');
          break;
        default:
          bc_array_s_push(a_trg,*ptr);
      }

    } while(++ptr < ptr_end);
  }

  bc_array_s_push(a_trg,'"');
}/*}}}*/

unsigned string_s_get_idx(string_s *this,unsigned a_idx,unsigned a_length,const char *a_data)
{/*{{{*/
  if (a_idx >= (this->size - 1) || a_length == 0 || a_length >= (this->size - a_idx))
  {
    return c_idx_not_exist;
  }

  // - single character search -
  if (a_length == 1)
  {
    char *s_ptr = this->data + a_idx;
    char *s_ptr_end = this->data + (this->size - a_length);
    do {
      if (*s_ptr == *a_data)
      {
        return s_ptr - this->data;
      }
    } while(++s_ptr < s_ptr_end);
  }

  // - multiple characters search -
  else
  {
    // - compute search sum -
    unsigned search_sum = 0;
    const unsigned char *ss_ptr = (const unsigned char *)a_data;
    const unsigned char *ss_ptr_end = ss_ptr + a_length;
    do {
      search_sum += *ss_ptr;
    } while(++ss_ptr < ss_ptr_end);

    // - compute text sum -
    unsigned text_sum = 0;
    unsigned char *s_ptr = (unsigned char *)this->data + a_idx;
    unsigned char *s_ptr_end = s_ptr + a_length;
    do {
      text_sum += *s_ptr;
    } while(++s_ptr < s_ptr_end);

    s_ptr = (unsigned char *)this->data + a_idx;
    s_ptr_end = (unsigned char *)this->data + (this->size - a_length);
    do {

      // - if search sum was found -
      if (text_sum == search_sum)
      {
        if (memcmp(a_data,s_ptr,a_length) == 0)
        {
          return s_ptr - (unsigned char *)this->data;
        }
      }

      // - update text sum -
      text_sum -= *s_ptr;
      text_sum += s_ptr[a_length];

    } while(++s_ptr < s_ptr_end);
  }

  return c_idx_not_exist;
}/*}}}*/

// === methods of generated structures =========================================

// -- string_array_s --
@begin
methods string_array_s
@end

// -- string_tree_s --
@begin
methods string_tree_s
@end

