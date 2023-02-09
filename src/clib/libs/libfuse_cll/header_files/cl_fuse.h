
#ifndef CL_LIB_FUSE_H
#define CL_LIB_FUSE_H

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

// - error macros -
#define throw_fuse_error(REQUEST,ERROR_ID)\
{/*{{{*/\
  debug_message_1(fprintf(stderr,"ERROR: " #ERROR_ID ", %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__));\
  fuse_reply_err(REQUEST,ERROR_ID);\
  return;\
}/*}}}*/

// - map fusehl callbacks to fuse callbacks -
#define fusehl_req_userdata(req)                 fuse_req_userdata(req)
#define fusehl_reply_buf(req,buf,size)           fuse_reply_buf(req,buf,size)
#define fusehl_reply_err(req,err)                fuse_reply_err(req,err)
#define fusehl_reply_entry(req,e)                fuse_reply_entry(req,e)
#define fusehl_reply_attr(req,attr,attr_timeout) fuse_reply_attr(req,attr,attr_timeout)
#define fusehl_reply_open(req,fi)                fuse_reply_open(req,fi)

#define fusehl_dirbuff_s_add(this,a_req,a_name,a_ino) \
  fuse_dirbuff_s_add(this,a_req,a_name,a_ino)

#define fusehl_dirbuff_s_reply(this,a_req,a_size,a_off) \
  fuse_dirbuff_s_reply(this,a_req,a_size,a_off)

// === definition of structure fuse_dirbuff_s ==================================

static inline void fuse_dirbuff_s_add(bc_array_s *this,fuse_req_t a_req,const char *a_name,fuse_ino_t a_ino);
static inline void fuse_dirbuff_s_reply(bc_array_s *this,fuse_req_t a_req,size_t a_size,off_t a_off);

// === definition of structure fuse_session_s ==================================

typedef struct fuse_session_s
{
  struct fuse_session *se;
  struct fuse_buf buffer;
} fuse_session_s;

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

// === inline methods of structure fuse_dirbuff_s ==============================

static inline void fuse_dirbuff_s_add(bc_array_s *this,fuse_req_t a_req,const char *a_name,fuse_ino_t a_ino)
{/*{{{*/
  struct stat stbuf = {};
  stbuf.st_ino = a_ino;

  unsigned entry_size = fuse_add_direntry(a_req,NULL,0,a_name,NULL,0);

  unsigned old_used = this->used;
  bc_array_s_push_blanks(this,entry_size);
  fuse_add_direntry(a_req,this->data + old_used,this->size - old_used,a_name,&stbuf,this->used);
}/*}}}*/

static inline void fuse_dirbuff_s_reply(bc_array_s *this,fuse_req_t a_req,size_t a_size,off_t a_off)
{/*{{{*/
  if (a_off < this->used)
  {
    unsigned rest = this->used - a_off;
    unsigned size = rest < a_size ? rest : a_size;

    fuse_reply_buf(a_req,this->data + a_off,size);
  }
  else
  {
    fuse_reply_buf(a_req,NULL,0);
  }
}/*}}}*/

// === inline methods of structure fuse_session_s ==============================

static inline void fuse_session_s_init(fuse_session_s *this)
{/*{{{*/
  this->se = NULL;
  memset(&this->buffer,0,sizeof(this->buffer));
}/*}}}*/

static inline void fuse_session_s_clear(fuse_session_s *this)
{/*{{{*/
  if (this->se != NULL)
  {
    fuse_session_unmount(this->se);
    fuse_session_destroy(this->se);
  }

  if (this->buffer.mem != NULL)
  {
    free(this->buffer.mem);
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
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void fuse_session_s_to_string(const fuse_session_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"fuse_session_s{se:%p}",this->se);
}/*}}}*/
#endif

static inline int fuse_session_s_mount(fuse_session_s *this,const char *a_mountpoint)
{/*{{{*/
  fuse_session_unmount(this->se);

  // - ERROR -
  if (fuse_session_mount(this->se,a_mountpoint))
  {
    throw_error(FUSE_SESSION_MOUNT_ERROR);
  }

  return 0;
}/*}}}*/

static inline int fuse_session_s_fd(fuse_session_s *this)
{/*{{{*/
  return fuse_session_fd(this->se);
}/*}}}*/

// === inline methods of generated structures ==================================

#endif

