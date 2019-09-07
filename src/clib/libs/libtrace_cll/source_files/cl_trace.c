
@begin
include "cl_trace.h"
@end

// === constants and definitions ===============================================

// === methods of generated structures =========================================

// -- trace_record_header_s --
@begin
methods trace_record_header_s
@end

// -- trace_record_header_tree_s --
@begin
methods trace_record_header_tree_s
@end

// === methods of structure trace_record_queue_s ===============================

void trace_record_queue_s_copy(trace_record_queue_s *this,const trace_record_queue_s *a_src)
{/*{{{*/
  debug_assert(a_src->used <= this->size && this->rec_size == a_src->rec_size);

  trace_record_queue_s_clear(this);

  if (a_src->used == 0)
  {
    return;
  }

  unsigned sec_cnt;
  char *ptr = this->data;
  char *s_ptr = a_src->data + a_src->begin*this->rec_size;
  char *s_ptr_end;

  if (a_src->begin + a_src->used > a_src->size)
  {
    s_ptr_end = a_src->data + a_src->size*this->rec_size;
    sec_cnt = a_src->begin + a_src->used - a_src->size;
  }
  else
  {
    s_ptr_end = s_ptr + a_src->used*this->rec_size;
    sec_cnt = 0;
  }

  do {
    trace_record_s_copy((trace_record_s *)ptr,(trace_record_s *)s_ptr,this->rec_size); 
  } while(ptr += this->rec_size,(s_ptr += this->rec_size) < s_ptr_end);

  if (sec_cnt != 0)
  {
    s_ptr = a_src->data;
    s_ptr_end = s_ptr + sec_cnt*this->rec_size;

    do {
      trace_record_s_copy((trace_record_s *)ptr,(trace_record_s *)s_ptr,this->rec_size);
    } while(ptr += this->rec_size,(s_ptr += this->rec_size) < s_ptr_end);
  }

  this->used = a_src->used;
}/*}}}*/

int trace_record_queue_s_compare(const trace_record_queue_s *this,const trace_record_queue_s *a_second)
{/*{{{*/
  debug_assert(this->rec_size == a_second->rec_size);

  if (this->used != a_second->used)
  {
    return 0;
  }

  if (this->used == 0)
  {
    return 1;
  }

  char *ptr = this->data + this->begin*this->rec_size;
  char *ptr_break = this->data + this->size*this->rec_size;
  char *ptr_end;
  char *s_ptr = a_second->data + a_second->begin*this->rec_size;
  char *s_ptr_break = a_second->data + a_second->size*this->rec_size;

  if (this->begin + this->used > this->size)
  {
    ptr_end = this->data + (this->begin + this->used - this->size)*this->rec_size;
  }
  else
  {
    ptr_end = ptr + this->used*this->rec_size;
  }

  do {
    if (!trace_record_s_compare((trace_record_s *)ptr,(trace_record_s *)s_ptr,this->rec_size))
    {
      return 0;
    }

    if ((ptr += this->rec_size) >= ptr_break)
    {
      ptr = this->data;
    }

    if ((s_ptr += this->rec_size) >= s_ptr_break)
    {
      s_ptr = a_second->data;
    }

  } while(ptr != ptr_end);

  return 1;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
void trace_record_queue_s___to_string(const trace_record_queue_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_push(a_trg,'[');

  if (this->used != 0)
  {
    unsigned sec_cnt;
    char *ptr = this->data + this->begin*this->rec_size;
    char *ptr_end;

    if (this->begin + this->used > this->size)
    {
      ptr_end = this->data + this->size*this->rec_size;
      sec_cnt = this->begin + this->used - this->size;
    }
    else
    {
      ptr_end = ptr + this->used*this->rec_size;
      sec_cnt = 0;
    }

    do {
      trace_record_s_to_string((trace_record_s *)ptr,a_trg);

      if ((ptr += this->rec_size) >= ptr_end)
      {
        break;
      }

      bc_array_s_push(a_trg,',');
    } while(1);

    if (sec_cnt != 0)
    {
      ptr = this->data;
      ptr_end = ptr + sec_cnt*this->rec_size;

      do {
        bc_array_s_push(a_trg,',');
        trace_record_s_to_string((trace_record_s *)ptr,a_trg);
      } while((ptr += this->rec_size) < ptr_end);
    }
  }

  bc_array_s_push(a_trg,']');
}/*}}}*/
#endif

#if OPTION_TO_STRING == ENABLED
void trace_record_queue_s_to_string_separator(const trace_record_queue_s *this,bc_array_s *a_trg,unsigned a_count,const char *a_data)
{/*{{{*/
  if (this->used != 0)
  {
    unsigned sec_cnt;
    char *ptr = this->data + this->begin*this->rec_size;
    char *ptr_end;

    if (this->begin + this->used > this->size)
    {
      ptr_end = this->data + this->size*this->rec_size;
      sec_cnt = this->begin + this->used - this->size;
    }
    else
    {
      ptr_end = ptr + this->used*this->rec_size;
      sec_cnt = 0;
    }

    do {
      trace_record_s_to_string((trace_record_s *)ptr,a_trg);

      if ((ptr += this->rec_size) >= ptr_end)
      {
        break;
      }

      bc_array_s_append(a_trg,a_count,a_data);
    } while(1);

    if (sec_cnt != 0)
    {
      ptr = this->data;
      ptr_end = ptr + sec_cnt*this->rec_size;

      do {
        bc_array_s_append(a_trg,a_count,a_data);
        trace_record_s_to_string((trace_record_s *)ptr,a_trg);
      } while((ptr += this->rec_size) < ptr_end);
    }
  }
}/*}}}*/
#endif

// -- trace_s --
@begin
methods trace_s
@end

int trace_s_create(trace_s *this,
    void *header_data,ulli header_size,
    void *trace_data,ulli trace_size,unsigned a_rec_size)
{/*{{{*/
  trace_s_clear(this);

  //unsigned header_rec_size = sizeof(trace_record_s) + sizeof(trace_record_queue_header_s);
  //unsigned header_rec_cnt = header_size/header_rec_size;
  //trace_record_queue_s_set_buffer(&this->header_queue,header_rec_cnt,header_data);

  // FIXME TODO:
  //  - scan header queue for largest rec id
  //  - set header queue header:
  //    - used = max
  //    - begin = after rec. with largest id
  //    - ...
  //  - update trace queue header from header queue rec.
  //    - if not header queue rec found:
  //      - reset storage header

  return 0;
}/*}}}*/

