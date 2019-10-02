
@begin
include "sd_trace.h"
@end

// === constants and definitions ===============================================

// === methods of generated structures =========================================

// -- sd_record_timestamp_array_s --
@begin
methods sd_record_timestamp_array_s
@end

// -- sd_record_timestamp_queue_s --
@begin
methods sd_record_timestamp_queue_s
@end

// -- sd_record_timestamp_tree_s --
@begin
methods sd_record_timestamp_tree_s
@end

// === methods of structure sd_trace_queue_s ===================================

void sd_trace_queue_s_copy(sd_trace_queue_s *this,const sd_trace_queue_s *a_src)
{/*{{{*/
  debug_assert(a_src->used <= this->size && this->rec_size == a_src->rec_size);

  sd_trace_queue_s_clear(this);

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
    sd_record_s_copy((sd_record_s *)ptr,(sd_record_s *)s_ptr,this->rec_size);
  } while(ptr += this->rec_size,(s_ptr += this->rec_size) < s_ptr_end);

  if (sec_cnt != 0)
  {
    s_ptr = a_src->data;
    s_ptr_end = s_ptr + sec_cnt*this->rec_size;

    do {
      sd_record_s_copy((sd_record_s *)ptr,(sd_record_s *)s_ptr,this->rec_size);
    } while(ptr += this->rec_size,(s_ptr += this->rec_size) < s_ptr_end);
  }

  this->used = a_src->used;
}/*}}}*/

int sd_trace_queue_s_compare(const sd_trace_queue_s *this,const sd_trace_queue_s *a_second)
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
    if (!sd_record_s_compare((sd_record_s *)ptr,(sd_record_s *)s_ptr,this->rec_size))
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
void sd_trace_queue_s___to_string(const sd_trace_queue_s *this,bc_array_s *a_trg)
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
      sd_record_s_to_string((sd_record_s *)ptr,a_trg);

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
        sd_record_s_to_string((sd_record_s *)ptr,a_trg);
      } while((ptr += this->rec_size) < ptr_end);
    }
  }

  bc_array_s_push(a_trg,']');
}/*}}}*/
#endif

#if OPTION_TO_STRING == ENABLED
void sd_trace_queue_s_to_string_separator(const sd_trace_queue_s *this,bc_array_s *a_trg,unsigned a_count,const char *a_data)
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
      sd_record_s_to_string((sd_record_s *)ptr,a_trg);

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
        sd_record_s_to_string((sd_record_s *)ptr,a_trg);
      } while((ptr += this->rec_size) < ptr_end);
    }
  }
}/*}}}*/
#endif

