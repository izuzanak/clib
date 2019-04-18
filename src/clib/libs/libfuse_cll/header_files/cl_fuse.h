
#ifndef __CL_FUSE_H
#define __CL_FUSE_H

@begin
include "cl_struct.h"
@end

#define FUSE_USE_VERSION 30
#include <fuse3/fuse_lowlevel.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libfuse_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libfuse_cll_EXPORTS
#define libfuse_cll_EXPORT __declspec(dllexport)
#else
#define libfuse_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_FUSE_SESSION_CREATE_ERROR 1
#define ERROR_FUSE_SESSION_MOUNT_ERROR 2
#define ERROR_FUSE_SESSION_RECEIVE_BUFFER_ERROR 3

// === definition of structure fuse_session_s ==================================

typedef struct fuse_session * fuse_session_s;
@begin
define fuse_session_s dynamic
@end

static inline void fuse_session_s_init(fuse_session_s *this);
static inline void fuse_session_s_clear(fuse_session_s *this);
static inline void fuse_session_s_flush_all(fuse_session_s *this);
static inline void fuse_session_s_swap(fuse_session_s *this,fuse_session_s *a_second);
static inline void fuse_session_s_copy(const fuse_session_s *this,const fuse_session_s *a_src);
static inline int fuse_session_s_compare(const fuse_session_s *this,const fuse_session_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void fuse_session_s_to_string(const fuse_session_s *this,bc_array_s *a_trg);
#endif

WUR libfuse_cll_EXPORT int fuse_session_s_create(fuse_session_s *this,string_array_s *a_args,
    struct fuse_lowlevel_ops *a_fuse_ll_ops,void *a_user_data);
WUR static inline int fuse_session_s_mount(fuse_session_s *this,const char *a_mountpoint);
WUR libfuse_cll_EXPORT int fuse_session_s_process(fuse_session_s *this);
static inline int fuse_session_s_fd(fuse_session_s *this);

// === definition of generated structures ======================================

// === inline methods of structure fuse_session_s ==============================

static inline void fuse_session_s_init(fuse_session_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void fuse_session_s_clear(fuse_session_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    fuse_session_unmount(*this);
    fuse_session_destroy(*this);
  }

  fuse_session_s_init(this);
}/*}}}*/

static inline void fuse_session_s_flush_all(fuse_session_s *this)
{/*{{{*/
}/*}}}*/

static inline void fuse_session_s_swap(fuse_session_s *this,fuse_session_s *a_second)
{/*{{{*/
  fuse_session_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void fuse_session_s_copy(const fuse_session_s *this,const fuse_session_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int fuse_session_s_compare(const fuse_session_s *this,const fuse_session_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void fuse_session_s_to_string(const fuse_session_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"fuse_session_s{%p}",*this);
}/*}}}*/
#endif

static inline int fuse_session_s_mount(fuse_session_s *this,const char *a_mountpoint)
{/*{{{*/
  fuse_session_unmount(*this);

  // - ERROR -
  if (fuse_session_mount(*this,a_mountpoint))
  {
    throw_error(FUSE_SESSION_MOUNT_ERROR);
  }

  return 0;
}/*}}}*/

static inline int fuse_session_s_fd(fuse_session_s *this)
{/*{{{*/
  return fuse_session_fd(*this);
}/*}}}*/

// === inline methods of generated structures ==================================

#endif

