
@begin
include "sd_segfiles.h"
@end

// === constants and definitions ===============================================

// === methods of generated structures =========================================

// -- sd_segment_files_s --
@begin
methods sd_segment_files_s
@end

int sd_segment_files_s_create(sd_segment_files_s *this,
    const char *a_path,unsigned a_data_size)
{/*{{{*/
  sd_segment_files_s_clear(this);

  // - set file paths -
  string_s_set_ptr(&this->path,a_path);
  string_s_set_format(&this->path_tmp,"%s.tmp",a_path);

  // - set record size -
  this->data_size = a_data_size;
  this->rec_size = sizeof(sd_record_header_s) + this->data_size;;

  // - reset last record identifier -
  this->last_id = -1;

  // - open file -
  CONT_INIT_CLEAR(fd_s,fd);
  fd = open(this->path.data,O_RDWR | O_CREAT,0666);
  if (fd == -1)
  {
    throw_error(SD_SEGFILES_FILE_OPEN_ERROR);
  }

  struct stat stat;
  if (fstat(fd,&stat))
  {
    throw_error(SD_SEGFILES_FILE_STAT_ERROR);
  }

  // - resize file if needed -
  if (stat.st_size == this->rec_size)
  {
    // - read record data -
    if (fd_s_read_cnt(&fd,this->rec_size,&this->rec_data))
    {
      throw_error(SD_SEGFILES_RECORD_READ_ERROR);
    }

    sd_record_s *record = (sd_record_s *)this->rec_data.data;
    debug_assert(record != NULL);

    // - record crc is valid -
    if (sd_record_s_compute_crc(record,this->rec_size) == record->header.crc)
    {
      if (record->header.id > this->last_id)
      {
        this->last_id = record->header.id;
      }
    }
    else
    {
      this->rec_data.used = 0;
    }
  }

  return 0;
}/*}}}*/

int sd_segment_files_s_write_record(sd_segment_files_s *this,
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


  // - write record to temporary file -
  CONT_INIT_CLEAR(fd_s,fd);

  if ((fd = open(this->path_tmp.data,O_WRONLY | O_CREAT,0666)) == -1 ||
      fd_s_write(&fd,this->rec_data.data,this->rec_data.used) ||
      fsync(fd))
  {
    throw_error(SD_SEGFILES_RECORD_WRITE_ERROR);
  }

  fd_s_clear(&fd);

  // - syncfs is used to asure, that directory entry is also synced -
  if (rename(this->path_tmp.data,this->path.data) ||
      (fd = open(this->path.data,O_WRONLY)) == -1 ||
      syncfs(fd))
  {
    throw_error(SD_SEGFILES_RECORD_WRITE_ERROR);
  }

  return 0;
}/*}}}*/

int sd_segment_files_s_get_record(sd_segment_files_s *this,
    time_s *a_time,bc_array_s *a_record)
{/*{{{*/
  if (this->last_id < 0)
  {
    throw_error(SD_SEGFILES_RECORD_NOT_EXIST);
  }

  sd_record_s *record = (sd_record_s *)this->rec_data.data;

  *a_time = record->header.time;

  a_record->size = record->header.data_size;
  a_record->used = a_record->size;
  a_record->data = record->data;

  return 0;
}/*}}}*/

