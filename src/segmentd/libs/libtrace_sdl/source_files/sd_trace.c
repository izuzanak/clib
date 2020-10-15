
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
    unsigned a_header_type,
    void *header_data,ulli header_size,
    sd_conf_segment_s *a_header_segment,
    void *trace_data,ulli sd_trace_size,
    unsigned a_ts_type,
    void *ts_trace_data,ulli ts_trace_size,
    sd_conf_segment_s *a_ts_segment,
    unsigned a_data_size,
    lli a_timestamp_div,
    int a_timestamp_nomem)
{/*{{{*/
  sd_trace_s_clear(this);

  // - store data size -
  this->data_size = a_data_size;

  // - store header type -
  this->header_type = a_header_type;

  // - store timestamp type -
  this->ts_type = a_ts_type;

  // - store timestamp divisor -
  this->timestamp_div = a_timestamp_div;

  // - store timestamp nomem flag -
  this->timestamp_nomem = a_timestamp_nomem;

  // - initialize trace queue header -
  sd_trace_queue_header_s trqh = {
    .used = 0,
    .begin = 0
  };

  switch (this->header_type)
  {
  case c_sd_trace_data_type_MMAP:
    {/*{{{*/
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
    }/*}}}*/
    break;
  case c_sd_trace_data_type_SEGMENT:
    {/*{{{*/
      if (a_header_segment->size < sizeof(sd_trace_queue_header_s))
      {
        throw_error(SD_TRACE_SEGMENT_INVALID_SIZE);
      }

      if (sd_segment_descr_s_create(&this->header_segment,a_header_segment))
      {
        throw_error(SD_TRACE_SEGMENT_CREATE_ERROR);
      }

      // - read record from segment -
      time_s time;
      bc_array_s record;
      if (sd_segment_handle_s_get_record(&this->header_segment.handle,&time,&record))
      {
        // - reset last trace id -
        this->trace_last_id = -1;
      }
      else
      {
        // - retrieve trace queue header -
        trqh = *((sd_trace_queue_header_s *)record.data);
      }
    }/*}}}*/
    break;
  default:
    throw_error(SD_TRACE_INVALID_TRACE_DATA_TYPE);
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

    // - timestamp queue on storage -
    if (this->timestamp_div > 0)
    {
      switch (this->ts_type)
      {
      case c_sd_trace_data_type_MMAP:
        {/*{{{*/
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
            // - update begin of timestamp trace queue -
            this->ts_trace_queue.begin = (last_ts_trace_idx + 1) % this->ts_trace_queue.size;
          }
        }/*}}}*/
        break;
      case c_sd_trace_data_type_SEGMENT:
        {/*{{{*/
          if (a_ts_segment->size < trace_rec_cnt*sizeof(sd_record_timestamp_s))
          {
            throw_error(SD_TRACE_SEGMENT_INVALID_SIZE);
          }

          if (sd_segment_descr_s_create(&this->ts_segment,a_ts_segment))
          {
            throw_error(SD_TRACE_SEGMENT_CREATE_ERROR);
          }
        }/*}}}*/
        break;
      default:
        throw_error(SD_TRACE_INVALID_TRACE_DATA_TYPE);
      }
    }

    if (!this->timestamp_nomem)
    {
      if (sd_trace_s_read_timestamp_structures(this))
      {
        throw_error(SD_TRACE_READ_TIMESTAMP_STRUCTURES_ERROR);
      }

      //fprintf(stderr,"TIMESTAMP QUEUE:\n");
      //DEBUG_PRINT_LINES(sd_record_timestamp_queue_s,&this->timestamp_queue);

      //fprintf(stderr,"TIMESTAMP TREE:\n");
      //DEBUG_PRINT_LINES(sd_record_timestamp_tree_s,&this->timestamp_tree);
    }
  }

  return 0;
}/*}}}*/

