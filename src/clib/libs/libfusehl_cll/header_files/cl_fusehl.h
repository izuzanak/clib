
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
  fusehl_reply_err(REQUEST,ERROR_ID);\
  return;\
}/*}}}*/

// === definition of fuse_lowlevel api =========================================

typedef uintptr_t fuse_req_t;
typedef uint64_t fuse_ino_t;

struct fuse_entry_param
{/*{{{*/
  fuse_ino_t ino;
  uint64_t generation;
  struct stat attr;
  double attr_timeout;
  double entry_timeout;
};/*}}}*/

struct fusehl_request_data_s
{/*{{{*/
  fuse_req_t req;
  fuse_ino_t parent;
  fuse_ino_t ino;
  const char *name;
  struct fuse_file_info *fi;
  char *buf;
  size_t size;
};/*}}}*/

struct fusehl_response_data_s
{/*{{{*/
  int err;
  struct fuse_entry_param ep;
  struct stat attr;
  const struct fuse_file_info *fi;
  size_t size;
};/*}}}*/

extern struct fusehl_request_data_s g_fusehl_request;
extern struct fusehl_response_data_s g_fusehl_response;

#define FUSEHL_REQUEST_LOOKUP  '\x00'
#define FUSEHL_REQUEST_GETATTR '\x01'
#define FUSEHL_REQUEST_READDIR '\x02'
#define FUSEHL_REQUEST_OPEN    '\x03'
#define FUSEHL_REQUEST_RELEASE '\x04'
#define FUSEHL_REQUEST_READ    '\x05'

#define FUSEHL_RESPONSE_ERR    '\xf0'
#define FUSEHL_RESPONSE_ENTRY  '\xf1'
#define FUSEHL_RESPONSE_ATTR   '\xf2'
#define FUSEHL_RESPONSE_DIRBUF '\xf3'
#define FUSEHL_RESPONSE_OPEN   '\xf4'
#define FUSEHL_RESPONSE_BUF    '\xf5'

// === definition of generated structures ======================================

// === inline methods of generated structures ==================================

#endif

