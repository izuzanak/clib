
@begin
include "cl_logger.h"
@end

// === methods of generated structures =========================================

// -- log_file_s --
@begin
methods log_file_s
@end

int log_file_s_rotate(log_file_s *this)
{/*{{{*/
  struct stat st;

  // - close log file -
  file_s_clear(&this->file);

  CONT_INIT_CLEAR(bc_array_s,old_buff);
  CONT_INIT_CLEAR(bc_array_s,new_buff);

  unsigned new_idx = this->max_count - 1;
  do
  {
    // - format name of new file -
    if (new_idx > 0)
    {
      new_buff.used = 0;
      bc_array_s_append_format(&new_buff,"%s.%u",this->path.data,new_idx);
    }
    else
    {
      new_buff.used = 0;
      bc_array_s_append(&new_buff,this->path.size,this->path.data);
    }

    if (new_idx + 1 < this->max_count)
    {
      // - format name of old file -
      old_buff.used = 0;
      bc_array_s_append_format(&old_buff,"%s.%u",this->path.data,new_idx + 1);

      // - unlink old file -
      if (stat(old_buff.data,&st) == 0)
      {
        // - ERROR -
        if (unlink(old_buff.data))
        {
          throw_error(LOG_FILE_UNLINK_ERROR);
        }
      }

      // - rename new file to old file -
      if (stat(new_buff.data,&st) == 0)
      {
        // - ERROR -
        if (rename(new_buff.data,old_buff.data))
        {
          throw_error(LOG_FILE_RENAME_ERROR);
        }
      }
    }
    else
    {
      // - unlink new file -
      if (stat(new_buff.data,&st) == 0)
      {
        // - ERROR -
        if (unlink(new_buff.data))
        {
          throw_error(LOG_FILE_UNLINK_ERROR);
        }
      }
    }

  } while(new_idx-- != 0);

  // - open new log file -
  if (file_s_open(&this->file,new_buff.data,"a"))
  {
    throw_error(LOG_FILE_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

// -- log_file_tree_s --
@begin
methods log_file_tree_s
@end

// -- logger_s --
@begin
methods logger_s
@end

int logger_s_add_file(logger_s *this,
    unsigned a_level,const char *a_path,unsigned a_max_size,unsigned a_max_count)
{/*{{{*/

  // - ERROR -
  if (a_max_size < 4096 || a_max_count < 1)
  {
    throw_error(LOGGER_LOG_FILE_INVALID_PARAMETERS);
  }

  log_file_s search_log_file = {{strlen(a_path) + 1,(char *)a_path},};
  unsigned log_file_idx = log_file_tree_s_get_idx(&this->log_files,&search_log_file);

  // - ERROR -
  if (log_file_idx != c_idx_not_exist)
  {
    throw_error(LOGGER_LOG_FILE_ALREADY_REGISTERED);
  }

  CONT_INIT_CLEAR(log_file_s,log_file);

  // - create log file -
  if (log_file_s_create(&log_file,a_level,a_path,a_max_size,a_max_count))
  {
    throw_error(LOGGER_LOG_FILE_CREATE_ERROR);
  }

  // - insert log file to tree -
  log_file_tree_s_swap_insert(&this->log_files,&log_file);

  return 0;
}/*}}}*/

int logger_s_write(logger_s *this,unsigned a_level,const char *a_format,...)
{/*{{{*/
  this->buffer.used = 0;

  time_s time;
  if (clock_s_gettime(CLOCK_REALTIME,&time))
  {
    throw_error(LOGGER_GET_TIME_ERROR);
  }

  // - format message prefix -
  time_s_to_string(&time,&this->buffer);
  bc_array_s_append_format(&this->buffer," %u %s: ",a_level,this->user.data);

  // - format user message -
  va_list ap;
  va_start(ap,a_format);
  bc_array_s_append_format_ap(&this->buffer,a_format,ap);
  va_end(ap);

  // - append end of line -
  bc_array_s_push(&this->buffer,'\n');

  int write_error = 0;

  if (this->log_files.root_idx != c_idx_not_exist)
  {
    log_file_tree_s_node *lftn_ptr = this->log_files.data;
    log_file_tree_s_node *lftn_ptr_end = lftn_ptr + this->log_files.used;
    do {
      if (lftn_ptr->valid)
      {
        log_file_s *log_file = &lftn_ptr->object;

        // - check log level -
        if (log_file->level <= a_level)
        {
          if (log_file_s_write(log_file,this->buffer.used,this->buffer.data))
          {
            write_error |= 1;
          }
        }
      }
    } while(++lftn_ptr < lftn_ptr_end);
  }

  // - ERROR -
  if (write_error)
  {
    throw_error(LOGGER_LOG_WRITE_ERROR);
  }

  return 0;
}/*}}}*/