unsigned sd_trace_queue_s_scan_for_last(const sd_trace_queue_s *this)
{/*{{{*/
  lli max_id = -1;
  unsigned last_id_idx = c_idx_not_exist;

  // - scan queue for latest record identifier -
  unsigned idx = 0;
  do {
    const sd_record_s *record = sd_trace_queue_s_at(this,idx);

    // - record crc is valid -
    if (sd_record_s_compute_crc(record,this->rec_size) == record->header.crc)
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

// -- sd_trace_queue_header_s --
@begin
methods sd_trace_queue_header_s
@end

// -- sd_trace_mmap_s --
@begin
methods sd_trace_mmap_s
@end

// -- sd_trace_s --
@begin
methods sd_trace_s
@end

int sd_trace_s_create(sd_trace_s *this,
    void *header_data,ulli header_size,
    void *trace_data,ulli sd_trace_size,
    void *ts_trace_data,ulli ts_trace_size,
    unsigned a_data_size,
    lli a_timestamp_div)
{/*{{{*/
  sd_trace_s_clear(this);

  // - store data size -
  this->data_size = a_data_size;

  // - store timestamp divisor -
  this->timestamp_div = a_timestamp_div;

  unsigned header_rec_size = sizeof(sd_record_s) + sizeof(sd_trace_queue_header_s);

  // - ERROR -
  if (header_size < (header_rec_size << 1))
  {
    throw_error(SD_TRACE_HEADER_QUEUE_SIZE_ERROR);
  }

  unsigned header_rec_cnt = header_size/header_rec_size;
  sd_trace_queue_s_set_buffer(&this->header_queue,header_rec_cnt,header_rec_size,header_data);
  this->header_queue.used = this->header_queue.size;

  // - scan queue for latest record index -
  unsigned last_header_idx = sd_trace_queue_s_scan_for_last(&this->header_queue);

  sd_trace_queue_header_s trqh = {
    .used = 0,
    .begin = 0
  };

  // - trace header record was found -
  if (last_header_idx != c_idx_not_exist)
  {
    // - retrieve record pointer before change of queue begin -
    sd_record_s *header_record = sd_trace_queue_s_at(&this->header_queue,last_header_idx);

    // - update begin of header queue -
    this->header_queue.begin = (last_header_idx + 1) % this->header_queue.size;

    // - store last header id -
    this->header_last_id = header_record->header.id;

    // - retrieve trace queue header -
    trqh = *((sd_trace_queue_header_s *)header_record->data);
  }
  else
  {
    // - reset last header id -
    this->header_last_id = -1;

    // - reset last trace id -
    this->trace_last_id = -1;
  }

  unsigned trace_rec_size = sizeof(sd_record_s) + this->data_size;

  // - ERROR -
  if (sd_trace_size < (trace_rec_size << 1))
  {
    throw_error(SD_TRACE_TRACE_QUEUE_SIZE_ERROR);
  }

  unsigned trace_rec_cnt = sd_trace_size/trace_rec_size;
  sd_trace_queue_s_set_buffer(&this->trace_queue,trace_rec_cnt,trace_rec_size,trace_data);

  this->trace_queue.used = trqh.used;
  this->trace_queue.begin = trqh.begin;

  // - retrieve last trace id -
  if (this->trace_queue.used != 0)
  {
    sd_record_s *trace_record = sd_trace_queue_s_last(&this->trace_queue);

    // - ERROR -
    if (sd_record_s_compute_crc(trace_record,this->trace_queue.rec_size) != trace_record->header.crc)
    {
      throw_error(SD_TRACE_INVALID_RECORD_CRC);
    }

    // - store last trace id -
    this->trace_last_id = trace_record->header.id;
  }

  // - timestamp divisor is set -
  if (this->timestamp_div > -1)
  {
    unsigned last_ts_trace_idx = c_idx_not_exist;

    // - timestamp last id and index -
    lli timestamp_last_id = -1;
    unsigned timestamp_last_idx = c_idx_not_exist;
    
    // - timestamp queue on storage -
    if (this->timestamp_div > 0)
    {
      unsigned ts_trace_rec_size = sizeof(sd_record_s) + trace_rec_cnt*sizeof(sd_record_timestamp_s);

      // - ERROR -
      if (ts_trace_size < (ts_trace_rec_size << 1))
      {
        throw_error(SD_TRACE_TIMESTAMP_TRACE_QUEUE_SIZE_ERROR);
      }

      unsigned ts_trace_rec_cnt = ts_trace_size/ts_trace_rec_size;
      sd_trace_queue_s_set_buffer(&this->ts_trace_queue,ts_trace_rec_cnt,ts_trace_rec_size,ts_trace_data);
      this->ts_trace_queue.used = this->ts_trace_queue.size;

      // - scan queue for latest record index -
      last_ts_trace_idx = sd_trace_queue_s_scan_for_last(&this->ts_trace_queue);

      if (last_ts_trace_idx != c_idx_not_exist)
      {
        sd_record_timestamp_array_s_set(&this->timestamp_buffer,trace_rec_cnt,
          (sd_record_timestamp_s *)sd_trace_queue_s_at(&this->ts_trace_queue,last_ts_trace_idx)->data);

        // - update begin of timestamp trace queue -
        this->ts_trace_queue.begin = (last_ts_trace_idx + 1) % this->ts_trace_queue.size;

        // - resize timestamp tree -
        sd_record_timestamp_tree_s_copy_resize(&this->timestamp_tree,this->timestamp_buffer.size + 1);

        // - find timestamp last id and index -
        sd_record_timestamp_s *ts_ptr = this->timestamp_buffer.data;
        sd_record_timestamp_s *ts_ptr_end = ts_ptr + this->timestamp_buffer.size;
        do {
          if (ts_ptr->id > -1)
          {
            // - insert timestamp to timestamp tree -
            sd_record_timestamp_tree_s_insert(&this->timestamp_tree,ts_ptr);

            if (ts_ptr->id > timestamp_last_id)
            {
              // - store last id and its position -
              timestamp_last_id = ts_ptr->id;
              timestamp_last_idx = (ts_ptr - this->timestamp_buffer.data);
            }
          }
        } while(++ts_ptr < ts_ptr_end);
      }
    }

    // - valid timestamp queue was not found -
    if (last_ts_trace_idx == c_idx_not_exist)
    {
      // - create timestamp buffer -
      sd_record_timestamp_array_s_copy_resize(&this->timestamp_buffer,trace_rec_cnt);

      // - resize timestamp tree -
      sd_record_timestamp_tree_s_copy_resize(&this->timestamp_tree,this->timestamp_buffer.size + 1);

      // - reset timestamp values -
      sd_record_timestamp_s *ts_ptr = this->timestamp_buffer.data;
      sd_record_timestamp_s *ts_ptr_end = ts_ptr + this->timestamp_buffer.size;
      do {
        ts_ptr->id = -1;
        ts_ptr->time = 0;
      } while(++ts_ptr < ts_ptr_end);
    }

    // - set timestamp queue buffer -
    sd_record_timestamp_queue_s_set_buffer(&this->timestamp_queue,
        this->timestamp_buffer.size,this->timestamp_buffer.data);

    this->timestamp_queue.used = this->timestamp_buffer.size;

    // - update timestamp queue begin -
    if (timestamp_last_idx != c_idx_not_exist)
    {
      this->timestamp_queue.begin = (timestamp_last_idx + 1) % this->timestamp_queue.size;
    }

    // - initialize first timestamp tree index -
    this->first_timestamp_tree_idx = 1;

    // - update timestamp queue from trace queue -
    if (timestamp_last_id < this->trace_last_id)
    {
      unsigned record_idx = 0;

      // - process tail of trace queue -
      if (this->trace_last_id - timestamp_last_id < this->trace_queue.used)
      {
        record_idx = this->trace_queue.used - (this->trace_last_id - timestamp_last_id);
      }

      do {
        sd_record_s *trace_record = sd_trace_queue_s_at(&this->trace_queue,record_idx);
        sd_record_timestamp_s timestamp;

        // - crc is valid -
        if (sd_record_s_compute_crc(trace_record,this->trace_queue.rec_size) == trace_record->header.crc)
        {
          timestamp.id = trace_record->header.id;
          timestamp.time = trace_record->header.time;
        }

        // - crc is invalid -
        else
        {
          timestamp.id = timestamp_last_id - ((this->trace_queue.used - 1) - record_idx);
          timestamp.time = 0;
        }

        sd_trace_s_update_timestamp_structures(this,&timestamp);

      } while(++record_idx < this->trace_queue.used);
    }

    //fprintf(stderr,"TIMESTAMP QUEUE:\n");
    //DEBUG_PRINT_LINES(sd_record_timestamp_queue_s,&this->timestamp_queue);

    //fprintf(stderr,"TIMESTAMP TREE:\n");
    //DEBUG_PRINT_LINES(sd_record_timestamp_tree_s,&this->timestamp_tree);
  }

  return 0;
}/*}}}*/

void sd_trace_s_update_timestamp_structures(sd_trace_s *this,sd_record_timestamp_s *a_timestamp)
{/*{{{*/

  // - insert timestamp to timestamp queue -
  sd_record_timestamp_queue_s_next(&this->timestamp_queue);
  sd_record_timestamp_queue_s_insert(&this->timestamp_queue,a_timestamp);

  // - remove first timestamp from timestamp tree -
  if (this->timestamp_tree.count >= this->timestamp_queue.used)
  {
    sd_record_timestamp_tree_s_remove(&this->timestamp_tree,this->first_timestamp_tree_idx);

    // - update first timestamp tree index -
    if (++this->first_timestamp_tree_idx > this->timestamp_queue.used)
    {
      this->first_timestamp_tree_idx = 1;
    }
  }

  // - insert timestamp to timestamp tree -
  sd_record_timestamp_tree_s_insert(&this->timestamp_tree,a_timestamp);
}/*}}}*/

int sd_trace_s_write_header(sd_trace_s *this,time_s a_time)
{/*{{{*/
  if (this->header_queue.used >= this->header_queue.size)
  {
    // - drop tail record -
    sd_trace_queue_s_next(&this->header_queue);
  }

  // - insert record -
  sd_trace_queue_s_insert_blank(&this->header_queue);
  sd_record_s *header_record = sd_trace_queue_s_last(&this->header_queue);
  sd_trace_queue_header_s *trqh = (sd_trace_queue_header_s *)header_record->data;

  // - fill record header (except crc) -
  header_record->header.id = ++this->header_last_id;
  header_record->header.time = a_time;
  header_record->header.data_size = sizeof(sd_trace_queue_header_s);

  // - fill record data -
  trqh->used = this->trace_queue.used;
  trqh->begin = this->trace_queue.begin;

  // - compute record crc -
  header_record->header.crc = sd_record_s_compute_crc(header_record,this->header_queue.rec_size);

  // - sync data to storage -
  if (sd_record_s_msync(header_record,this->header_queue.rec_size))
  {
    throw_error(SD_TRACE_RECORD_MSYNC_ERROR);
  }

  return 0;
}/*}}}*/

int sd_trace_s_write_timestamp_queue(sd_trace_s *this,time_s a_time)
{/*{{{*/
  debug_assert(this->timestamp_div > 0);

  if (this->ts_trace_queue.used >= this->ts_trace_queue.size)
  {
    // - drop tail record -
    sd_trace_queue_s_next(&this->ts_trace_queue);
  }

  // - insert record -
  sd_trace_queue_s_insert_blank(&this->ts_trace_queue);
  sd_record_s *ts_trace_record = sd_trace_queue_s_last(&this->ts_trace_queue);

  // - fill record header (except crc) -
  ts_trace_record->header.id = this->trace_last_id;
  ts_trace_record->header.time = a_time;
  ts_trace_record->header.data_size = this->timestamp_buffer.size*sizeof(sd_record_timestamp_s);

  // - fill record data -
  memcpy(ts_trace_record->data,this->timestamp_buffer.data,ts_trace_record->header.data_size);

  // - compute record crc -
  ts_trace_record->header.crc = sd_record_s_compute_crc(ts_trace_record,this->ts_trace_queue.rec_size);

  // - sync data to storage -
  if (sd_record_s_msync(ts_trace_record,this->ts_trace_queue.rec_size))
  {
    throw_error(SD_TRACE_RECORD_MSYNC_ERROR);
  }

  return 0;
}/*}}}*/

int sd_trace_s_write_record(sd_trace_s *this,time_s a_time,unsigned a_size,const char *a_data)
{/*{{{*/
  debug_assert(a_size <= this->data_size);

  if (this->trace_queue.used >= this->trace_queue.size)
  {
    // - drop tail record -
    sd_trace_queue_s_next(&this->trace_queue);

    // - write trace header to storage -
    if (sd_trace_s_write_header(this,a_time))
    {
      throw_error(SD_TRACE_WRITE_HEADER_ERROR);
    }
  }

  // - insert record -
  sd_trace_queue_s_insert_blank(&this->trace_queue);
  sd_record_s *trace_record = sd_trace_queue_s_last(&this->trace_queue);

  // - fill record header (except crc) -
  trace_record->header.id = ++this->trace_last_id;
  trace_record->header.time = a_time;
  trace_record->header.data_size = a_size;

  // - fill record data -
  memcpy(trace_record->data,a_data,a_size);

  // - fill rest of record data -
  if (a_size < this->data_size)
  {
    memset(trace_record->data + a_size,0,this->data_size - a_size);
  }

  // - compute record crc -
  trace_record->header.crc = sd_record_s_compute_crc(trace_record,this->trace_queue.rec_size);

  // - sync data to storage -
  if (sd_record_s_msync(trace_record,this->trace_queue.rec_size))
  {
    throw_error(SD_TRACE_RECORD_MSYNC_ERROR);
  }

  // - write trace header to storage -
  if (sd_trace_s_write_header(this,a_time))
  {
    throw_error(SD_TRACE_WRITE_HEADER_ERROR);
  }

  // - timestamp divisor is set -
  if (this->timestamp_div > -1)
  {
    // - update timestamp structures -
    sd_record_timestamp_s timestamp = {this->trace_last_id,a_time};
    sd_trace_s_update_timestamp_structures(this,&timestamp);

    // - timestamp queue on storage -
    if (this->timestamp_div > 0 && this->trace_last_id % this->timestamp_div == 0)
    {
      if (sd_trace_s_write_timestamp_queue(this,a_time))
      {
        throw_error(SD_TRACE_WRITE_TIMESTAMP_QUEUE_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int sd_trace_s_read_record(sd_trace_s *this,lli a_id,time_s *a_time,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  if (a_id > this->trace_last_id || a_id <= (this->trace_last_id - this->trace_queue.used))
  {
    throw_error(SD_TRACE_INVALID_RECORD_ID);
  }

  unsigned record_idx = (this->trace_queue.used - 1) - (this->trace_last_id - a_id);
  sd_record_s *trace_record = sd_trace_queue_s_at(&this->trace_queue,record_idx);

  // - ERROR -
  if (sd_record_s_compute_crc(trace_record,this->trace_queue.rec_size) != trace_record->header.crc)
  {
    throw_error(SD_TRACE_INVALID_RECORD_CRC);
  }

  *a_time = trace_record->header.time;
  bc_array_s_append(a_trg,trace_record->header.data_size,trace_record->data);

  return 0;
}/*}}}*/

