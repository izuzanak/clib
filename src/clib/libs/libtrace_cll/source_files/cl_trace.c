
@begin
include "cl_trace.h"
@end

// === constants and definitions ===============================================

unsigned g_page_size;

// === methods of generated structures =========================================

// -- trace_record_header_s --
@begin
methods trace_record_header_s
@end

// -- trace_record_header_tree_s --
@begin
methods trace_record_header_tree_s
@end

// === methods of structure trace_queue_s ======================================

void trace_queue_s_copy(trace_queue_s *this,const trace_queue_s *a_src)
{/*{{{*/
  debug_assert(a_src->used <= this->size && this->rec_size == a_src->rec_size);

  trace_queue_s_clear(this);

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

int trace_queue_s_compare(const trace_queue_s *this,const trace_queue_s *a_second)
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
void trace_queue_s___to_string(const trace_queue_s *this,bc_array_s *a_trg)
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
void trace_queue_s_to_string_separator(const trace_queue_s *this,bc_array_s *a_trg,unsigned a_count,const char *a_data)
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

unsigned trace_queue_s_scan_for_last(const trace_queue_s *this)
{/*{{{*/
  lli max_id = -1;
  unsigned last_id_idx = c_idx_not_exist;

  // - scan queue for latest record identifier -
  unsigned idx = 0;
  do {
    const trace_record_s *record = trace_queue_s_at(this,idx);

    // - record crc is valid -
    if (trace_record_s_compute_crc(record,this->rec_size) == record->header.crc)
    {
      if (record->header.id > max_id)
      {
        max_id = record->header.id;
        last_id_idx = idx;
      }
    }
  } while(++idx < this->used);

  return last_id_idx;
}/*}}}*/

// -- trace_queue_header_s --
@begin
methods trace_queue_header_s
@end

// -- trace_s --
@begin
methods trace_s
@end

int trace_s_create(trace_s *this,
    void *header_data,ulli header_size,
    void *trace_data,ulli trace_size,unsigned a_data_size)
{/*{{{*/
  trace_s_clear(this);

  // - store data size -
  this->data_size = a_data_size;

  unsigned header_rec_size = sizeof(trace_record_s) + sizeof(trace_queue_header_s);

  // - ERROR -
  if (header_size < (header_rec_size << 1))
  {
    throw_error(TRACE_HEADER_QUEUE_SIZE_ERROR);
  }

  unsigned header_rec_cnt = header_size/header_rec_size;
  trace_queue_s_set_buffer(&this->header_queue,header_rec_cnt,header_rec_size,header_data);
  this->header_queue.used = this->header_queue.size;

  // - scan queue for latest record index -
  unsigned last_header_idx = trace_queue_s_scan_for_last(&this->header_queue);

  trace_queue_header_s trqh = {
    .used = 0,
    .begin = 0
  };

  // - trace header record was found -
  if (last_header_idx != c_idx_not_exist)
  {
    // - retrieve record pointer before change of queue begin -
    trace_record_s *header_record = trace_queue_s_at(&this->header_queue,last_header_idx);

    // - update begin of header queue -
    this->header_queue.begin = (last_header_idx + 1) % this->header_queue.size;

    // - store last header id -
    this->header_last_id = header_record->header.id;

    // - retrieve trace queue header -
    trqh = *((trace_queue_header_s *)header_record->data);
  }
  else
  {
    // - reset last header id -
    this->header_last_id = -1;

    // - reset last trace id -
    this->trace_last_id = -1;
  }

  unsigned trace_rec_size = sizeof(trace_record_s) + this->data_size;

  // - ERROR -
  if (trace_size < (trace_rec_size << 1))
  {
    throw_error(TRACE_TRACE_QUEUE_SIZE_ERROR);
  }

  unsigned trace_rec_cnt = trace_size/trace_rec_size;
  trace_queue_s_set_buffer(&this->trace_queue,trace_rec_cnt,trace_rec_size,trace_data);

  this->trace_queue.used = trqh.used;
  this->trace_queue.begin = trqh.begin;

  // - retrieve last trace id -
  if (this->trace_queue.used != 0)
  {
    trace_record_s *trace_record = trace_queue_s_last(&this->trace_queue);

    // - ERROR -
    if (trace_record_s_compute_crc(trace_record,this->trace_queue.rec_size) != trace_record->header.crc)
    {
      throw_error(TRACE_INVALID_RECORD_CRC);
    }

    // - store last trace id -
    this->trace_last_id = trace_record->header.id;
  }

  return 0;
}/*}}}*/

int trace_s_write_header(trace_s *this,time_s a_time)
{/*{{{*/
  if (this->header_queue.used >= this->header_queue.size)
  {
    // - drop tail record -
    trace_queue_s_next(&this->header_queue);
  }

  // - insert record -
  trace_queue_s_insert_blank(&this->header_queue);
  trace_record_s *header_record = trace_queue_s_last(&this->header_queue);
  trace_queue_header_s *trqh = (trace_queue_header_s *)header_record->data;

  // - fill record header (except crc) -
  header_record->header.id = ++this->header_last_id;
  header_record->header.time = a_time;

  // - fill record data -
  trqh->used = this->trace_queue.used;
  trqh->begin = this->trace_queue.begin;

  // - compute record crc -
  header_record->header.crc = trace_record_s_compute_crc(header_record,this->header_queue.rec_size);

  // - sync data to storage -
  if (trace_record_s_msync(header_record,this->header_queue.rec_size))
  {
    throw_error(TRACE_RECORD_MSYNC_ERROR);
  }

  return 0;
}/*}}}*/

int trace_s_write_record(trace_s *this,time_s a_time,const char *a_data)
{/*{{{*/
  if (this->trace_queue.used >= this->trace_queue.size)
  {
    // - drop tail record -
    trace_queue_s_next(&this->trace_queue);

    // - write trace header to storage -
    if (trace_s_write_header(this,a_time))
    {
      throw_error(TRACE_WRITE_HEADER_ERROR);
    }
  }

  // - insert record -
  trace_queue_s_insert_blank(&this->trace_queue);
  trace_record_s *trace_record = trace_queue_s_last(&this->trace_queue);

  // - fill record header (except crc) -
  trace_record->header.id = ++this->trace_last_id;
  trace_record->header.time = a_time;

  // - fill record data -
  memcpy(trace_record->data,a_data,this->data_size);

  // - compute record crc -
  trace_record->header.crc = trace_record_s_compute_crc(trace_record,this->trace_queue.rec_size);

  // - sync data to storage -
  if (trace_record_s_msync(trace_record,this->trace_queue.rec_size))
  {
    throw_error(TRACE_RECORD_MSYNC_ERROR);
  }

  // - write trace header to storage -
  if (trace_s_write_header(this,a_time))
  {
    throw_error(TRACE_WRITE_HEADER_ERROR);
  }

  return 0;
}/*}}}*/

int trace_s_read_record(trace_s *this,lli a_id,time_s *a_time,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  if (a_id > this->trace_last_id || a_id <= (this->trace_last_id - this->trace_queue.used))
  {
    throw_error(TRACE_INVALID_RECORD_ID);
  }

  unsigned record_idx = (this->trace_queue.used - 1) - (this->trace_last_id - a_id);
  trace_record_s *trace_record = trace_queue_s_at(&this->trace_queue,record_idx);

  // - ERROR -
  if (trace_record_s_compute_crc(trace_record,this->trace_queue.rec_size) != trace_record->header.crc)
  {
    throw_error(TRACE_INVALID_RECORD_CRC);
  }

  *a_time = trace_record->header.time;
  bc_array_s_append(a_trg,this->trace_queue.rec_size,trace_record->data);

  return 0;
}/*}}}*/

// === global functions ========================================================

void libtrace_cll_init()
{/*{{{*/

  // - retrieve system page size -
  g_page_size = sysconf(_SC_PAGE_SIZE);

}/*}}}*/

void libtrace_cll_clear()
{/*{{{*/
}/*}}}*/