int sd_trace_s_read_timestamp_structures(sd_trace_s *this)
{/*{{{*/
  unsigned trace_rec_cnt = this->trace_queue.size;

  // - timestamp queue on storage -
  if (this->timestamp_div > 0)
  {
    switch (this->ts_type)
    {
    case c_sd_trace_data_type_MMAP:
      {/*{{{*/
        if (this->ts_trace_queue.used != 0)
        {
          sd_record_timestamp_array_s_set(&this->timestamp_buffer,trace_rec_cnt,
            (sd_record_timestamp_s *)sd_trace_queue_s_last(&this->ts_trace_queue)->data);
        }
      }/*}}}*/
      break;
    case c_sd_trace_data_type_SEGMENT:
      {/*{{{*/

        // - read record from segment -
        time_s time;
        bc_array_s record;
        if (sd_segment_handle_s_get_record(&this->ts_segment.handle,&time,&record) == 0)
        {
          debug_assert(record.used == trace_rec_cnt*sizeof(sd_record_timestamp_s));

          sd_record_timestamp_array_s_set(&this->timestamp_buffer,trace_rec_cnt,
            (sd_record_timestamp_s *)record.data);
        }
      }/*}}}*/
      break;
    default:
      throw_error(SD_TRACE_INVALID_TRACE_DATA_TYPE);
    }
  }

  // - timestamp last id and index -
  lli timestamp_last_id = -1;
  unsigned timestamp_last_idx = c_idx_not_exist;

  // - timestamp buffer was retrieved -
  if (this->timestamp_buffer.used != 0)
  {
    // - find timestamp last id and index -
    sd_record_timestamp_s *ts_ptr = this->timestamp_buffer.data;
    sd_record_timestamp_s *ts_ptr_end = ts_ptr + this->timestamp_buffer.used;
    do {
      if (ts_ptr->id > -1)
      {
        if (ts_ptr->id > timestamp_last_id)
        {
          // - store last id and its position -
          timestamp_last_id = ts_ptr->id;
          timestamp_last_idx = (ts_ptr - this->timestamp_buffer.data);
        }
      }
    } while(++ts_ptr < ts_ptr_end);
  }
  else
  {
    // - create timestamp buffer -
    sd_record_timestamp_array_s_copy_resize(&this->timestamp_buffer,trace_rec_cnt);
    this->timestamp_buffer.used = trace_rec_cnt;

    // - check created timestamp buffer -
    if (this->timestamp_buffer.used != 0)
    {
      // - reset timestamp values -
      sd_record_timestamp_s *ts_ptr = this->timestamp_buffer.data;
      sd_record_timestamp_s *ts_ptr_end = ts_ptr + this->timestamp_buffer.used;
      do {
        ts_ptr->id = -1;
        ts_ptr->time = 0;
      } while(++ts_ptr < ts_ptr_end);
    }
  }

  // - set timestamp queue buffer -
  sd_record_timestamp_queue_s_set_buffer(&this->timestamp_queue,
      this->timestamp_buffer.used,this->timestamp_buffer.data);

  this->timestamp_queue.used = this->timestamp_buffer.used;

  // - update timestamp queue begin -
  if (timestamp_last_idx != c_idx_not_exist)
  {
    this->timestamp_queue.begin = (timestamp_last_idx + 1) % this->timestamp_queue.size;
  }

  if (!this->timestamp_nomem)
  {
    // - resize timestamp tree -
    sd_record_timestamp_tree_s_copy_resize(&this->timestamp_tree,trace_rec_cnt + 1);

    // - fill timestamp tree -
    if (this->timestamp_queue.used != 0)
    {
      unsigned tsq_idx = 0;
      do 
      {
        // - insert timestamp to timestamp tree -
        sd_record_timestamp_tree_s_insert(&this->timestamp_tree,
          sd_record_timestamp_queue_s_at(&this->timestamp_queue,tsq_idx)); // NOLINT

      } while(++tsq_idx < this->timestamp_queue.used);
    }

    // - initialize first timestamp tree index -
    this->first_timestamp_tree_idx = 1;
  }

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

  return 0;
}/*}}}*/

void sd_trace_s_clear_timestamp_structures(sd_trace_s *this)
{/*{{{*/
  sd_record_timestamp_queue_s_clear(&this->timestamp_queue);
  sd_record_timestamp_array_s_clear(&this->timestamp_buffer);
}/*}}}*/

void sd_trace_s_update_timestamp_structures(sd_trace_s *this,sd_record_timestamp_s *a_timestamp)
{/*{{{*/

  // - insert timestamp to timestamp queue -
  sd_record_timestamp_queue_s_next(&this->timestamp_queue);
  sd_record_timestamp_queue_s_insert(&this->timestamp_queue,a_timestamp);

  if (!this->timestamp_nomem)
  {
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
  }
}/*}}}*/

int sd_trace_s_write_header(sd_trace_s *this,time_s a_time)
{/*{{{*/
  switch (this->header_type)
  {
  case c_sd_trace_data_type_MMAP:
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
    }/*}}}*/
    break;
  case c_sd_trace_data_type_SEGMENT:
    {/*{{{*/
      sd_trace_queue_header_s trqh = {
        this->trace_queue.used,
        this->trace_queue.begin
      };

      // - write record to segment -
      if (sd_segment_handle_s_write_record(&this->header_segment.handle,a_time,
            sizeof(sd_trace_queue_header_s),(const char *)&trqh))
      {
        throw_error(SD_TRACE_SEGMENT_WRITE_ERROR);
      }
    }/*}}}*/
    break;
  default:
    throw_error(SD_TRACE_INVALID_TRACE_DATA_TYPE);
  }

  return 0;
}/*}}}*/

