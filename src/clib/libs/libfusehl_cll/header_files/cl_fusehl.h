
#ifndef CL_LIB_FUSEHL_H
#define CL_LIB_FUSEHL_H

@begin
include "cl_struct.h"
@end

#define FUSE_USE_VERSION 30
#include <fuse3/fuse.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libfusehl_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libfusehl_cll_EXPORTS
#define libfusehl_cll_EXPORT __declspec(dllexport)
#else
#define libfusehl_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_FUSEHL_SESSION_CREATE_ERROR 1
#define ERROR_FUSEHL_SESSION_MOUNT_ERROR 2
#define ERROR_FUSEHL_SESSION_RECEIVE_BUFFER_ERROR 3

// - error macros -
#define throw_fuse_error(REQUEST,ERROR_ID)\
{/*{{{*/\
  debug_message_1(fprintf(stderr,"ERROR: " #ERROR_ID ", %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__));\
  fuse_reply_err(REQUEST,ERROR_ID);\
  return;\
}/*}}}*/

// === definition of fuse_lowlevel api =========================================

typedef uint32_t fuse_req_t;
typedef uint64_t fuse_ino_t;

// === definition of structure fuse_dirbuff_s ==================================

static inline void fuse_dirbuff_s_add(bc_array_s *this,fuse_req_t a_req,const char *a_name,fuse_ino_t a_ino);
static inline void fuse_dirbuff_s_reply(bc_array_s *this,fuse_req_t a_req,size_t a_size,off_t a_off);

// === definition of generated structures ======================================

// === inline methods of structure fuse_dirbuff_s ==============================

static inline void fuse_dirbuff_s_add(bc_array_s *this,fuse_req_t a_req,const char *a_name,fuse_ino_t a_ino)
{/*{{{*/
  
  // FIXME TODO add to dir buffer ...
  //struct stat stbuf = {};
  //stbuf.st_ino = a_ino;

  //unsigned entry_size = fuse_add_direntry(a_req,NULL,0,a_name,NULL,0);

  //unsigned old_used = this->used;
  //bc_array_s_push_blanks(this,entry_size);
  //fuse_add_direntry(a_req,this->data + old_used,this->size - old_used,a_name,&stbuf,this->used);
}/*}}}*/

static inline void fuse_dirbuff_s_reply(bc_array_s *this,fuse_req_t a_req,size_t a_size,off_t a_off)
{/*{{{*/
  if (a_off < this->used)
  {
    // FIXME TODO reply dir buffer ...
    //unsigned rest = this->used - a_off;
    //unsigned size = rest < a_size ? rest : a_size;

    //fuse_reply_buf(a_req,this->data + a_off,size);
  }
  else
  {
    // FIXME TODO reply dir buffer ...
    //fuse_reply_buf(a_req,NULL,0);
  }
}/*}}}*/

// === inline methods of generated structures ==================================

#endif

