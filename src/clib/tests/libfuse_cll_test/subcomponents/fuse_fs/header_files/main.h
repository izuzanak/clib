
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_linux.h"
include "cl_fuse.h"
@end

// === definition of generated structures ======================================

static inline void fuse_dirbuf_s_add(bc_array_s *this,fuse_req_t a_req,const char *a_name,fuse_ino_t a_ino);
static inline void fuse_dirbuf_s_reply(bc_array_s *this,fuse_req_t a_req,size_t a_size,off_t a_off);

// === inline methods of generated structures ==================================

static inline void fuse_dirbuf_s_add(bc_array_s *this,fuse_req_t a_req,const char *a_name,fuse_ino_t a_ino)
{/*{{{*/
  struct stat stbuf = {};
  stbuf.st_ino = a_ino;

  unsigned entry_size = fuse_add_direntry(a_req,NULL,0,a_name,NULL,0);

  unsigned old_used = this->used;
  bc_array_s_push_blanks(this,entry_size);
  fuse_add_direntry(a_req,this->data + old_used,this->size - old_used,a_name,&stbuf,this->used);
}/*}}}*/

static inline void fuse_dirbuf_s_reply(bc_array_s *this,fuse_req_t a_req,size_t a_size,off_t a_off)
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

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