int sd_trace_s_write_timestamp_queue(sd_trace_s *this,time_s a_time)
{/*{{{*/
  debug_assert(this->timestamp_div > 0);

  switch (this->ts_type)
  {
  case c_sd_trace_data_type_MMAP:
    {/*{{{*/
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
      ts_trace_record->header.data_size = this->timestamp_buffer.used*sizeof(sd_record_timestamp_s);

      // - fill record data -
      memcpy(ts_trace_record->data,this->timestamp_buffer.data,ts_trace_record->header.data_size);

      // - compute record crc -
      ts_trace_record->header.crc = sd_record_s_compute_crc(ts_trace_record,this->ts_trace_queue.rec_size);

      // - sync data to storage -
      if (sd_record_s_msync(ts_trace_record,this->ts_trace_queue.rec_size))
      {
        throw_error(SD_TRACE_RECORD_MSYNC_ERROR);
      }
    }/*}}}*/
    break;
  case c_sd_trace_data_type_SEGMENT:
    {/*{{{*/

      // - write record to segment -
      if (sd_segment_handle_s_write_record(&this->ts_segment.handle,a_time,
            this->timestamp_buffer.used*sizeof(sd_record_timestamp_s),
            (const char *)this->timestamp_buffer.data))
      {
        throw_error(SD_TRACE_SEGMENT_WRITE_ERROR);
      }
    }/*}}}*/
    break;
  default:
    throw_error(SD_TRACE_INVALID_TRACE_DATA_TYPE);
  }

  return 0;
}/*}}}*/

lli sd_trace_s_lee_time(sd_trace_s *this,time_s a_time)
{/*{{{*/
  if (this->timestamp_nomem)
  {
    lli id = -1;
    ulli diff = ULLONG_MAX;

    if (this->timestamp_queue.used != 0)
    {
      unsigned tsq_idx = 0;
      do 
      {
        sd_record_timestamp_s *timestamp = sd_record_timestamp_queue_s_at(&this->timestamp_queue,tsq_idx);
        if (timestamp->time <= a_time)
        {
          ulli ts_diff = a_time - timestamp->time;
          if (ts_diff <= diff)
          {
            id = timestamp->id;
            diff = ts_diff;
          }
        }

      } while(++tsq_idx < this->timestamp_queue.used);
    }

    return id;
  }

  sd_record_timestamp_s search_timestamp = {-1,a_time};

  unsigned timestamp_idx = sd_record_timestamp_tree_s_get_lee_idx(&this->timestamp_tree,&search_timestamp);
  if (timestamp_idx == c_idx_not_exist)
  {
    return -1;
  }

  return sd_record_timestamp_tree_s_at(&this->timestamp_tree,timestamp_idx)->id;
}/*}}}*/

