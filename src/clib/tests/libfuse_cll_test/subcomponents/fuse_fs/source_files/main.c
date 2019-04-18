
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === global functions ========================================================

void fuse_fs_lookup(fuse_req_t req,fuse_ino_t parent,const char *name)
{/*{{{*/
  fprintf(stderr,"fuse_fs_lookup\n");

  fuse_reply_err(req,ENOENT);
}/*}}}*/

void fuse_fs_getattr(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fuse_fs_getattr\n");

  fuse_reply_err(req,ENOENT);
}/*}}}*/


void fuse_fs_readdir(fuse_req_t req,fuse_ino_t ino,size_t size,off_t off,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fuse_fs_readdir\n");

  fuse_reply_err(req,ENOTDIR);
}/*}}}*/

void fuse_fs_open(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fuse_fs_open\n");

  fuse_reply_err(req,EACCES);
}/*}}}*/

void fuse_fs_read(fuse_req_t req,fuse_ino_t ino,size_t size,off_t off,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fuse_fs_read\n");
}/*}}}*/

int fuse_fd_event(void *a_fuse_session,unsigned a_index,epoll_event_s *a_event,epoll_s *a_epoll)
{/*{{{*/
  fprintf(stderr,"fuse_fd_event\n");

  fuse_session_s *session = (fuse_session_s *)a_fuse_session;
  return fuse_session_s_process(session);
}/*}}}*/

void signal_handler(int a_signum)
{/*{{{*/
  switch (a_signum)
  {
  case SIGCHLD:
    {
      int status;
      wait(&status);
    }
    break;
  default:

    // - terminate on all signals -
    __sync_add_and_fetch(&g_terminate,1);
  }
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  cassert(signal_s_simple_handler(signal_handler) == 0);

  {
    struct fuse_lowlevel_ops fuse_oper =
    {
      .lookup  = fuse_fs_lookup,
      .getattr = fuse_fs_getattr,
      .readdir = fuse_fs_readdir,
      .open    = fuse_fs_open,
      .read    = fuse_fs_read,
    };

    CONT_INIT_CLEAR(string_array_s,args);
    string_array_s_push_ptr(&args,"create");

    CONT_INIT_CLEAR(fuse_session_s,session);
    cassert(fuse_session_s_create(&session,&args,&fuse_oper,NULL) == 0);
    //cassert(fuse_session_s_mount(&session,"tests/libfuse_cll_test/resources/fuse_dir") == 0);
    cassert(fuse_session_s_mount(&session,"/dev/shm/test") == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,0);

    CONT_INIT_CLEAR(epoll_fd_s,epoll_fd);
    cassert((epoll_fd.fd = dup(fuse_session_s_fd(&session))) != -1);
    cassert(epoll_s_fd_callback(&epoll,&epoll_fd,EPOLLIN || EPOLLPRI,fuse_fd_event,&session,0) == 0);

    do {

      // - wait on events -
      int err;
      if ((err = epoll_s_wait(&epoll,16,500)))
      {
        cassert(err == ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED);
      }

      if (fuse_session_exited(session.se))
      {
        g_terminate = 1;
      }

      fprintf(stderr,"looping ...\n");

    } while(g_terminate == 0);
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

