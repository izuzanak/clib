
@begin
include "sd_segfile.h"
@end

// === constants and definitions ===============================================

// === methods of generated structures =========================================

// -- sd_segment_file_s --
@begin
methods sd_segment_file_s
@end

int sd_segment_file_s_create(sd_segment_file_s *this,
    const char *a_path,unsigned a_data_size)
{/*{{{*/
  sd_segment_file_s_clear(this);

  // - set record size -
  this->data_size = a_data_size;
  this->rec_size = sizeof(sd_record_header_s) + this->data_size;;

  // - reset last record identifier -
  this->last_id = -1;

  // - reset last record index -
  this->last_index = 0;

  // - open file -
  this->fd = open(a_path,O_RDWR | O_CREAT,0666);
  if (this->fd == -1)
  {
    throw_error(SD_SEGFILE_FILE_OPEN_ERROR);
  }

  struct stat stat;
  if (fstat(this->fd,&stat))
  {
    throw_error(SD_SEGFILE_FILE_STAT_ERROR);
  }

  // - resize file if needed -
  if (stat.st_size != this->rec_size << 1)
  {
    if (ftruncate(this->fd,this->rec_size << 1))
    {
      throw_error(SD_SEGFILE_FILE_TRUNCATE_ERROR);
    }
  }

  // - search for latest record -
  unsigned idx = 0;
  do {
    this->rec_data.used = 0;

    // - read record data -
    if (fd_s_read_cnt(&this->fd,this->rec_size,&this->rec_data))
    {
      throw_error(SD_SEGFILE_RECORD_READ_ERROR);
    }

    sd_record_s *record = (sd_record_s *)this->rec_data.data;
    debug_assert(record != NULL);

    // - record crc is valid -
    if (sd_record_s_compute_crc(record,this->rec_size) == record->header.crc)
    {
      if (record->header.id > this->last_id)
      {
        this->last_id = record->header.id;
        this->last_index = idx;
      }
    }
  } while(++idx < 2);

  // - reset record data -
  this->rec_data.used = 0;

  // - some record was found -
  if (this->last_id > -1)
  {
    // - seek to record -
    if (lseek64(this->fd,(off64_t)this->last_index*this->rec_size,SEEK_SET) == -1)
    {
      throw_error(SD_SEGFILE_RECORD_SEEK_ERROR);
    }

    // - read record data -
    if (fd_s_read_cnt(&this->fd,this->rec_size,&this->rec_data))
    {
      throw_error(SD_SEGFILE_RECORD_READ_ERROR);
    }
  }

  return 0;
}/*}}}*/

int sd_segment_file_s_write_record(sd_segment_file_s *this,
    time_s a_time,unsigned a_size,const char *a_data)
{/*{{{*/
  debug_assert(a_size <= this->data_size);

  this->rec_data.used = 0;
  bc_array_s_reserve(&this->rec_data,this->rec_size);

  sd_record_s *record = (sd_record_s *)this->rec_data.data;

  // - fill record header (except crc) -
  record->header.id = ++this->last_id;
  record->header.time = a_time;
  record->header.data_size = a_size;

  // - fill record data -
  memcpy(record->data,a_data,a_size);

  // - fill rest of record data -
  if (a_size < this->data_size)
  {
    memset(record->data + a_size,0,this->data_size - a_size);
  }

  // - compute record crc -
  record->header.crc = sd_record_s_compute_crc(record,this->rec_size);

  this->rec_data.used = this->rec_size;

  // - seek to record -
  this->last_index = 1 - this->last_index;
  if (lseek64(this->fd,(off64_t)this->last_index*this->rec_size,SEEK_SET) == -1)
  {
    throw_error(SD_SEGFILE_RECORD_SEEK_ERROR);
  }

  // - write record to file -
  if (fd_s_write(&this->fd,this->rec_data.data,this->rec_data.used))
  {
    throw_error(SD_SEGFILE_RECORD_WRITE_ERROR);
  }

  // - sync data to file -
  if (fsync(this->fd) == -1)
  {
    throw_error(SD_SEGFILE_FILE_SYNC_ERROR);
  }

  return 0;
}/*}}}*/

int sd_segment_file_s_get_record(sd_segment_file_s *this,
    time_s *a_time,bc_array_s *a_record)
{/*{{{*/
  if (this->last_id < 0)
  {
    throw_error(SD_SEGFILE_RECORD_NOT_EXIST);
  }

  sd_record_s *record = (sd_record_s *)this->rec_data.data;

  *a_time = record->header.time;

  a_record->size = record->header.data_size;
  a_record->used = a_record->size;
  a_record->data = record->data;

  return 0;
}/*}}}*/

