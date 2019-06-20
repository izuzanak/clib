
#ifndef __CL_LOGGER_H
#define __CL_LOGGER_H

@begin
include "cl_sys.h"
include "cl_time.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define liblogger_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef liblogger_cll_EXPORTS
#define liblogger_cll_EXPORT __declspec(dllexport)
#else
#define liblogger_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_LOGGER_LOG_FILE_INVALID_PARAMETERS 1
#define ERROR_LOGGER_LOG_FILE_ALREADY_REGISTERED 2
#define ERROR_LOGGER_LOG_FILE_CREATE_ERROR 3
#define ERROR_LOGGER_GET_TIME_ERROR 4
#define ERROR_LOGGER_LOG_WRITE_ERROR 5

#define ERROR_LOG_FILE_OPEN_ERROR 1
#define ERROR_LOG_FILE_FTELL_ERROR 2
#define ERROR_LOG_FILE_ROTATE_ERROR 3
#define ERROR_LOG_FILE_WRITE_ERROR 4
#define ERROR_LOG_FILE_UNLINK_ERROR 5
#define ERROR_LOG_FILE_RENAME_ERROR 6

// === definition of generated structures ======================================

// -- log_file_s --
@begin
struct
<
string_s:path
ui:max_size
ui:max_count
ui:level
file_s:file
>
log_file_s;
@end

WUR static inline int log_file_s_create(log_file_s *this,
    unsigned a_level,const char *a_path,unsigned a_max_size,unsigned a_max_count);
WUR static inline int log_file_s_write(log_file_s *this,unsigned a_size,const char *a_data);
WUR liblogger_cll_EXPORT int log_file_s_rotate(log_file_s *this);

// -- log_file_tree_s --
@begin
safe_rb_tree<log_file_s> log_file_tree_s;
@end

// -- logger_s --
@begin
struct
<
string_s:user
log_file_tree_s:log_files
bc_array_s:buffer
>
logger_s;
@end

static inline void logger_s_create(logger_s *this,const char *a_user);
liblogger_cll_EXPORT WUR int logger_s_add_file(logger_s *this,
    unsigned a_level,const char *a_path,unsigned a_max_size,unsigned a_max_count);
liblogger_cll_EXPORT WUR int logger_s_write(logger_s *this,unsigned a_level,const char *a_format,...);

// === inline methods of generated structures ==================================

// -- log_file_s --
@begin
inlines log_file_s
@end

static inline int log_file_s_create(log_file_s *this,
    unsigned a_level,const char *a_path,unsigned a_max_size,unsigned a_max_count)
{/*{{{*/
  string_s_set_ptr(&this->path,a_path);
  this->max_size = a_max_size;
  this->max_count = a_max_count;
  this->level = a_level;

  // - ERROR -
  if (file_s_open(&this->file,a_path,"a"))
  {
    throw_error(LOG_FILE_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int log_file_s_write(log_file_s *this,unsigned a_size,const char *a_data)
{/*{{{*/

  // - ERROR -
  long count = ftell(this->file);
  if (count == -1)
  {
    throw_error(LOG_FILE_FTELL_ERROR);
  }

  // - maximal byte count exceeded -
  if (count + a_size > this->max_size)
  {
    // - ERROR -
    if (log_file_s_rotate(this))
    {
      throw_error(LOG_FILE_ROTATE_ERROR);
    }
  }

  // - ERROR -
  if (stream_s_write(&this->file,a_data,a_size))
  {
    throw_error(LOG_FILE_WRITE_ERROR);
  }

  return 0;
}/*}}}*/

// -- log_file_tree_s --
@begin
inlines log_file_tree_s
@end

static inline int log_file_tree_s___compare_value(const log_file_tree_s *this,const log_file_s *a_first,const log_file_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *f_name = &a_first->path;
  const string_s *s_name = &a_second->path;

  if (f_name->size < s_name->size) { return -1; }
  if (f_name->size > s_name->size) { return 1; }
  return memcmp(f_name->data,s_name->data,f_name->size - 1);
}/*}}}*/

// -- logger_s --
@begin
inlines logger_s
@end

static inline void logger_s_create(logger_s *this,const char *a_user)
{/*{{{*/
  logger_s_clear(this);

  string_s_set_ptr(&this->user,a_user);
}/*}}}*/

#endif