lli sd_trace_s_gre_time(sd_trace_s *this,time_s a_time)
{/*{{{*/
  if (this->timestamp_nomem)
  {
    lli id = -1;
    ulli diff = ULLONG_MAX;

    if (this->timestamp_queue.used != 0)
    {
      unsigned tsq_idx = 0;
      do 
      {
        sd_record_timestamp_s *timestamp = sd_record_timestamp_queue_s_at(&this->timestamp_queue,tsq_idx);
        if (timestamp->time >= a_time)
        {
          ulli ts_diff = timestamp->time - a_time;
          if (ts_diff < diff)
          {
            id = timestamp->id;
            diff = ts_diff;
          }
        }

      } while(++tsq_idx < this->timestamp_queue.used);
    }

    return id;
  }

  sd_record_timestamp_s search_timestamp = {-1,a_time};

  unsigned timestamp_idx = sd_record_timestamp_tree_s_get_gre_idx(&this->timestamp_tree,&search_timestamp);
  if (timestamp_idx == c_idx_not_exist)
  {
    return -1;
  }

  return sd_record_timestamp_tree_s_at(&this->timestamp_tree,timestamp_idx)->id;
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
    if (!this->timestamp_nomem)
    {
      // - update timestamp structures -
      sd_record_timestamp_s timestamp = {this->trace_last_id,a_time};
      sd_trace_s_update_timestamp_structures(this,&timestamp);
    }

    // - timestamp queue on storage -
    if (this->timestamp_div > 0 && this->trace_last_id % this->timestamp_div == 0)
    {
      if (this->timestamp_nomem)
      {
        // - read timestamp structures -
        if (sd_trace_s_read_timestamp_structures(this))
        {
          throw_error(SD_TRACE_READ_TIMESTAMP_STRUCTURES_ERROR);
        }

        // - write timestamp queue -
        if (sd_trace_s_write_timestamp_queue(this,a_time))
        {
          throw_error(SD_TRACE_WRITE_TIMESTAMP_QUEUE_ERROR);
        }

        // - drop timestamp structures -
        sd_trace_s_clear_timestamp_structures(this);
      }
      else
      {
        // - write timestamp queue -
        if (sd_trace_s_write_timestamp_queue(this,a_time))
        {
          throw_error(SD_TRACE_WRITE_TIMESTAMP_QUEUE_ERROR);
        }
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

// -- sd_trace_descr_s --
@begin
methods sd_trace_descr_s
@end

int sd_trace_descr_s_create(sd_trace_descr_s *this,sd_conf_trace_s *a_config)
{/*{{{*/
  sd_trace_descr_s_clear(this);

  // - log message -
  log_info_2("create trace, id: %s",a_config->trace_id.data);

  sd_conf_trace_s_copy(&this->config,a_config);

  if (this->config.header.type == c_sd_trace_data_type_MMAP)
  {
    // - create trace header memory map -
    if (sd_trace_descr_s_mmap_file(&this->mmap_header,&this->config.header.mmap))
    {
      throw_error(SD_TRACE_DESCR_FILES_MMAP_ERROR);
    }
  }

  if (this->config.timestamp.type == c_sd_trace_data_type_MMAP)
  {
    // - create trace timestamp memory map -
    if (sd_trace_descr_s_mmap_file(&this->mmap_timestamp,&this->config.timestamp.mmap))
    {
      throw_error(SD_TRACE_DESCR_FILES_MMAP_ERROR);
    }
  }

  // - create trace memory map -
  if (sd_trace_descr_s_mmap_file(&this->mmap_trace,&this->config.trace))
  {
    throw_error(SD_TRACE_DESCR_FILES_MMAP_ERROR);
  }

  // - create trace -
  if (sd_trace_s_create(&this->trace,
        this->config.header.type,
        this->mmap_header.address,this->config.header.mmap.size,
        &this->config.header.segment,
        this->mmap_trace.address,this->config.trace.size,
        this->config.timestamp.type,
        this->mmap_timestamp.address,this->config.timestamp.mmap.size,
        &this->config.timestamp.segment,
        this->config.record.size,
        this->config.timestamp_div,
        this->config.timestamp_nomem))
  {
    throw_error(SD_TRACE_DESCR_TRACE_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

void sd_trace_descr_s_read_to_message(sd_trace_descr_s *this,lli a_record_id,bc_array_s *a_trg)
{/*{{{*/
  unsigned record_id_used = a_trg->used;
  bc_array_s_push_blanks(a_trg,sizeof(lli) + sizeof(ulli));

  // - read record from trace -
  time_s time;
  if (sd_trace_s_read_record(&this->trace,a_record_id,&time,a_trg))
  {
    // - read error -
    a_trg->used = record_id_used;
    bc_array_s_append_be_lli(a_trg,-1);
    bc_array_s_append_be_ulli(a_trg,0);
  }
  else
  {
    unsigned buffer_used = a_trg->used;

    a_trg->used = record_id_used;
    bc_array_s_append_be_lli(a_trg,a_record_id);
    bc_array_s_append_be_ulli(a_trg,time);

    a_trg->used = buffer_used;
  }
}/*}}}*/

void sd_trace_descr_s_read_size_to_message(sd_trace_descr_s *this,lli a_record_id,bc_array_s *a_trg)
{/*{{{*/
  unsigned record_id_used = a_trg->used;
  bc_array_s_push_blanks(a_trg,sizeof(lli) + sizeof(ulli) + sizeof(unsigned));

  // - read record from trace -
  time_s time;
  if (sd_trace_s_read_record(&this->trace,a_record_id,&time,a_trg))
  {
    // - read error -
    a_trg->used = record_id_used;
    bc_array_s_append_be_lli(a_trg,-1);
    bc_array_s_append_be_ulli(a_trg,0);
    bc_array_s_append_be_ui(a_trg,0);
  }
  else
  {
    unsigned buffer_used = a_trg->used;

    a_trg->used = record_id_used;
    bc_array_s_append_be_lli(a_trg,a_record_id);
    bc_array_s_append_be_ulli(a_trg,time);
    bc_array_s_append_be_ui(a_trg,buffer_used - (a_trg->used + sizeof(unsigned)));

    a_trg->used = buffer_used;
  }
}/*}}}*/

