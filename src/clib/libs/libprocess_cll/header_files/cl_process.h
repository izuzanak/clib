
#ifndef CL_LIB_PROCESS_H
#define CL_LIB_PROCESS_H

@begin
include "cl_logger.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libprocess_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libprocess_cll_EXPORTS
#define libprocess_cll_EXPORT __declspec(dllexport)
#else
#define libprocess_cll_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef SHELF
#define PROCESS_RUN_DIR_PATH "/var/run"
#define PROCESS_LOG_DIR_PATH "/var/log"
#else
#define PROCESS_RUN_DIR_PATH "/dev/shm/run"
#define PROCESS_LOG_DIR_PATH "/dev/shm/log"
#endif

// - error codes -
#define ERROR_PROCESS_DIRECTORY_MISSING 1
#define ERROR_PROCESS_DIRECTORY_CREATE_ERROR 2
#define ERROR_PROCESS_LOGGER_CREATE_ERROR 3
#define ERROR_PROCESS_RUN_FILE_ALREADY_EXIST 4
#define ERROR_PROCESS_RUN_FILE_CREATE_ERROR 5

// === definition of structure process_s =======================================

typedef struct process_s
{
  string_s name;
  logger_s logger;
  pid_t pid;
} process_s;

@begin
define process_s dynamic
@end

static inline void process_s_init(process_s *this);
static inline void process_s_clear(process_s *this);
static inline void process_s_flush_all(process_s *this);
static inline void process_s_swap(process_s *this,process_s *a_second);
static inline void process_s_copy(process_s *this,const process_s *a_src);
static inline int process_s_compare(const process_s *this,const process_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void process_s_to_string(const process_s *this,bc_array_s *a_trg);
#endif

WUR libprocess_cll_EXPORT int process_s_check_dir(const char *a_dir);
WUR libprocess_cll_EXPORT int process_s_create(process_s *this,const char *a_name);

// === inline methods of structure process_s ===================================

static inline void process_s_init(process_s *this)
{/*{{{*/
  this->pid = -1;
  string_s_init(&this->name);
  logger_s_init(&this->logger);
}/*}}}*/

static inline void process_s_clear(process_s *this)
{/*{{{*/
  if (this->pid != -1)
  {
    // - log message -
    log_info_0("termination");

    CONT_INIT_CLEAR(bc_array_s,pid_path);
    bc_array_s_append_format(&pid_path,PROCESS_RUN_DIR_PATH "/%s.pid",this->name.data);

    CONT_INIT_CLEAR(bc_array_s,pid_str);

    // - read process pid from file -
    CONT_INIT_CLEAR(file_s,file);
    if (file_s_open(&file,pid_path.data,"r") == 0 &&
        file_s_read_close(&file,&pid_str) == 0)
    {
      pid_t pid = strtoll(pid_str.data,NULL,10);

      // - pid match with process pid -
      if (pid == this->pid)
      {
        // - unlink pid file -
        unlink(pid_path.data);
      }
    }
  }

  // - reset global logger pointer -
  g_logger = NULL;

  string_s_clear(&this->name);
  logger_s_clear(&this->logger);

  process_s_init(this);
}/*}}}*/

static inline void process_s_flush_all(process_s *this)
{/*{{{*/
}/*}}}*/

static inline void process_s_swap(process_s *this,process_s *a_second)
{/*{{{*/
  process_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void process_s_copy(process_s *this,const process_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int process_s_compare(const process_s *this,const process_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void process_s_to_string(const process_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"process_s{%ld}",(long)this->pid);
}/*}}}*/
#endif

#endif

