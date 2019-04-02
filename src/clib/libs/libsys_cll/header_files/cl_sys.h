
#ifndef __CL_SYS_H
#define __CL_SYS_H

@begin
include "cl_struct.h"
@end

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#include <poll.h>
#endif

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libsys_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libsys_cll_EXPORTS
#define libsys_cll_EXPORT __declspec(dllexport)
#else
#define libsys_cll_EXPORT __declspec(dllimport)
#endif
#define popen _popen
#define pclose _pclose
#define fileno _fileno
#endif

// - error codes -
#define ERROR_STREAM_WRITE_ERROR 1
#define ERROR_STREAM_FLUSH_ERROR 2
#define ERROR_STREAM_READ_ERROR 3

#define ERROR_FILE_OPEN_ERROR 1
#define ERROR_FILE_SEEK_ERROR 2
#define ERROR_FILE_TELL_ERROR 3

#define ERROR_PIPE_OPEN_ERROR 1

// === definition of structure pollfd_s ========================================

typedef struct pollfd pollfd_s;
@begin
define pollfd_s static
@end

static inline int pollfd_s_compare(const pollfd_s *this,const pollfd_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void pollfd_s_to_string(const pollfd_s *this,bc_array_s *a_trg);
#endif

// === definition of structure stream_s ========================================

typedef FILE * stream_s;

WUR static inline int stream_s_write_cnt(stream_s *this,const void *a_src,size_t a_size,size_t *a_writed);
WUR static inline int stream_s_write(stream_s *this,const void *a_src,size_t a_size);
WUR static inline int stream_s_fflush(stream_s *this);
WUR libsys_cll_EXPORT int stream_s_read(stream_s *this,bc_array_s *a_trg);
WUR static inline int stream_s_read_cnt(stream_s *this,bc_array_s *a_trg,size_t a_size);
WUR libsys_cll_EXPORT int stream_s_read_max(stream_s *this,bc_array_s *a_trg,size_t a_size);
static inline int stream_s_fd(stream_s *this);

// === definition of structure file_s ==========================================

typedef FILE * file_s;
@begin
define file_s dynamic
@end

static inline void file_s_init(file_s *this);
static inline void file_s_clear(file_s *this);
static inline void file_s_flush_all(file_s *this);
static inline void file_s_swap(file_s *this,file_s *a_second);
static inline void file_s_copy(file_s *this,const file_s *a_src);
static inline int file_s_compare(const file_s *this,const file_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void file_s_to_string(const file_s *this,bc_array_s *a_trg);
#endif

WUR static inline int file_s_open(file_s *this,const char *a_pathname,const char *a_mode);
WUR static inline int file_s_write_close(file_s *this,const void *a_src,size_t a_size);
WUR static inline int file_s_read_close(file_s *this,bc_array_s *a_trg);
WUR static inline int file_s_seek(file_s *this,long a_offset,int a_whence);
WUR static inline int file_s_tell(file_s *this,long *a_offset);

// === definition of structure pipe_s ==========================================

typedef FILE * pipe_s;
@begin
define pipe_s dynamic
@end

static inline void pipe_s_init(pipe_s *this);
static inline void pipe_s_clear(pipe_s *this);
static inline void pipe_s_flush_all(pipe_s *this);
static inline void pipe_s_swap(pipe_s *this,pipe_s *a_second);
static inline void pipe_s_copy(pipe_s *this,const pipe_s *a_src);
static inline int pipe_s_compare(const pipe_s *this,const pipe_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void pipe_s_to_string(const pipe_s *this,bc_array_s *a_trg);
#endif

WUR static inline int pipe_s_popen(pipe_s *this,const char *a_command,const char *a_type);
WUR static inline int pipe_s_write_close(pipe_s *this,const void *a_src,size_t a_size);
WUR static inline int pipe_s_read_close(pipe_s *this,bc_array_s *a_trg);

// === inline methods of structure pollfd_s ====================================

static inline int pollfd_s_compare(const pollfd_s *this,const pollfd_s *a_second)
{/*{{{*/
  return this->fd == a_second->fd &&
         this->events == a_second->events &&
         this->revents == a_second->revents;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void pollfd_s_to_string(const pollfd_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"pollfd_s{%d,%hd,%hd}",
      this->fd,this->events,this->revents);
}/*}}}*/
#endif

// === inline methods of structure stream_s ====================================

static inline int stream_s_write_cnt(stream_s *this,const void *a_src,size_t a_size,size_t *a_writed)
{/*{{{*/
  debug_assert(*this != NULL);

  // - ERROR -
  if ((*a_writed = fwrite(a_src,1,a_size,*this)) != a_size)
  {
    throw_error(STREAM_WRITE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int stream_s_write(stream_s *this,const void *a_src,size_t a_size)
{/*{{{*/
  size_t a_writed;
  return stream_s_write_cnt(this,a_src,a_size,&a_writed);
}/*}}}*/

static inline int stream_s_fflush(stream_s *this)
{/*{{{*/
  debug_assert(*this != NULL);

  // - ERROR -
  if (fflush(*this) != 0)
  {
    throw_error(STREAM_FLUSH_ERROR);
  }

  return 0;
}/*}}}*/

static inline int stream_s_read_cnt(stream_s *this,bc_array_s *a_trg,size_t a_size)
{/*{{{*/
  debug_assert(*this != NULL);

  // - prepare target buffer -
  bc_array_s_reserve(a_trg,a_size);

  if (fread(a_trg->data + a_trg->used,1,a_size,*this) != a_size)
  {
    throw_error(STREAM_READ_ERROR);
  }

  a_trg->used += a_size;

  return 0;
}/*}}}*/

static inline int stream_s_fd(stream_s *this)
{/*{{{*/
  return fileno(*this);
}/*}}}*/

// === inline methods of structure file_s ======================================

static inline void file_s_init(file_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void file_s_clear(file_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    fclose(*this);
  }

  file_s_init(this);
}/*}}}*/

static inline void file_s_flush_all(file_s *this)
{/*{{{*/
}/*}}}*/

static inline void file_s_swap(file_s *this,file_s *a_second)
{/*{{{*/
  file_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void file_s_copy(file_s *this,const file_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int file_s_compare(const file_s *this,const file_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void file_s_to_string(const file_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"file_s{%p}",*this);
}/*}}}*/
#endif

static inline int file_s_open(file_s *this,const char *a_pathname,const char *a_mode)
{/*{{{*/
  file_s_clear(this);

  *this = fopen(a_pathname,a_mode);

  // - ERROR -
  if (*this == NULL)
  {
    throw_error(FILE_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int file_s_write_close(file_s *this,const void *a_src,size_t a_size)
{/*{{{*/
  int res = stream_s_write(this,a_src,a_size);
  if (res != 0)
  {
    return res;
  }

  file_s_clear(this);

  return 0;
}/*}}}*/

static inline int file_s_read_close(file_s *this,bc_array_s *a_trg)
{/*{{{*/
  int res = stream_s_read(this,a_trg);
  if (res != 0)
  {
    return res;
  }

  file_s_clear(this);

  return 0;
}/*}}}*/

static inline int file_s_seek(file_s *this,long a_offset,int a_whence)
{/*{{{*/
  debug_assert(*this != NULL);

  // - ERROR -
  if (fseek(*this,a_offset,a_whence))
  {
    throw_error(FILE_SEEK_ERROR);
  }

  return 0;
}/*}}}*/

static inline int file_s_tell(file_s *this,long *a_offset)
{/*{{{*/

  // - ERROR -
  if ((*a_offset = ftell(*this)) < 0)
  {
    throw_error(FILE_TELL_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure pipe_s ======================================

static inline void pipe_s_init(pipe_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void pipe_s_clear(pipe_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    pclose(*this);
  }

  pipe_s_init(this);
}/*}}}*/

static inline void pipe_s_flush_all(pipe_s *this)
{/*{{{*/
}/*}}}*/

static inline void pipe_s_swap(pipe_s *this,pipe_s *a_second)
{/*{{{*/
  pipe_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void pipe_s_copy(pipe_s *this,const pipe_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int pipe_s_compare(const pipe_s *this,const pipe_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void pipe_s_to_string(const pipe_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"pipe_s{%p}",*this);
}/*}}}*/
#endif

static inline int pipe_s_popen(pipe_s *this,const char *a_command,const char *a_type)
{/*{{{*/
  pipe_s_clear(this);

  *this = popen(a_command,a_type); // NOLINT

  // - ERROR -
  if (*this == NULL)
  {
    throw_error(PIPE_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int pipe_s_write_close(pipe_s *this,const void *a_src,size_t a_size)
{/*{{{*/
  int res = stream_s_write(this,a_src,a_size);
  if (res != 0)
  {
    return res;
  }

  pipe_s_clear(this);

  return 0;
}/*}}}*/

static inline int pipe_s_read_close(pipe_s *this,bc_array_s *a_trg)
{/*{{{*/
  int res = stream_s_read(this,a_trg);
  if (res != 0)
  {
    return res;
  }

  pipe_s_clear(this);

  return 0;
}/*}}}*/

#endif

