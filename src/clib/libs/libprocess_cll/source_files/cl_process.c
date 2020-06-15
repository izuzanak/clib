
@begin
include "cl_process.h"
@end

// === methods of structure process_s ==========================================

int process_s_check_dir(const char *a_dir)
{/*{{{*/
  struct stat st;

  // - check run directory -
  if (stat(a_dir,&st) != 0 || !S_ISDIR(st.st_mode))
  {
    throw_error(PROCESS_DIRECTORY_MISSING);
  }

  return 0;
}/*}}}*/

int process_s_create(process_s *this,const char *a_name)
{/*{{{*/
  process_s_clear(this);

  // - check log directory -
  if (process_s_check_dir(PROCESS_LOG_DIR_PATH))
  {
    throw_error(PROCESS_DIRECTORY_CREATE_ERROR);
  }

  // - create logger -
  logger_s_create(&this->logger,a_name);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  bc_array_s_append_format(&buffer,PROCESS_LOG_DIR_PATH "/%s.log",a_name);

  if (logger_s_add_file(&this->logger,9,buffer.data,1048576,2))
  {
    throw_error(PROCESS_LOGGER_CREATE_ERROR);
  }

  // - set global logger pointer -
  g_logger = &this->logger;

  // - check run directory -
  if (process_s_check_dir(PROCESS_RUN_DIR_PATH))
  {
    throw_error(PROCESS_DIRECTORY_CREATE_ERROR);
  }

  buffer.used = 0;
  bc_array_s_append_format(&buffer,PROCESS_RUN_DIR_PATH "/%s.pid",a_name);

  struct stat st;

  // - check process pid file -
  if (stat(buffer.data,&st) == 0)
  {
    throw_error(PROCESS_RUN_FILE_ALREADY_EXIST);
  }

  pid_t pid = getpid();

  // - create process pid file -
  CONT_INIT_CLEAR(file_s,file);

  if (file_s_open(&file,buffer.data,"w") ||
      (buffer.used = 0,bc_array_s_append_format(&buffer,"%ld\n",(long)pid),0) ||
      file_s_write_close(&file,buffer.data,buffer.used))
  {
    throw_error(PROCESS_RUN_FILE_CREATE_ERROR);
  }

  this->pid = pid;
  string_s_set_ptr(&this->name,a_name);

  // - log message -
  log_info_0("initialization");

  return 0;
}/*}}}*/

